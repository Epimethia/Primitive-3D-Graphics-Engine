#pragma once
#ifndef __RESOURCE_FILE_H__
#define __RESOURCE_FILE_H__

#pragma once

//Include files
#include "libs\glew\glew.h"
#include "libs\freeglut\freeglut.h"
#include "libs\soil\SOIL.h"
#include "libs\glm\glm.hpp"
#include "libs\glm\gtc\matrix_transform.hpp"
#include "libs\glm\gtc\type_ptr.hpp"
#include "libs\FMOD\fmod.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

//VLD
//#include "Dependencies\Visual Leak Detector\include\vld.h"

//Main Includes
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <iterator>
#include <memory>
#include <stdlib.h>
#include <time.h>

namespace UTILS {
	const int WindowWidth = 750;
	const int WindowHeight = 750;
}

#pragma region SHADER MACROS
//Entity Shader
#define VERT_SHADER				"Assets/Shaders/Entity.vs"
#define FRAG_SHADER				"Assets/Shaders/Entity.fs"

//Text Shader
#define TEXT_VERT_SHADER		"Assets/Shaders/Text.vs"
#define TEXT_FRAG_SHADER		"Assets/Shaders/Text.fs"

//Model Shader
#define MODEL_VERT_SHADER		"Assets/Shaders/Model.vs"
#define MODEL_FRAG_SHADER		"Assets/Shaders/Model.fs"

//CubeMap Shader
#define CUBEMAP_VERT_SHADER		"Assets/Shaders/CubeMap.vs"
#define CUBEMAP_FRAG_SHADER		"Assets/Shaders/CubeMap.fs"

#pragma endregion	SHADER MACROS END

//Sprite Macros
#define BULLET_SPRITE			"Assets/Sprite/Rayman.jpg"
#define RIVERBED_TEXTURE		"Assets/Sprite/RiverBedTexture.png"

//CubeMap Macro
#define CUBEMAP_FILE_PATH		"Assets/Sprite/"

//Text Macros
#define ARIAL					"Assets/Fonts/arial.ttf"
#define SNACKERCOMIC			"Assets/Fonts/SnackerComic.ttf"
#define WALLTOGRAPHUI			"Assets/Fonts/waltographUI.ttf"
#define PIRATEFONT				"Assets/Fonts/PretendPirates-Regular.ttf"

//Model Macros					
#define PLAYER_MODEL			"Assets/Models/PlayerBoat/Boat1.obj"
#define PLAYER_STENCIL_MODEL	"Assets/Models/Stencil/Boat1.obj"
#define WAVE_MODEL				"Assets/Models/wave/Wave2.obj"
#define POWERUP_MODEL			"Assets/Models/PowerUp3/Power_Up_3.obj"

#endif //__RESOURCE_FILE_H__