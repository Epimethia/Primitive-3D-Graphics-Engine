#pragma once
#include "Resource.h"
#include "Entity.h"
#include "InputManager.h"

enum GameState {
	START_MENU,
	GAME_PLAY,
	OPTION_MENU,
	MULTIPLAYER_LOBBY,
	HOST_LOBBY,
	CLIENT_LOBBY,
	END_MENU
};

class Menu {
public:
	Menu();
	~Menu();
	Menu(std::vector<std::string> _OptVect, glm::vec2 Pos);
	void IncrementMenu();
	void DecrementMenu();
	int GetCurrentOpt() { return CurrentOption; };
	void ReplaceOption(int _OptIndex, std::string _OptionText);
	void SelectOption(unsigned int _Option);
	void Process(int& _Option);
	void Render();

private:
	std::vector<std::shared_ptr<Text>> OptionVect;

	GLuint TextShader;
	int NumMenuOptions;
	int CurrentOption;
};