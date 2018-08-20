#pragma once
#include "Resource.h"
class Clock {
public:
	~Clock() {};
	static std::shared_ptr<Clock> GetInstance();
	static void DestroyInstance() { ClockPtr = nullptr; };
	static float GetDeltaTime();
private:
	Clock();
	static std::shared_ptr<Clock> ClockPtr;
	static float OldTime;
};

