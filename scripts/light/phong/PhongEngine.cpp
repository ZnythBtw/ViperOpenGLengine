#include "PhongEngine.h"

const void PhongModel::InitLight(const GLuint& LoadedShader)
{
	try
	{
		if (LoadedShader == NULL)
		{
			throw std::runtime_error("O shader passado para o iniciador do modelo de phong nao foi carregado corretamente ou esta corrompido");
		}
	}
	catch (std::invalid_argument& out)
	{
		std::cerr << out.what() << std::endl;
		exit(-1);
	}
	this->Shader = LoadedShader;
	return;
}
const void PhongModel::DynamicLight(const float LightIntensity, const float Alpha, const glm::vec3& LightDirection, const glm::mat4 NormalMatrix) const
{
	glUseProgram(this->Shader);
	glUniformMatrix4fv(glGetUniformLocation(this->Shader, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));
	glUniform3fv(glGetUniformLocation(this->Shader, "LightDirection"), 1, glm::value_ptr(LightDirection));
	glUniform1f(glGetUniformLocation(this->Shader, "LightIntensity"), LightIntensity);
	glUniform1f(glGetUniformLocation(this->Shader, "Alpha"), Alpha);
	return;
}