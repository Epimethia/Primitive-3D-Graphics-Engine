
#include "Resource.h"
#include "Camera.h"
#include "Clock.h"
#include "InputManager.h"
#include "Terrain.h"
#include "PlayerObject.h"
#include "GeometryModel.h"
#include "Text.h"

float g_DeltaTime = 0.0f;

Terrain* t = new Terrain;
PlayerObject* p = new PlayerObject;
Text* tx;
GeometryModel* gm = new GeometryModel;
void ProcessInput();

/*Initializing the entire program. This function gets called in glutInit()*/
/*This turns on backface culling and the depth test (useful for some other*/
/*things). Does not return anything. Should always complete successfully. */
void Initialize() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glDisable(GL_MULTISAMPLE);
	EntityManager::GetInstance();
	Camera::GetInstance();
	InputManager::Init();
	t->Init();
	p->Init(glm::vec3(0.0f, 0.0f, 0.0f));
	tx = new Text("Hello", ARIAL, glm::vec2(0.0f, 800.0f), 10);
	gm->Init(p->ObjPos);
}

/*Where rendering occurs. You should have one single object that has a	  */
/*Render() function that calls all your object's Render functions. There  */
/*should really only be a single render call in here.					  */
void Render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//RENDER ITEMS HERE//
	p->Render();
	t->Render();
	tx->Render();
	gm->Render();
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
	glutCreateWindow("Piroots of the CurryBeans");

	//Window Background color (default pixel color when nothing is rendered over it)
	glClearColor(0.0, 0.0, 0.0, 1.0);

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
	if (p->ObjPos.x > 2.55f){
		p->ObjPos.x = 2.55f;
		return;
	}
	if (p->ObjPos.x < -2.55f){
		p->ObjPos.x = -2.55f;
		return;
	}
	if (p->ObjPos.y > 2.55f){
		p->ObjPos.y = 2.55f;
		return;
	}
	if (p->ObjPos.y < -2.55f){
		p->ObjPos.y = -2.55f;
		return;
	}

	if (InputManager::KeyArray['w'] == KEY_HELD) {
		p->ObjPos = glm::vec3(p->ObjPos.x, p->ObjPos.y += 0.1f * g_DeltaTime, p->ObjPos.z );
	}
	if (InputManager::KeyArray['s'] == KEY_HELD){
		p->ObjPos = glm::vec3(p->ObjPos.x, p->ObjPos.y -= 0.1f * g_DeltaTime, p->ObjPos.z);
	}
	if (InputManager::KeyArray['a'] == KEY_HELD){
		p->ObjPos = glm::vec3(p->ObjPos.x -= 0.1f * g_DeltaTime, p->ObjPos.y, p->ObjPos.z);
	}
	if (InputManager::KeyArray['d'] == KEY_HELD){
		p->ObjPos = glm::vec3(p->ObjPos.x += 0.1f * g_DeltaTime, p->ObjPos.y, p->ObjPos.z );
	}
	gm->Pos = p->ObjPos;
	gm->Pos.z += 0.1f;
	p->ObjPos.z = t->GetHeight(p->ObjPos.x, p->ObjPos.y) + 0.01f;
	Camera::GetPos() = -glm::vec3(p->ObjPos.x, p->ObjPos.y + 1.0f, p->ObjPos.z-1.3f);
	InputManager::ProcessKeyInput(g_DeltaTime);
}
