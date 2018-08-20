#include "Camera.h"
std::shared_ptr<Camera> Camera::CameraPtr;
glm::mat4 Camera::VPMat;
glm::mat4 Camera::View;
glm::mat4 Camera::Projection;
glm::vec3 Camera::CameraPos = { 0.0f, 0.0f, 0.0f };
static int XRot;
static int YRot;

Camera::Camera() {
	View = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);/*glm::ortho(0.0f, 900.0f, 900.0f, 0.0f, 0.1f, 1.0f);*/
	VPMat = Projection * View;
}

Camera::~Camera() {
	CameraPtr = nullptr;
}

std::shared_ptr<Camera> Camera::GetInstance() {
	if (CameraPtr == nullptr) CameraPtr = std::shared_ptr<Camera>(new Camera);
	std::cout << "Camera Created\n";
	return CameraPtr;
}

void Camera::DestroyInstance() {
	CameraPtr = nullptr;
	std::cout << "Camera Destroyed\n";
}

glm::mat4& Camera::GetMatrix() {
	View = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	Projection = glm::perspective(1.0f, 1.0f, 0.1f, 20000.0f);
	glm::mat4 RotationMatrixX =
		glm::rotate(
		glm::mat4(),
		glm::radians(45.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotationMatrixY =
		glm::rotate(
			glm::mat4(),
			glm::radians(-30.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

	glm::mat4 TranslationMatrix =
		glm::translate(
		glm::mat4(),
		CameraPos
		);

	VPMat = Projection * View * ((RotationMatrixX) * TranslationMatrix);
	return VPMat;
}
