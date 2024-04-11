#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define Id glm::identity<glm::mat4>()

class PhongModel
{
public:
	GLuint Shader;
	const void InitLight(const GLuint& LoadedShader);
	const void DynamicLight(const float LightIntensity, const float Alpha = 25.0f, const glm::vec3& LightDirection = glm::vec3{ 0.0f, 0.0f, -1.0f }, const glm::mat4 NormalMatrix = Id) const;
};