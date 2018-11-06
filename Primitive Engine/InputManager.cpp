#include "InputManager.h"
#include "Camera.h"
#include "Clock.h"

unsigned char InputManager::KeyArray[255];
unsigned char InputManager::KeySpecialArray[255];
unsigned int InputManager::MouseButtonArray[3];
glm::vec2 InputManager::v2MouseLastPos = {0.0f, 0.0f};
glm::vec2 InputManager::v2MouseCurrentPos = {0.0f, 0.0f};


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

	//processing the mouse buttons
	for (int i = 0; i < 3; ++i){
		if (MouseButtonArray[i] == KEY_FIRST_PRESS){
			MouseButtonArray[i] = KEY_HELD;
		}
	}

	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);

	glutMouseFunc(InputManager::ProcessMouseButtons);
	glutMotionFunc(InputManager::ProcessMouseMovement);
	v2MouseLastPos = v2MouseCurrentPos;
}

void InputManager::Init(){
	for (int i = 0; i < 255; ++i){
		KeyArray[i] = KEY_RELEASED;
		KeySpecialArray[i] = KEY_RELEASED;
	}
	for (int i = 0; i < 3; ++i){
		MouseButtonArray[i] = KEY_RELEASED;
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		v2MouseLastPos = {x, y};
		v2MouseCurrentPos = {x, y};
		MouseButtonArray[0] = KEY_FIRST_PRESS;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		MouseButtonArray[0] = KEY_RELEASED;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		v2MouseLastPos = {x, y};
		v2MouseCurrentPos = {x, y};
		MouseButtonArray[1] = KEY_FIRST_PRESS;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
		MouseButtonArray[1] = KEY_RELEASED;
	}
}

void InputManager::ProcessMouseMovement(int x, int y){
	v2MouseCurrentPos = {x, y};
}

