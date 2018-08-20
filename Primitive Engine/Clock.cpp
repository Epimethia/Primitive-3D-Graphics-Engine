#include "Clock.h"

std::shared_ptr<Clock> Clock::ClockPtr = nullptr;
float Clock::OldTime = 0.0f;

Clock::Clock() {
	OldTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
};

std::shared_ptr<Clock> Clock::GetInstance() {
	if (ClockPtr == nullptr) ClockPtr = std::shared_ptr <Clock>(new Clock());
	return ClockPtr;
};

float Clock::GetDeltaTime() {
	float CurrentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	float TimeDelta = (CurrentTime - OldTime);
	OldTime = CurrentTime;
	return TimeDelta;
};
