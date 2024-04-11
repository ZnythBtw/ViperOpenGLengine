#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

void VPEShowPcInfo();
void VPEInit_And_CheckGLFW();
void VPEInit_And_CheckGLEW();

template<typename Type> constexpr Type VPEGetNull();
template<> constexpr glm::vec1 VPEGetNull()
{
	return glm::vec1{ 0.0f };
}

template<> constexpr glm::vec2 VPEGetNull()
{
	return glm::vec2{ 0.0f, 0.0f };
}

template<> constexpr glm::vec3 VPEGetNull()
{
	return glm::vec3{ 0.0f, 0.0f, 0.0f };
}

template<> constexpr glm::vec4 VPEGetNull()
{
	return glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
}

template<> constexpr glm::ivec1 VPEGetNull()
{
	return glm::ivec1{ 0 };
}

template<> constexpr glm::ivec2 VPEGetNull()
{
	return glm::ivec2{ 0, 0 };
}

template<> constexpr glm::ivec3 VPEGetNull()
{
	return glm::ivec3{ 0, 0, 0 };
}

template<> constexpr glm::ivec4 VPEGetNull()
{
	return glm::ivec4{ 0, 0, 0, 0 };
}