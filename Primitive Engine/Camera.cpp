#include "Camera.h"
std::shared_ptr<Camera> Camera::CameraPtr;
glm::mat4 Camera::VPMat;
glm::mat4 Camera::View;
glm::mat4 Camera::Projection;
glm::vec3 Camera::CameraPos = { 0.0f, 0.0f, 3.0f };
glm::vec3 Camera::Up = { 0.0f, 0.0f, 0.0f };
glm::vec3 Camera::Front = { 0.0f, 0.0f, 0.0f };
glm::vec3 Camera::Right = { 0.0f, 0.0f, 0.0f };
glm::vec3 Camera::Target = {0.0f, 0.0f, 0.0f};
glm::vec3 Camera::LookDir = { 0.0f, 0.0f, 0.0f };
static int XRot;
static int YRot;

Camera::Camera() {

}

Camera::~Camera() {
	CameraPtr = nullptr;
}

std::shared_ptr<Camera> Camera::GetInstance() {
	if (CameraPtr == nullptr) CameraPtr = std::shared_ptr<Camera>(new Camera);
	std::cout << "Camera Created\n";
	
	Front = glm::normalize(CameraPos - (Target + LookDir));
	glm::vec3 tempUp = { 0.0f, 1.0f, 0.0f };
	glm::vec3 Right = glm::normalize(glm::cross(tempUp, Front));
	Up = glm::cross(Front, Right);

	View = glm::lookAt(CameraPos, CameraPos - Front, Up);
	Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 20000.0f);
	VPMat = Projection * View;

	return CameraPtr;
}

void Camera::DestroyInstance() {
	CameraPtr = nullptr;
	std::cout << "Camera Destroyed\n";
}

glm::mat4 Camera::GetVPMatrix() {
	return VPMat;
}

void Camera::Process(){
	Target = CameraPos - Front;
	glm::vec3 tempUp = { 0.0f, 1.0f, 0.0f };

	Front = glm::normalize(CameraPos - (Target + LookDir));
	Right = glm::normalize(glm::cross(tempUp, Front));
	Up = glm::normalize(glm::cross(Front, Right));

	View = glm::lookAt(CameraPos, CameraPos - Front, Up);
	Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 20000.0f);
	VPMat = Projection * View;
	LookDir = glm::vec3();
}
