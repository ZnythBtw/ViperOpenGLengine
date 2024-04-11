#include "VPEnoise.h"

float VPENoise::MainLoopFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	static float* Vf_ = new float(0.0f);
	static float* i_ = new float(MinAngle);

	if (*i_ == MinAngle)
	{
		*Vf_ -= Speed;
		if (*Vf_ < *i_)
		{
			*Vf_ = *i_;
			*i_ = MaxAngle;
		}
	}
	else if (*i_ == MaxAngle)
	{
		*Vf_ += Speed;
		if (*Vf_ > *i_)
		{
			*Vf_ = *i_;
			*i_ = MinAngle;
		}
	}

	//End the function
	if (EndCall)
	{
		const float vf = *Vf_;

		delete Vf_;
		delete i_;

		return vf;
	}
}

float VPENoise::MainLoopSinFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::sin(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopCosFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::cos(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopTanFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::tan(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}

//inverse loop functions
float VPENoise::MainLoopSecFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::sec(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopCscFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::csc(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}

//hiperbolic loop functions
float VPENoise::MainLoopSinHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::sinh(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopCosHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::cosh(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopTanHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::tanh(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}

//arc loop functions
float VPENoise::MainLoopASinFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::asin(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopACosFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::acos(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopATanFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::atan(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}

//hiperbolic arc functions
float VPENoise::MainLoopASinHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::asinh(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopACosHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::acosh(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}
float VPENoise::MainLoopATanHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const
{
	return glm::atanh(this->MainLoopFunction(MaxAngle, MinAngle, Speed, EndCall));
}