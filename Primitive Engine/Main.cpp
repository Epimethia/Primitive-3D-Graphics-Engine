
#include "Resource.h"
#include "Camera.h"
#include "Clock.h"
#include "InputManager.h"

float g_DeltaTime = 0.0f;

void Initialize() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(static_cast<int>(UTILS::WindowWidth), static_cast<int>(UTILS::WindowHeight));
	glutCreateWindow("Piroots of the CurryBeans");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glewInit();

}

void Render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//RENDER ITEMS HERE


	//-----------------

	glutSwapBuffers();
}

void Process(void) {
	g_DeltaTime = Clock::GetDeltaTime();

	//DO LOGIC PROCESSING HERE
	//Use g_DeltaTime if possible



	//------------------------

	glutPostRedisplay();
}

void Exit(void) {
	//ANY EXIT FUNCTIONS THAT NEED TO BE RUN GO HERE
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	Initialize();
	glutDisplayFunc(Render);
	glutIdleFunc(Process);
	glutIgnoreKeyRepeat(1);
	glutCloseFunc(Exit);
	glutMainLoop();
	return 0;
}