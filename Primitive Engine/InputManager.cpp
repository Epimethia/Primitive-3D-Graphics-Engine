#include "InputManager.h"
#include "Camera.h"

unsigned char InputManager::KeyArray[255];
unsigned char InputManager::KeySpecialArray[255];
bool InputManager::AntiAliasing = false;

InputManager::InputManager() {

}

void InputManager::ProcessKeyInput(float _dt) {
	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);
	glm::vec3 Target;
	glm::vec3 Pos = Camera::GetPos();

	//DO NORMAL KEY INPUTS HERE

	//RIGHT KEY INPUT
	if (KeyArray['d'] == KEY_HELD) {
		Pos = glm::vec3(Pos.x - (0.25f * _dt), Pos.y, Pos.z);
	}
	if (KeyArray['d'] == KEY_FIRST_PRESS) KeyArray['d'] = KEY_HELD;

	//LEFT KEY INPUT
	if (KeyArray['a'] == KEY_HELD) {
		Pos = glm::vec3(Pos.x + (0.25f * _dt), Pos.y, Pos.z);
	}
	if (KeyArray['a'] == KEY_FIRST_PRESS) KeyArray['a'] = KEY_HELD;

	//UP KEY INPUT
	if (KeyArray['w'] == KEY_HELD) {
		Pos = glm::vec3(Pos.x, Pos.y - (0.25f * _dt), Pos.z);
	}
	if (KeyArray['w'] == KEY_FIRST_PRESS) KeyArray['w'] = KEY_HELD;

	if (KeyArray['s'] == KEY_HELD) {
		Pos = glm::vec3(Pos.x, Pos.y + (0.25f * _dt), Pos.z);
	}
	if (KeyArray['s'] == KEY_FIRST_PRESS) KeyArray['s'] = KEY_HELD;

	if (KeyArray[(char)32] == KEY_HELD) {
		Pos = glm::vec3(Pos.x, Pos.y , Pos.z - (0.25f * _dt));
	}
	if (KeyArray[(char)32] == KEY_FIRST_PRESS) KeyArray[(char)32] = KEY_HELD;

	if (KeyArray['p'] == KEY_FIRST_PRESS) {
		if (AntiAliasing) {
			AntiAliasing = false;
			std::cout << "Disabled AntiAliasing\n";
			glutSetOption(GLUT_MULTISAMPLE, 0);
			glDisable(GL_MULTISAMPLE);
		}
		else {
			AntiAliasing = true;
			std::cout << "Enabled AntiAliasing\n";
			glutSetOption(GLUT_MULTISAMPLE, 8);
			glEnable(GL_MULTISAMPLE);
		}
		KeyArray['p'] = KEY_HELD;
	};

	//Impliment Special Keys here
	if (KeySpecialArray[GLUT_KEY_SHIFT_L] == KEY_HELD) {
		Pos = glm::vec3(Pos.x, Pos.y, Pos.z + (0.25f * _dt));
	}
	if (KeySpecialArray[GLUT_KEY_SHIFT_L] == KEY_FIRST_PRESS) KeySpecialArray[GLUT_KEY_SHIFT_L] = KEY_HELD;
	Camera::GetPos() = Pos;
}

//Menu input
void InputManager::ProcessSpecialKeyInput() {
	glutSpecialFunc(InputManager::SpecialKeyDown);
	glutSpecialUpFunc(InputManager::SpecialKeyUp);
	glutKeyboardFunc(InputManager::NormKeyDown);
	glutKeyboardUpFunc(InputManager::NormKeyUp);
	glm::vec3 Pos = Camera::GetPos();
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
