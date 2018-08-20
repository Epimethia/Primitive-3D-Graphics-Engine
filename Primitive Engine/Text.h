#pragma once
#include "Resource.h"
#include "ShaderLoader.h"

struct Character {
	GLuint        TextureID;	// Texture ID 
	glm::ivec2    Size;	// Size of glyph
	glm::ivec2    Bearing;	// Positon of glyph 
	GLuint        Advance;	// How far to move for the next character
};

class Text {
public:
	Text();
	Text(std::string text, std::string font, glm::vec2 pos, GLuint Shader, int size);
	void SetColor(glm::vec3 _Color) { color = _Color; };
	void SetScale(GLfloat _Scale) { scale = _Scale; };
	void SetText(std::string _Text) { text = _Text; };
	glm::vec2 GetPos() { return position; };

	~Text();

	void Render();
	void SetPosition(glm::vec2 newPosition) { position = newPosition; };

private:
	std::string text;
	GLfloat scale;
	glm::vec3 color;
	glm::vec2 position;

	GLuint VAO, VBO, TextShaderProg;
	std::map<GLchar, Character> CharMap;
};