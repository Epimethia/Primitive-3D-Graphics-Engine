#include "Terrain.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Entity.h"

Terrain::Terrain(){

}

Terrain::~Terrain(){}



void Terrain::Init(){
	p = Plane(glm::vec3(0.0f, -0.0f, -50.0f), "Assets/Sprite/RiverBedTexture.png");
	p.GetScale() = glm::vec3(20.0f, 20.0f, 20.0f);

}


void Terrain::Render(){
	p.Process(0);
}