
#include "Resource.h"
#include "Camera.h"
#include "Clock.h"
#include "InputManager.h"
#include "Environment.h"

float g_DeltaTime = 0.0f;

Environment env;

/*Initializing the entire program. This function gets called in glutInit()*/
/*This turns on backface culling and the depth test (useful for some other*/
/*things). Does not return anything. Should always complete successfully. */
void Initialize() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glDisable(GL_MULTISAMPLE);
	env.Init();
}

/*Where rendering occurs. You should have one single object that has a	  */
/*Render() function that calls all your object's Render functions. There  */
/*should really only be a single render call in here.					  */
void Render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Scissor Test
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 100, 750, 550);

	//RENDER ITEMS HERE
	env.Render(g_DeltaTime);

	//-----------------
	glDisable(GL_SCISSOR_TEST);
	glutSwapBuffers();
}

/*This is where the processing happens. Again, should have a single object*/
/*that calls all other objects render functions. This is where you also   */
/*pass in DeltaTime (as updates to the g_DeltaTime are called at the start*/
/*of this loop.															  */
void Process(void) {
	g_DeltaTime = Clock::GetDeltaTime();

	//DO LOGIC PROCESSING HERE
	//Use g_DeltaTime if possible

	env.Process(g_DeltaTime);

	//------------------------

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
	glClearColor(0.5, 0.5, 0.5, 1.0);

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