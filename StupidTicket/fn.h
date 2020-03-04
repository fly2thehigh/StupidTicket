#pragma once
#include <vector>
#include "tp.h"

const char* fmt(const char* format, ...);
void log(const char* str);
bool query_staions(std::vector<StationInfo>& stations, bool force);