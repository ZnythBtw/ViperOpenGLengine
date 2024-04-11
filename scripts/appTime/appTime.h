#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef const unsigned short VPETickRate;

struct VPEAppTimeManip
{
	double zeroTime = 0.0,
		DeltaTime = 0.0,
		timeCounter = 0.0, 
		timeCounterLimit = 0.1,
		fps = 0.0;
	
	float TickRateFactor = 0.0f;

	double& GetDeltaTime();
	const float& GetTickRateFactor(VPETickRate& TickRate);
	void UpdateFPSWindowTitle(GLFWwindow* window);
};