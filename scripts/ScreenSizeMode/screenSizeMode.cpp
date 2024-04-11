#include "screenSizeMode.h"

const void VPEScreenSizeModeManip::SetOnOffFullscreen(GLFWwindow* window, GLFWmonitor* monitor, const GLFWvidmode* mode, const VPEResolution& WindowResolution, const int& key)
{
	if (glfwGetKey(window, key) == GLFW_PRESS && this->keyReleased == true)
	{
		if (!this->isFullscreen)
		{
			glfwGetWindowPos(window, &this->WindowPos.x, &this->WindowPos.y);
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			this->isFullscreen = true;
		}
		else
		{
			glfwSetWindowMonitor(window, nullptr, this->WindowPos.x, this->WindowPos.y, WindowResolution.x, WindowResolution.y, GLFW_DONT_CARE);
			this->isFullscreen = false;
		}
		this->keyReleased = false;
	}
	else if (glfwGetKey(window, key) == GLFW_RELEASE)
	{
		this->keyReleased = true;
	}
	return;
}