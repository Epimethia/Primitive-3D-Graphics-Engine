#include "FrameBuffer.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "EntityManager.h"
FrameBuffer::FrameBuffer(){
	dt = 0.0f;
	CurrentDisplayMode = 0;
}

FrameBuffer::~FrameBuffer(){

}

void FrameBuffer::Init(){
	
	GLfloat BufferVerts[] = {
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
	};

	//Defining Cube Indices
	GLuint BufferIndices[] = {
		0, 1, 2, 0, 2, 3		// Front Face
	};
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &quadEBO);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BufferVerts), &BufferVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BufferIndices), BufferIndices, GL_STATIC_DRAW);

	//Enabling the positional floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
		(GLvoid*) 0);
	glEnableVertexAttribArray(0);

	//Enabling Normal Floats
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
		(GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	Shader = ShaderLoader::CreateProgram("Assets/Shaders/FrameBuffer.vs", "Assets/Shaders/FrameBuffer.fs");
	
	

	//Intializing the framebuffer object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexImage2D(GL_TEXTURE_2D,
		0, GL_RGB,
		UTILS::WindowWidth, UTILS::WindowHeight,
		0, //border
		GL_RGB, //format
		GL_UNSIGNED_BYTE, //data type 
		NULL
	);

	//Setting texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Attach texture to framebuffer object
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,			//target buffer
		GL_COLOR_ATTACHMENT0,	//attachment, could be GL_DEPTH_ATTACHMENT or GL_STENCIL_ATTACHMENT
		GL_TEXTURE_2D,			//texture target type
		Texture,				//texture
		0);						//level

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	glRenderbufferStorage(
		GL_RENDERBUFFER,		// must be
		GL_DEPTH24_STENCIL8,	//use as depth - stencil buffer
		UTILS::WindowWidth,		//viewport width and height;
		UTILS::WindowHeight
	);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	

	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,					//target
		GL_DEPTH_STENCIL_ATTACHMENT,	//attachment
		GL_RENDERBUFFER,				//renderbufferTarget
		RBO								//render buffer
	);	

	(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		? std::cout << "FrameBuffer Intialized" << std::endl
		: std::cout << "ERROR:: FrameBuffer Not Intialized" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BeginCapture(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
}

void FrameBuffer::Render(){
	glUseProgram(Shader);

	//Binding the array
	glBindVertexArray(quadVAO);

	//Setting and binding the correct texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Sending the texture to the GPU via uniform
	glUniform1i(glGetUniformLocation(Shader, "tex"), 0);
	glUniform1f(glGetUniformLocation(Shader, "dTime"), dt);
	glUniform1i(glGetUniformLocation(Shader, "DisplayMode"), CurrentDisplayMode);

	//Setting back face culling
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glEnable(GL_CULL_FACE);

	//Clearing the vertex array
	glBindVertexArray(0);
}

void FrameBuffer::CycleDisplayMode(){
	(CurrentDisplayMode >= 2) 
		? CurrentDisplayMode = 0 
		: CurrentDisplayMode++;
}
