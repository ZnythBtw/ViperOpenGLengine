#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class VPENoise
{
private:
	float MainLoopFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall) const;
public:
	//default loop functions
	float MainLoopSinFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopCosFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopTanFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;

	//inverse loop functions
	float MainLoopSecFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopCscFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;

	//hiperbolic loop functions
	float MainLoopSinHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopCosHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopTanHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;

	//arc loop functions
	float MainLoopASinFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopACosFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopATanFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;

	//hiperbolic arc functions
	float MainLoopASinHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopACosHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
	float MainLoopATanHFunction(const float& MaxAngle, const float& MinAngle, const float& Speed, const bool& EndCall = false) const;
};