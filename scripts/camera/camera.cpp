#include "camera.h"
#define Id glm::identity<glm::mat4>()
#define VPE_FIX_Z_ROTATION glm::vec3{0.0f, 1.0f, 0.0f}

void VPECamera__::InitCamera(const float& Near, const float& Far, const float& Fov)
{
	this->Far = Far;
	this->Near = Near;
	this->FieldOfView = glm::radians(Fov);

	return;
}
void VPECamera__::Look(float Yaw, float Pitch)
{
	//Yaw -> Rotate X
	//Pitch -> Rotate Y
	
	Yaw *= this->MouseSens * this->TickRateFactor;
	Pitch *= this->MouseSens * this->TickRateFactor;

	//fix camera bug where it rotates uncontrollably
	if ((this->Direction.y < -0.99f && Pitch < 0.0f) || (this->Direction.y > 0.99f && Pitch > 0.0f))
	{
		Pitch = 0.0f;
	}

	const glm::vec3 Right{ glm::normalize(glm::cross(this->Direction, this->Up)) };
	
	const glm::mat4 
		YawRotation = glm::rotate(Id, glm::radians(Yaw), this->Up),
		PitchRotation = glm::rotate(Id, glm::radians(Pitch), Right);

	this->Up = static_cast<glm::vec3>(PitchRotation * glm::vec4(VPE_FIX_Z_ROTATION, 0.0f));
	this->Direction = static_cast<glm::vec3>(YawRotation * PitchRotation * glm::vec4(this->Direction, 0.0f));

	return;
}
void VPECamera__::GoForward(const float& amount)
{
	const glm::vec3 P = this->Direction * amount * this->Speed;
	this->Location += glm::vec3{ P.x, 0.0f, P.z };

	return;
}
void VPECamera__::GoRight(const float& amount)
{
	const glm::vec3 Right = glm::normalize(glm::cross(this->Direction, VPE_FIX_Z_ROTATION)) * amount * this->Speed;
	this->Location += glm::vec3{ Right.x, 0.0f, Right.z };
	
	return;
}
glm::mat4 VPECamera__::GetView() const
{
	return glm::lookAt(this->Location, this->Location + this->Direction, this->Up);
}
glm::mat4 VPECamera__::GetPerspective(const float& AspectRatio) const
{
	return glm::perspective(this->FieldOfView, AspectRatio, this->Near, this->Far);
}
glm::mat4 VPECamera__::GetStaticView(const float& AspectRatio, const glm::vec3& P) const
{
	return GetPerspective(AspectRatio) * glm::translate(Id, P);
}
glm::mat4 VPECamera__::GetNormalMatrix(const glm::mat4& M) const
{
	return glm::inverse(glm::transpose(GetView() * M));
}
glm::mat4 VPECamera__::GetViewProjection(const float& AspectRatio) const
{
	return GetPerspective(AspectRatio) * GetView();
}