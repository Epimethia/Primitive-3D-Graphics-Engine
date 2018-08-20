#include "Menu.h"

Menu::Menu() {}

Menu::~Menu() {};

Menu::Menu(std::vector<std::string> _OptVect, glm::vec2 _Pos) {
	ShaderLoader sl;
	TextShader = sl.CreateProgram(TEXT_VERT_SHADER, TEXT_FRAG_SHADER);
	NumMenuOptions = 0;
	CurrentOption = 0;
	glm::vec2 Position = _Pos;

	//Iterating through the provided option vect
	for (unsigned int i = 0; i < _OptVect.size(); ++i) {
		OptionVect.push_back(std::make_shared<Text>(_OptVect[i], PIRATEFONT, Position, TextShader, 40));
		Position.y -= 60.0f;
	}
	NumMenuOptions = _OptVect.size() - 1;
}

void Menu::IncrementMenu() {
	if (CurrentOption == NumMenuOptions) CurrentOption = 0;
	else ++CurrentOption;
}

void Menu::DecrementMenu() {
	if (CurrentOption == 0) CurrentOption = NumMenuOptions;
	else --CurrentOption;
}

void Menu::Process(int& _Option) {
	if (InputManager::KeySpecialArray[GLUT_KEY_UP] == KEY_FIRST_PRESS) {
		DecrementMenu();
	}
	else if (InputManager::KeySpecialArray[GLUT_KEY_DOWN] == KEY_FIRST_PRESS) {
		IncrementMenu();
	}
	else if (InputManager::KeyArray['\r'] == KEY_FIRST_PRESS) {
		_Option = CurrentOption;
		CurrentOption = 0;
		SelectOption(CurrentOption);
		return;
	}
	
	SelectOption(CurrentOption);

	_Option = 999;
}

void Menu::ReplaceOption(int _OptIndex, std::string _OptionText) {
	if (_OptIndex > NumMenuOptions || _OptIndex < 0) return;
	OptionVect[_OptIndex]->SetText(_OptionText);
}

void Menu::SelectOption(unsigned int _Option) {
	for (unsigned int i = 0; i < OptionVect.size(); ++i) {
		if (i == _Option) {
			OptionVect[i]->SetColor(glm::vec3(0.10, 0.2, 0.37));
			OptionVect[i]->SetScale(0.8f);
		}
		else {
			OptionVect[i]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
			OptionVect[i]->SetScale(0.5f);
		}
	}
}

void Menu::Render() {
	for (auto it : OptionVect) it->Render();
}
