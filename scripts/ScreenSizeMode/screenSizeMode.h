#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

typedef glm::vec2 VPEResolution;

class VPEScreenSizeModeManip
{
private:
	bool keyReleased = false, isFullscreen = false;
	glm::ivec2 WindowPos = { 0, 0 };
public:
	const void SetOnOffFullscreen(GLFWwindow* window, GLFWmonitor* monitor, const GLFWvidmode* mode, const VPEResolution& WindowResolution, const int& key);
};