#include "utils.h"

void VPEShowPcInfo()
{
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Sua placa de video: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Versão do OpenGL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Versão do Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

	return;
}
void VPEInit_And_CheckGLFW()
{
	try
	{
		if (glfwInit() != GLFW_TRUE)
		{
			throw std::runtime_error("Erro ao iniciar a API glfw.");
		}
	}
	catch (std::runtime_error& out)
	{
		std::cerr << out.what() << std::endl;
		exit(-1);
	}
	return;
}
void VPEInit_And_CheckGLEW()
{
	try
	{
		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error("Erro ao iniciar a API glew");
		}
	}
	catch (std::runtime_error& out)
	{
		std::cerr << out.what() << std::endl;
		exit(-1);
	}
	return;
}