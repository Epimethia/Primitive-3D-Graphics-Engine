#include "InputManager.h"
#include "Camera.h"

unsigned char InputManager::KeyArray[255];
unsigned char InputManager::KeySpecialArray[255];

InputManager::InputManager() {

}

void InputManager::ProcessInputs() {

	for (int i = 0; i < 255; ++i){
		if (KeyArray[i] == KEY_FIRST_PRESS){
			KeyArray[i] = KEY_HELD;
		}
		if (KeySpecialArray[i] == KEY_FIRST_PRESS){
			KeySpecialArray[i] = KEY_HELD;
		}
	}

	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);

	/*glutMouseFunc(InputManager::ProcessMouseButtons);
	glutPassiveMotionFunc(InputManager::ProcessMousePassiveMovement);
	glutMotionFunc(InputManager::ProcessMouseMovement);*/
}

void InputManager::Init(){
	for (int i = 0; i < 255; ++i){
		KeyArray[i] = KEY_RELEASED;
		KeySpecialArray[i] = KEY_RELEASED;
	}
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

void InputManager::ProcessMouseButtons(int button, int state, int x, int y){

}

void InputManager::ProcessMouseMovement(int x, int y){}

void InputManager::ProcessMousePassiveMovement(int x, int y){}

