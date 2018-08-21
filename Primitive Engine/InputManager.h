#pragma once
#include "Resource.h"


enum KEYSTATE {
	KEY_FIRST_PRESS,
	KEY_RELEASED,
	KEY_HELD,
};

class InputManager {
public:
	InputManager();
	static void ProcessKeyInput();
	static void ProcessSpecialKeyInput();

	static unsigned char KeyArray[255];
	static unsigned char KeySpecialArray[255];

private:	
	static void NormKeyDown(unsigned char key, int x, int y);
	static void NormKeyUp(unsigned char key, int x, int y);
	static void SpecialKeyDown(int key, int x, int y);
	static void SpecialKeyUp(int key, int x, int y);
	
	static bool AntiAliasing;
};