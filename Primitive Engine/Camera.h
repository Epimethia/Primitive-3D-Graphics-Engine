#pragma once
#include "Resource.h"

/*A camera class for the scene. Really does nothing more than*/
/*just update the global MVP matrix.						 */
class Camera {
public:
	~Camera();
	static std::shared_ptr<Camera> GetInstance();
	static void DestroyInstance();
	static glm::mat4 GetVPMatrix();
	static glm::vec3& GetPos() { return CameraPos; };
	static glm::vec3 GetFront() { return Front; };
	static glm::vec3 GetUp() { return Up; };
	static glm::vec3 GetRight() { return Right; };
	static glm::mat4 GetView() { return View; };
	static void Process();
	static int XRot;
	static int YRot;

private:
	Camera();
	static std::shared_ptr<Camera> CameraPtr;
	static glm::mat4 View, Projection;
	static glm::mat4 VPMat;
	static glm::vec3 CameraPos;
	static glm::vec3 Front, Up, Right;
};