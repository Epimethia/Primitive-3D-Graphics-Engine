
#include "Resource.h"
#include "Camera.h"
#include "Clock.h"
#include "InputManager.h"
#include "Terrain.h"
#include "FrameBuffer.h"
#include "Text.h"
#include "ParticleSystem.h"
#include "Cloth.h"
#include "Pawn.h"
#include "ClothParticle.h"

float g_DeltaTime = 0.0f;

Text* tx0;
Text* tx1;
Text* tx2;
FrameBuffer* fb = new FrameBuffer;
std::shared_ptr<Cloth> cl;
std::shared_ptr<Pawn> po;
std::shared_ptr<ClothParticle> HeldParticle;
float fanSpeed = 0.0f;
float DistToParticle;

int clothWidth = 30;
int clothHeight = 30;

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
	Camera::GetInstance()->GetPos() = glm::vec3(0.0f, 0.0f, 0.0f);
	InputManager::Init();
	
	//the cloth
	cl = std::make_shared<Cloth>();
	cl->Init();

	//the sphere
	po = std::make_shared<Pawn>();
	po->Init(glm::vec3(0.0f, 0.0f, -220.0f));



	tx0 = new Text("WASD to move camera, hold LMB to look around. Spacebar/Left shift to ascend/descend", ARIAL, glm::vec2(30.0f, 850.0f), 0.3f);
	tx1 = new Text("U/I to move the fan up and down. O/P to increase/decrease fan speed.", ARIAL, glm::vec2(30.0f, 830.0f), 0.3f);
	tx2 = new Text("V/B to reduce/increase cloth width.", ARIAL, glm::vec2(30.0f, 810.0f), 0.3f);
	fb->Init();
}


/*Where rendering occurs. You should have one single object that has a	  */
/*Render() function that calls all your object's Render functions. There  */
/*should really only be a single render call in here.					  */
void Render(void) {

	//RENDER ITEMS HERE//
	fb->BeginCapture();
	tx0->Render();
	tx1->Render();
	tx2->Render();
	cl->Render();
	po->Render();
	fb->Render();
	//-----------------//

 	glutSwapBuffers();
}


/*This is where the processing happens. Again, should have a single object*/
/*that calls all other objects render functions. This is where you also   */
/*pass in DeltaTime (as updates to the g_DeltaTime are called at the start*/
/*of this loop.															  */
void Process(void) {
	g_DeltaTime = Clock::GetInstance()->ProcessClock();

	//DO LOGIC PROCESSING HERE//
	//Use g_DeltaTime if possible
	ProcessInput();
	fb->dt += g_DeltaTime;
	Camera::Process();
	cl->Update(g_DeltaTime);
	po->Process(g_DeltaTime);
	cl->ApplyForce(po->ObjPos, glm::vec3(0.0f, 0.0f, 10.0f) * fanSpeed, 20.0f);

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
		GLUT_DEPTH  | 
		GLUT_DOUBLE | 
		GLUT_RGBA   | 
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
	float speed = 30.0f;

	auto CameraFront = glm::normalize(Camera::GetFront());
	auto CameraRight = glm::normalize(Camera::GetRight());
	auto CameraUp = glm::normalize(Camera::GetUp());
	if (InputManager::KeyArray['w'] == KEY_HELD) {
		Camera::GetPos() -= (CameraFront * speed * g_DeltaTime);
	}
	if (InputManager::KeyArray['s'] == KEY_HELD) {
		Camera::GetPos() += (CameraFront * speed * g_DeltaTime);
	}
	if (InputManager::KeyArray['a'] == KEY_HELD) {
		Camera::GetPos() -= (CameraRight * speed * g_DeltaTime);
	}
	if (InputManager::KeyArray['d'] == KEY_HELD) {
		Camera::GetPos() += (CameraRight * speed * g_DeltaTime);
	}
	if (InputManager::KeyArray[32] == KEY_HELD) {
		Camera::GetPos().y += speed * g_DeltaTime;
	}
	if (InputManager::KeySpecialArray[GLUT_KEY_SHIFT_L] == KEY_HELD){
		Camera::GetPos().y -= speed * g_DeltaTime;
	}
	if (InputManager::KeyArray['u'] == KEY_HELD) {
		po->ObjPos -= glm::vec3(0.0f, 30.0f, 0.0f) * g_DeltaTime;
	}
	if (InputManager::KeyArray['i'] == KEY_HELD) {
		po->ObjPos += glm::vec3(0.0f, 30.0f, 0.0f) * g_DeltaTime;
	}
	if (InputManager::KeyArray['o'] == KEY_FIRST_PRESS) {
		fanSpeed < 0 ? fanSpeed -= 0.5f : fanSpeed = 0.0f;
	}
	if (InputManager::KeyArray['p'] == KEY_FIRST_PRESS) {
		fanSpeed += 0.5f;
	}
	if (InputManager::KeyArray['r'] == KEY_FIRST_PRESS) {
		cl.reset();
		cl = std::make_shared<Cloth>(clothWidth);
		cl->Init();
	}

	if (InputManager::KeyArray['v'] == KEY_FIRST_PRESS) {
		clothWidth > 4 ? clothWidth-- : clothWidth = 4;
		cl.reset();
		cl = std::make_shared<Cloth>(clothWidth);
		cl->Init();
	}
	if (InputManager::KeyArray['b'] == KEY_FIRST_PRESS) {
		clothWidth < 35 ? clothWidth++ : clothWidth = 35;
		cl.reset();
		cl = std::make_shared<Cloth>(clothWidth);
		cl->Init();
	}

	if (InputManager::MouseButtonArray[0] == KEY_HELD){
		glm::vec2 dis = InputManager::v2MouseLastPos - InputManager::v2MouseCurrentPos;
		float xDif = (dis.x / UTILS::WindowWidth) * 100.0f * Clock::GetDeltaTime();
		float yDif = (-dis.y / UTILS::WindowHeight) * 100.0f * Clock::GetDeltaTime();
		Camera::LookDir = glm::vec3(xDif, yDif, 0.0f);
		InputManager::v2MouseLastPos = InputManager::v2MouseCurrentPos;
	}

	InputManager::ProcessInputs();
}

