#version 430 core

in vec2 fragTexCoord;
out vec4 color;

uniform sampler2D tex;

void main(){

	vec4 texColor = texture(tex, fragTexCoord);
	if (texColor.a < 0.1f) discard;
	color = texColor;
	
}