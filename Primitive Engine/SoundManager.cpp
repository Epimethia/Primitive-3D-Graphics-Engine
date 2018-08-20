#include "SoundManager.h"

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {
	/*delete audioMgr;
	delete ShootFX;
	delete bgmTheme;
	delete channel;
	audioMgr = nullptr;
	bgmTheme = nullptr;
	channel = nullptr;
	ShootFX = nullptr;*/
}

bool SoundManager::Init() {
	if (FMOD::System_Create(&audioMgr) != FMOD_OK) {
		std::cout << "Failed to create Audio Manager \n";
		return false;
	}
	if (audioMgr->init(50, FMOD_INIT_NORMAL, 0) != FMOD_OK) {
		std::cout << "Failed to initialize Audio Manager \n";
		return false;
	}
	std::cout << "Audio Manager created successfully \n";
	return true;
}

void SoundManager::PlayBGM() {
	audioMgr->playSound(bgmTheme, 0, false, &channel);
	channel->setVolume(0.3f);
	channel->setPitch(1.5f);
}

void SoundManager::PlayShootFX() {
	audioMgr->playSound(ShootFX, 0, false, &channel);
	channel->setVolume(0.6f);
	channel->setPitch(4.0f);
}

void SoundManager::ResumeBGM() {
	channel->setPaused(false);
}

void SoundManager::PauseBGM() {
	channel->setPaused(true);
}

const bool SoundManager::LoadAudio() {
	if (audioMgr->createSound(BG_MUSIC_LOOP, FMOD_DEFAULT, 0, &bgmTheme) != FMOD_OK) {
		std::cout << "Failed to create Sound " << BG_MUSIC_LOOP << std::endl;
		return false;
	}
	if (audioMgr->createSound(SHOOT_FX, FMOD_DEFAULT, 0, &ShootFX) != FMOD_OK) {
		std::cout << "Failed to create Sound " << BG_MUSIC_LOOP << std::endl;
		return false;
	}
	bgmTheme->setMode(FMOD_LOOP_NORMAL);
	std::cout << "Music Loaded Successfully\n";
	return true;
}
