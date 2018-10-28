#pragma once
#include "Resource.h"
class FrameBuffer{
public:
	FrameBuffer();
	~FrameBuffer();
	void Init();
	void BeginCapture();
	void Render();
	void CycleDisplayMode();
	float dt;
	int CurrentDisplayMode;
private:

	GLuint FBO, RBO;
	GLuint Texture;
	GLuint Shader;
	GLuint quadVAO, quadVBO, quadEBO;

};