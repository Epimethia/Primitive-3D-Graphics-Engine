
#include "Resource.h"
#include "Camera.h"
#include "Clock.h"
#include "InputManager.h"
#include "Terrain.h"
#include "FrameBuffer.h"
#include "Text.h"
#include "ParticleSystem.h"
#include "PlayerObject.h"
#include "ssAnimatedModel.h"

float g_DeltaTime = 0.0f;

Terrain* t = new Terrain;
Text* tx0;
Text* tx1;
Text* tx2;
FrameBuffer* fb = new FrameBuffer;
ParticleSystem* ps;
PlayerObject* p = new PlayerObject;


//init 
std::shared_ptr<ssAnimatedModel> animatedModel;


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

	InputManager::Init();
	ps = new ParticleSystem(glm::vec3(0.7f, 1.0f, 0.0f));
	ps->Init();
	t->Init();

	p->Init(glm::vec3(0.7f, 1.0f, 0.0f));
	p->ObjPos.y = (t->GetHeight(p->ObjPos.x, p->ObjPos.z) + 0.2f) * -1.5f;
	Camera::GetInstance()->GetPos() = -glm::vec3(p->ObjPos.x, p->ObjPos.y, p->ObjPos.z - 0.7f);

	tx0 = new Text("WASD to move", ARIAL, glm::vec2(30.0f, 850.0f), 0.7f);
	tx1 = new Text("P to cycle through display modes", ARIAL, glm::vec2(30.0f, 800.0f), 0.7f);
	tx2 = new Text("G to cycle through grass modes", ARIAL, glm::vec2(30.0f, 750.0f), 0.7f);
	fb->Init();

	animatedModel = std::make_shared<ssAnimatedModel>(
		"Assets/Models/TheDude/theDude.dae",
		"Assets/Models/TheDude/theDude.png" // texture name  
	);

	animatedModel->setCurrentAnimation(0, 30);// set idle animation a
	//animatedModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	animatedModel->setRotation(glm::vec3(0.0f));
	animatedModel->setScale(glm::vec3(0.001f)); 
	animatedModel->setSpeed(0.0f);

}


/*Where rendering occurs. You should have one single object that has a	  */
/*Render() function that calls all your object's Render functions. There  */
/*should really only be a single render call in here.					  */
void Render(void) {
	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//RENDER ITEMS HERE//
	fb->BeginCapture();
	t->Render();
	//tx0->Render();
	//tx1->Render();
	//tx2->Render();
	ps->Render();
	//p->Render();
	animatedModel->render(g_DeltaTime, t);

	fb->Render();
	//-----------------//

	glutSwapBuffers();
}


/*This is where the processing happens. Again, should have a single object*/
/*that calls all other objects render functions. This is where you also   */
/*pass in DeltaTime (as updates to the g_DeltaTime are called at the start*/
/*of this loop.															  */
void Process(void) {
	g_DeltaTime = Clock::ProcessClock();

	//DO LOGIC PROCESSING HERE//
	//Use g_DeltaTime if possible
	ProcessInput();
	fb->dt += g_DeltaTime;
	p->Process(g_DeltaTime);
	ps->Process(g_DeltaTime);
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
	animatedModel->move(0.0f);


	if (InputManager::KeyArray['w'] == KEY_HELD) {
		animatedModel->setRotation(glm::vec3(0.0f, -180.0f, 0.0f));
		if (animatedModel->bMoving == false) {
			animatedModel->bMoving = true;
			animatedModel->setCurrentAnimation(31, 50);
		}
		p->ObjPos.z -= 0.8f * g_DeltaTime;
	}
	else if (InputManager::KeyArray['s'] == KEY_HELD) {
		animatedModel->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		if (animatedModel->bMoving == false) {
			animatedModel->bMoving = true;
			animatedModel->setCurrentAnimation(31, 50);
		}
		p->ObjPos.z += 0.8f * g_DeltaTime;
	}
	else if (InputManager::KeyArray['a'] == KEY_HELD) {
		animatedModel->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
		if (animatedModel->bMoving == false) {
			animatedModel->bMoving = true;
			animatedModel->setCurrentAnimation(31, 50);
		}
		p->ObjPos.x -= 0.8f * g_DeltaTime;
	}
	else if (InputManager::KeyArray['d'] == KEY_HELD) {
		animatedModel->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
		if (animatedModel->bMoving == false) {
			animatedModel->bMoving = true;
			animatedModel->setCurrentAnimation(31, 50);
		}
		p->ObjPos.x += 0.8f * g_DeltaTime;
	}
	else if (InputManager::KeyArray[32] == KEY_FIRST_PRESS) {
		p->Jump();
		if (animatedModel->bMoving == false) {
			animatedModel->bMoving = true;
			animatedModel->setCurrentAnimation(71, 80);
		}

	}
	else {
		animatedModel->rotate(0.0f);
		if (animatedModel->bMoving == true) {
			animatedModel->bMoving = false;
			animatedModel->setCurrentAnimation(0, 30);
		}
		
	}

	if (InputManager::KeyArray['p'] == KEY_FIRST_PRESS) {
		fb->CycleDisplayMode();
	}
	if (InputManager::KeyArray['g'] == KEY_FIRST_PRESS) {
		t->ToggleGrass();
	}



	if (p->ObjPos.x > 2.52f) {
		p->ObjPos.x = 2.52f;
	}
	if (p->ObjPos.x < -2.52f) {
		p->ObjPos.x = -2.52f;
	}
	if (p->ObjPos.z > 2.52f) {
		p->ObjPos.z = 2.52f;
	}
	if (p->ObjPos.z < -2.52f) {
		p->ObjPos.z = -2.52f;
	}
	
	if (p->ObjPos.y < t->GetHeight(p->ObjPos.x, p->ObjPos.z)) {
		p->ObjPos.y = (t->GetHeight(p->ObjPos.x, p->ObjPos.z));
	}
	animatedModel->setPosition(p->ObjPos);
	Camera::GetPos() = glm::vec3(p->ObjPos.x, p->ObjPos.y + 0.5f, p->ObjPos.z + 0.5f);
	InputManager::ProcessKeyInput();
}

