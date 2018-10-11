
#include "Resource.h"
#include "Camera.h"
#include "Clock.h"
#include "InputManager.h"
#include "Terrain.h"
#include "PlayerObject.h"
#include "GeometryModel.h"
#include "TessModel.h"
#include "FrameBuffer.h"
#include "Text.h"

#include "libs/glm/gtx/string_cast.hpp"

float g_DeltaTime = 0.0f;

Terrain* t = new Terrain;
PlayerObject* p = new PlayerObject;
Text* tx0;
Text* tx1;
Text* tx2;
GeometryModel* gm = new GeometryModel;
TessModel* tm = new TessModel;
FrameBuffer* fb = new FrameBuffer;

void ProcessInput();


/*Initializing the entire program. This function gets called in glutInit()*/
/*This turns on backface culling and the depth test (useful for some other*/
/*things). Does not return anything. Should always complete successfully. */
void Initialize() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_MULTISAMPLE);
	EntityManager::GetInstance();
	Camera::GetInstance();
	InputManager::Init();
	t->Init();
	p->Init(glm::vec3(0.7f, 1.0f, 0.0f));
	tx0 = new Text("WASD to move", ARIAL, glm::vec2(30.0f, 850.0f), 0.7f);
	tx1 = new Text("P to cycle through display modes", ARIAL, glm::vec2(30.0f, 800.0f), 0.7f);
	tx2 = new Text("G to cycle through grass modes", ARIAL, glm::vec2(30.0f, 750.0f), 0.7f);

	tm->Init();
	tm->Pos = glm::vec3(0.9f, 1.3f, t->GetHeight(0.9f, 1.3f) + 0.1f);
	gm->Init(glm::vec3(0.0f, 400.0f, 400.0f));
	fb->Init();
}


/*Where rendering occurs. You should have one single object that has a	  */
/*Render() function that calls all your object's Render functions. There  */
/*should really only be a single render call in here.					  */
void Render(void) {

	//RENDER ITEMS HERE//
	fb->BeginCapture();
	t->Render();
	tx0->Render();
	tx1->Render();
	tx2->Render();
	gm->Render();
	tm->Render();
	p->Render();
	fb->Render();
	//-----------------//

	glutSwapBuffers();
}


/*This is where the processing happens. Again, should have a single object*/
/*that calls all other objects render functions. This is where you also   */
/*pass in DeltaTime (as updates to the g_DeltaTime are called at the start*/
/*of this loop.															  */
void Process(void) {
	g_DeltaTime = Clock::GetDeltaTime();

	//DO LOGIC PROCESSING HERE//
	//Use g_DeltaTime if possible
	ProcessInput();
	p->Process(g_DeltaTime);
	fb->dt += g_DeltaTime / 2.0f;
	Camera::Process();

	//------------------------//
	glutPostRedisplay();
}


/*Any functions that need to be called when the the program ends. Clear   */
/*any pointers and stuff that need to be cleared here.					  */		
void Exit(void) {
	//ANY EXIT FUNCTIONS THAT NEED TO BE RUN GO HERE
}


/*The main loop of the program. None of this stuff should need to be	  */
/*changed at all. Window size is changed under Resource.h				  */
int main(int argc, char **argv) {
	glutInit(&argc, argv);

	//The initialization parameters of the window
	glutInitDisplayMode(
		GLUT_DEPTH | 
		GLUT_DOUBLE | 
		GLUT_RGBA | 
		GLUT_MULTISAMPLE
	);

	//Initial window position (top left of the window)
	glutInitWindowPosition(0, 0);

	//Window size. Change under Resource.h
	glutInitWindowSize(
		static_cast<int>(UTILS::WindowWidth), 
		static_cast<int>(UTILS::WindowHeight)
	);

	//Window Title
	glutCreateWindow("Terrain Generation");

	//Window Background color (default pixel color when nothing is rendered over it)
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//Glut initialization functions.
	glewInit();
	Initialize();
	glutDisplayFunc(Render);
	glutIdleFunc(Process);
	glutIgnoreKeyRepeat(1);
	glutCloseFunc(Exit);
	glutMainLoop();
	return 0;
}

void ProcessInput(){


	if (InputManager::KeyArray['w'] == KEY_HELD) {
		p->ObjPos = glm::vec3(p->ObjPos.x, p->ObjPos.y += 1.1f * g_DeltaTime, p->ObjPos.z );
	}
	if (InputManager::KeyArray['s'] == KEY_HELD){
		p->ObjPos = glm::vec3(p->ObjPos.x, p->ObjPos.y -= 1.1f * g_DeltaTime, p->ObjPos.z);
	}
	if (InputManager::KeyArray['a'] == KEY_HELD){
		p->ObjPos = glm::vec3(p->ObjPos.x -= 1.1f * g_DeltaTime, p->ObjPos.y, p->ObjPos.z);
	}
	if (InputManager::KeyArray['d'] == KEY_HELD){
		p->ObjPos = glm::vec3(p->ObjPos.x += 1.0f * g_DeltaTime, p->ObjPos.y, p->ObjPos.z );
	}
	if (InputManager::KeyArray['p'] == KEY_FIRST_PRESS) {
		fb->CycleDisplayMode();
	}
	if (InputManager::KeyArray['g'] == KEY_FIRST_PRESS) {
		t->ToggleGrass();
	}

	if (p->ObjPos.x > 2.54f) {
		p->ObjPos.x = 2.54f;
	}
	if (p->ObjPos.x < -2.54f) {
		p->ObjPos.x = -2.54f;
	}
	if (p->ObjPos.y > 2.54f) {
		p->ObjPos.y = 2.54f;
	}
	if (p->ObjPos.y < -2.54f) {
		p->ObjPos.y = -2.54f;
	}

	gm->Pos = p->ObjPos;
	gm->Pos.z += 0.1f;
	p->ObjPos.z = t->GetHeight(p->ObjPos.x, p->ObjPos.y) + 0.04f;
	Camera::GetPos() = -glm::vec3(p->ObjPos.x, p->ObjPos.y + 1.6f, p->ObjPos.z - 0.7f);
	InputManager::ProcessKeyInput(g_DeltaTime);
}

