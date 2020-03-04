#include "pch.h"
#include <stdarg.h>
#include <io.h>
#include <string>
#include "curl.h"
#include "fn.h"

static bool download_station_file(const char* file);
static size_t on_write(const void* ptr, size_t size, size_t count, FILE* stream);
static void utf8_to_ascii(const char* utf8_str, char** asc_str, int* bytes);
static void parse_station_str(const char* str, std::vector<StationInfo>& stations);
static void parse_single_station(const char* str, StationInfo& si);

const char* fmt(const char* format, ...)
{
	static char buf[1024] = "\0";
	buf[0] = '\0';
	va_list ap;
	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);
	return buf;
}

void log(const char* str)
{

}

bool query_staions(std::vector<StationInfo>& stations, bool force)
{
	char fpath[MAX_PATH] = "\0";

	stations.clear();
	GetEnvironmentVariable("APPDATA", fpath, sizeof(fpath));
	strcat(fpath, "\\StupidTicket");
	if (_access(fpath, 0))
		CreateDirectory(fpath, 0);
	strcat(fpath, "\\station_name.js");
	if (force || _access(fpath, 0)) // not exist, download it
	{
		if (!download_station_file(fpath))
			return false;
	}

	FILE* fp = fopen(fpath, "r");
	if (!fp)
		return false;

	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	char* origin_str = new char[length];
	const int bufSize = 10240;
	char buf[bufSize] = "\0";
	long cur = 0;
	long bytes = 0;
	char* p = nullptr;
	char tag[] = "station_names";
	fseek(fp, 0, SEEK_SET);
	while (true)
	{
		bytes = fread(buf, 1, bufSize, fp);
		if (bytes <= 0)
			break;
		if (!p)
		{
			p = strstr(buf, tag);
			if (p)
			{
				p += strlen(tag);
				while (*p != '\'' && *p!='\0')
					p++;
				p++;
				strcpy(origin_str, p);
				cur = bytes - (int)(p-buf);
			}
			continue;
		}
		memcpy(origin_str + cur, buf, bytes);
		cur += bytes;
	}
	char* data = nullptr;
	int data_len = 0;
	utf8_to_ascii(origin_str, &data, &data_len);
	if (data)
	{
		parse_station_str(data, stations);
		delete[] data;
	}
	delete[] origin_str;

	fclose(fp);
	return true;
}

bool download_station_file(const char* file)
{
	CURL* curl = NULL;
	FILE* fp = NULL;

	ScopeGuard sg([&]() {
		if (curl)
			curl_easy_cleanup(curl);
		if (fp)
			fclose(fp);
		});

	curl = curl_easy_init();
	if (!curl)
		return false;
	fp = fopen(file, "wb+");
	if (!fp)
		return false;

	curl_easy_setopt(curl, CURLOPT_URL,
		"https://kyfw.12306.cn/otn/resources/js/framework/station_name.js?station_version=1.9053");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, on_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		log(fmt("curl_easy_perform failed with: %d", res));
		return false;
	}
	int code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if (code != 200)
	{
		log(fmt("http response code: %d", code));
		return false;
	}
	return true;
}

size_t on_write(const void* ptr, size_t size, size_t count, FILE* stream)
{
	return fwrite(ptr, size, count, stream);
}

void utf8_to_ascii(const char* utf8_str, char** asc_str, int* bytes)
{
	if (!utf8_str || !asc_str || !bytes)
		return;
	*bytes = 0;
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[wlen];
	MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, wstr, wlen);
	int clen = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, 0, 0);
	*asc_str = new char[clen];
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, *asc_str, clen, NULL, NULL);
	delete[] wstr;
}

void parse_station_str(const char* str, std::vector<StationInfo>& stations)
{
	if (!str)
		return;
	const char* p = str, * q = nullptr;
	char buf[256] = "\0";
	StationInfo si;
	int len = 0;
	while (true)
	{
		p = strchr(p, '@');
		if (!p)
			break;
		p++;
		q = strchr(p, '@');
		if (q)
		{
			len = q - p;
			memcpy(buf, p, len);
			buf[len] = '\0';
		}
		else
		{
			strcpy(buf, p);
			char *q = strchr(buf, '\'');
			if (q)
				*q = '\0';
		}
		parse_single_station(buf, si);
		stations.push_back(si);
	}
}

void parse_single_station(const char* str, StationInfo& si)
{
	memset(&si, 0, sizeof(si));
	std::vector<std::string> strs;
	char* token = strtok((char *)str, "|");
	while (token)
	{
		strs.push_back(token);
		token = strtok(NULL, "|");
	}
	if (strs.size() >= 6)
	{
		strcpy(si.alpha, strs[0].c_str());
		strcpy(si.name, strs[1].c_str());
		strcpy(si.key, strs[2].c_str());
		strcpy(si.spell, strs[3].c_str());
		strcpy(si.alpha1, strs[4].c_str());
		si.num = atoi(strs[5].c_str());
	}
}