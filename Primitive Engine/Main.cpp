
#include "Resource.h"
#include "Camera.h"
#include "Clock.h"
#include "InputManager.h"
#include "Environment.h"

float g_DeltaTime = 0.0f;

Environment env;


void Initialize() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glDisable(GL_MULTISAMPLE);
	env.Init();
}

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

void Process(void) {
	g_DeltaTime = Clock::GetDeltaTime();

	//DO LOGIC PROCESSING HERE
	//Use g_DeltaTime if possible

	env.Process(g_DeltaTime);

	//------------------------

	glutPostRedisplay();
}

void Exit(void) {
	//ANY EXIT FUNCTIONS THAT NEED TO BE RUN GO HERE
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(static_cast<int>(UTILS::WindowWidth), static_cast<int>(UTILS::WindowHeight));
	glutCreateWindow("Piroots of the CurryBeans");
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glewInit();
	Initialize();
	glutDisplayFunc(Render);
	glutIdleFunc(Process);
	//glutIgnoreKeyRepeat(1);
	glutCloseFunc(Exit);
	glutMainLoop();
	return 0;
}