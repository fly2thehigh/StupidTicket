#pragma once
#include <functional>

struct StationInfo
{
	char alpha[4];	//bjb
	char name[32];	//±±¾©±±
	char key[4];	//VAP
	char spell[32];	//beijingbei
	char alpha1[4];	//bjb
	int num;		//0
};

class ScopeGuard
{
public:
	ScopeGuard(std::function<void()> leave)
		:fn(leave), ignore(false) {};
	~ScopeGuard()
	{
		if (!ignore)
			fn();
	}
	inline void dismiss()
	{
		ignore = true;
	}
private:
	bool ignore;
	std::function<void()> fn;
};