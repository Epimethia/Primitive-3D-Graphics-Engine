#include "InputManager.h"

unsigned char InputManager::KeyArray[255];
unsigned char InputManager::KeySpecialArray[255];

InputManager::InputManager() {
	for (int i = 0; i < 255; ++i) {
		KeyArray[i] = KEY_RELEASED;
		KeySpecialArray[i] = KEY_RELEASED;
	}
}

void InputManager::ProcessKeyInput() {
	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);
	glm::vec3 Target;

	//DO NORMAL KEY INPUTS HERE

	//RIGHT KEY INPUT
	if (KeyArray['d'] == KEY_HELD) {
		std::cout << "D key held\n";
	} else if (KeyArray['d'] == KEY_FIRST_PRESS) KeyArray['d'] = KEY_HELD;

	//LEFT KEY INPUT
	else if (KeyArray['a'] == KEY_HELD) {
		std::cout << "A key held\n";
	} else if (KeyArray['a'] == KEY_FIRST_PRESS) KeyArray['a'] = KEY_HELD;

	//UP KEY INPUT
	else if (KeyArray['w'] == KEY_HELD) {
		std::cout << "W key held\n";
	} else if (KeyArray['w'] == KEY_FIRST_PRESS) KeyArray['w'] = KEY_HELD;

	else if (KeyArray['s'] == KEY_HELD) {
		std::cout << "S key held\n";
	} else if (KeyArray['s'] == KEY_FIRST_PRESS) KeyArray['s'] = KEY_HELD;
}

//Menu input
void InputManager::ProcessSpecialKeyInput() {
	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);
	
	//Impliment Special Keys here

}

void InputManager::NormKeyDown(unsigned char key, int x, int y) {
	KeyArray[key] = KEY_FIRST_PRESS;
}

void InputManager::NormKeyUp(unsigned char key, int x, int y) {
	KeyArray[key] = KEY_RELEASED;
}

void InputManager::SpecialKeyDown(int key, int x, int y) {
	KeySpecialArray[key] = KEY_FIRST_PRESS;
}

void InputManager::SpecialKeyUp(int key, int x, int y) {
	KeySpecialArray[key] = KEY_RELEASED;
}
