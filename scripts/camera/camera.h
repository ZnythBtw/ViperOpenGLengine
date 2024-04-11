#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class VPECamera__
{
private:
	glm::vec3 Up{ 0.0f, 1.0f, 0.0f };
public:
	float TickRateFactor{ 0.0f };
	float MouseSens{ 150.0f };
	float Speed{ 10.0f };

	float Near{ 0.0f };
	float Far{ 0.0f };
	float FieldOfView{ 0.0f };

	glm::vec3 Location{ 0.0f, 3.2f, 0.0f };
	glm::vec3 Direction{ 0.0f, 0.0f, -1.0f };

	void InitCamera(const float& Near, const float& Far, const float& Fov);

	void Look(float Yaw, float Pitch);
	
	void GoForward(const float& amount);
	
	void GoRight(const float& amount);

	glm::mat4 GetView() const;
	
	glm::mat4 GetPerspective(const float& AspectRatio) const;
	
	glm::mat4 GetStaticView(const float& AspectRatio, const glm::vec3& P) const;
	
	glm::mat4 GetNormalMatrix(const glm::mat4& M) const;
	
	glm::mat4 GetViewProjection(const float& AspectRatio) const;
};

typedef VPECamera__* VPECamera;

inline VPECamera VPECreateNewCamera()
{
	return new VPECamera__;
}
inline void VPEDeleteCamera(VPECamera Camera)
{
	delete Camera;
	return;
}