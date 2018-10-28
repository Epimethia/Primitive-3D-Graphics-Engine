#include "Camera.h"
std::shared_ptr<Camera> Camera::CameraPtr;
glm::mat4 Camera::VPMat;
glm::mat4 Camera::View;
glm::mat4 Camera::Projection;
glm::vec3 Camera::CameraPos = { 0.0f, 0.0f, -5.0f };
static int XRot;
static int YRot;

Camera::Camera() {
	View = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 20000.0f);/*glm::ortho(0.0f, 900.0f, 900.0f, 0.0f, 0.1f, 1.0f);*/
	VPMat = Projection * View;
}

Camera::~Camera() {
	CameraPtr = nullptr;
}

std::shared_ptr<Camera> Camera::GetInstance() {
	if (CameraPtr == nullptr) CameraPtr = std::shared_ptr<Camera>(new Camera);
	std::cout << "Camera Created\n";
	View = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 20000.0f);
	glm::mat4 RotationMatrixX =
		glm::rotate(
			glm::mat4(),
			glm::radians(-35.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotationMatrixY =
		glm::rotate(
			glm::mat4(),
			glm::radians(0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

	glm::mat4 TranslationMatrix =
		glm::translate(
			glm::mat4(),
			CameraPos
		);

	VPMat = Projection * View * ((RotationMatrixX) * TranslationMatrix);
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
	View = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Projection = glm::perspective(45.0f, 16.0f/9.0f, 0.1f, 20000.0f);
	glm::mat4 RotationMatrixX =
		glm::rotate(
			glm::mat4(),
			glm::radians(-65.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotationMatrixY =
		glm::rotate(
			glm::mat4(),
			glm::radians(0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

	glm::mat4 TranslationMatrix =
		glm::translate(
			glm::mat4(),
			CameraPos
		);

	VPMat = Projection * View * ((RotationMatrixX) * TranslationMatrix);
}
