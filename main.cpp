#include <iostream>
#include <ctime>
#include <array>
#include <tchar.h>
#include <fstream>
#include <vector>
#include <span>
#include <optional>
#include <Windows.h>
#include <algorithm>
#include <filesystem>
#include <irrKlang.h>
#include <ik_irrKlangTypes.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb/stb_image.h"
#include "externals.h"
#include "scripts/camera/camera.h"
#include "scripts/light/phong/PhongEngine.h"
#include "scripts/configs/configs.h"
#include "scripts/utils/utils.h"
#include "scripts/ScreenSizeMode/screenSizeMode.h"
#include "scripts/appTime/appTime.h"
#include "scripts/noise/VPEnoise.h"
#include "scripts/GLMVecToString/GLMVecToString.h"
#include "scripts/vec/VPEvec.h"
#include "scripts/getNull/getNull.h"

template<typename T> T VPEMin(const std::span<T>& Stack)
{
	T _min = 0.0f;
	for (VPEuLongLong i = 1; i < Stack.size(); i++)
	{
		if (Stack[i] < Stack[i - 1])
		{
			_min = Stack[i];
		}
	}
	return _min;
}
glm::vec3 VPEMinX(const std::span<glm::vec3>& Stack)
{
	glm::vec3 _min = Stack.front();
	for (const glm::vec3 i : Stack)
	{
		if (i.x < _min.x)
		{
			_min = i;
		}
	}
	return _min;
}
glm::vec3 VPEMinY(const std::span<glm::vec3>& Stack)
{
	glm::vec3 _min = Stack.front();
	for (const glm::vec3 i : Stack)
	{
		if (i.y < _min.y)
		{
			_min = i;
		}
	}
	return _min;
}
glm::vec3 VPEMinZ(const std::span<glm::vec3>& Stack)
{
	glm::vec3 _min = Stack.front();
	for (const glm::vec3 i : Stack)
	{
		if (i.z < _min.z)
		{
			_min = i;
		}
	}
	return _min;
}

namespace WindowConfig
{
	VPEint Width = 800;
	VPEint Height = 600;

	VPEfloat AspectRatio = static_cast<float>(Width) / Height;
}
namespace VPECallbacks
{
	double MouseX;
	double MouseY;
	glm::ivec2 WindowNewSize;
	glm::vec2 zeroCursorPosition;
	int Button;
	int Action;
	int Modifier;
	bool EnableMouseMoviment;

	void BindMouseMotionCallbacks(GLFWwindow* window, double x, double y)
	{
		MouseX = x;
		MouseY = y;

		return;
	}
	void BindMouseButtonCallbacks(GLFWwindow* window, int button, int action, int modifiers)
	{
		Button = button;
		Action = action;
		Modifier = modifiers;

		return;
	}
	void BindFrameBufferSizeCallbacks(GLFWwindow* Window, int NewWidth, int NewHeight)
	{
		WindowNewSize.x = NewWidth;
		WindowNewSize.y = NewHeight;

		return;
	}
};

struct VPE_HUD
{
	glm::ivec2 WindowPos = VPEGetNull<glm::ivec2>();
	glm::ivec2 WindowSize = VPEGetNull<glm::ivec2>();
	VPEuint VAO = VPE_NULL;
	VPEuint Shader = VPE_NULL;
	std::vector<VPEuint> Textures;
};
class VPEHUDEngine
{
private:
	static const struct VertexHUD
	{
		glm::vec3 Position = VPEGetNull<glm::vec3>();
		glm::vec2 UV = VPEGetNull<glm::vec2>();
	};
public:
	static VPE_HUD* CreateNewHud(GLFWwindow* Window)
	{
		VPE_HUD* rHud = new VPE_HUD;
		glfwGetWindowPos(Window, &rHud->WindowPos.x, &rHud->WindowPos.y);
		glfwGetWindowSize(Window, &rHud->WindowSize.x, &rHud->WindowSize.y);
		
		return rHud;
	}
	static VPEuint& GetVAO(VPECamera Camera, const glm::ivec2& WindowSize)
	{
		VPEuint VAO = VPE_NULL;
		try
		{
			if (WindowSize.x == 0 || WindowSize.y == 0)
			{
				throw std::exception("Resolucao passada como argumento para GLFWwindow invalida.");
			}
			VPEcFloat CamHudDistance = -(Camera->Near * 1.25f);
			glm::vec2 Pos = (static_cast<glm::vec2>(WindowSize) / Camera->FieldOfView) * (Camera->Near / Camera->Far);
			
			const std::array<VertexHUD, 4> geometry = {
				VertexHUD{glm::vec3{Pos, CamHudDistance}, glm::vec2{1.0f, 1.0f}},
				VertexHUD{glm::vec3{-Pos.x,  Pos.y, CamHudDistance}, glm::vec2{1.0f, 0.0f}},
				VertexHUD{glm::vec3{-Pos.x, -Pos.y, CamHudDistance}, glm::vec2{0.0f, 0.0f}},
				VertexHUD{glm::vec3{ Pos.x, -Pos.y, CamHudDistance}, glm::vec2{0.0f, 1.0f}},
			};
			constexpr const std::array<glm::ivec3, 2> index = { glm::ivec3{0, 1, 2}, glm::ivec3{2, 3, 0} };

			VPEuint VertexBuffer = VPE_NULL, ElementBuffer = VPE_NULL;
			glGenBuffers(1, &VertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(VertexHUD) * geometry.size(), geometry.data(), GL_STATIC_DRAW);

			glGenBuffers(1, &ElementBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * index.size(), index.data(), GL_STATIC_DRAW);

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexHUD), nullptr);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(VertexHUD), reinterpret_cast<void*>(offsetof(VertexHUD, UV)));
			
			glBindVertexArray(VPE_NULL);
			std::cout << "VRAM using: " << sizeof(VertexHUD) * geometry.size() << std::endl;
		}
		catch (VPE_CATCH_EXCEPTION)
		{
			VPE_TRY_CATCH_EXIT(-1);
		}
		return VAO;
	}
	static VPEDefaultFun Draw(VPE_HUD* Hud, const glm::mat4& Perspective)
	{
		glUseProgram(Hud->Shader);
		glUniformMatrix4fv(glGetUniformLocation(Hud->Shader, VPE_DEFAULT_MVP_SHADER_ID), 1, GL_FALSE, glm::value_ptr(Perspective));
		
		VPEushort counter = 0;
		for (VPEuint i : Hud->Textures)
		{
			glUniform1i(glGetUniformLocation(Hud->Shader, ("sampler" + std::to_string(counter)).c_str()), counter);
			glBindTexture(GL_TEXTURE_2D, i);
			counter++;
		}
		glBindVertexArray(Hud->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(VPE_NULL);
		return;
	}
};
class VPECallback_Aplys
{
public:
	void SetMouseButtonCallBacks(GLFWwindow* Window) const
	{
		if (VPECallbacks::Button == GLFW_MOUSE_BUTTON_LEFT && VPECallbacks::Action == GLFW_PRESS && !VPECallbacks::EnableMouseMoviment)
		{
			VPEdouble
				x = 0.0,
				y = 0.0;

			glfwGetCursorPos(Window, &x, &y);
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			VPECallbacks::zeroCursorPosition = { x, y };
			VPECallbacks::EnableMouseMoviment = true;
		}
		return;
	}
	void SetMouseMotionCallBacks(VPECamera Camera, VPEcDouble& DeltaTime) const
	{
		if (VPECallbacks::EnableMouseMoviment)
		{
			glm::vec2 finalMousePosition{ VPECallbacks::MouseX, VPECallbacks::MouseY };
			glm::vec2 DeltaMousePosition{ finalMousePosition - VPECallbacks::zeroCursorPosition };

			Camera->Look(-DeltaMousePosition.x, -DeltaMousePosition.y);

			VPECallbacks::zeroCursorPosition = finalMousePosition;
		}
		return;
	}
	void KeyboardCallbacks(VPECamera Camera, GLFWwindow* window, VPEcDouble& DeltaTime) const
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			VPECallbacks::EnableMouseMoviment = false;
		}
		if (VPECallbacks::EnableMouseMoviment == true)
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				Camera->GoForward(1.0f * DeltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				Camera->GoForward(-1.0f * DeltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				Camera->GoRight(-1.0 * DeltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				Camera->GoRight(1.0 * DeltaTime);
			}
		}
		return;
	}
};

typedef VPE_HUD* VPEHUD;
typedef VPECallback_Aplys* VPECallbackAplys;

typedef VPEcUshort VPETickRate;

VPEcFloat& GetTickRateFactor(VPETickRate& TickRate, VPEcDouble& DeltaTime);
void VPEResizeWindowFun(GLFWwindow* window, int NewWidth, int NewHeight);

VPEAppTimeManip appTime;

int main()
{
	VPECallbackAplys callbacks = new VPECallback_Aplys;
	VPEScreenSizeModeManip fc;
	VPEObjectEngine load;
	VPENoise noise;
	PhongModel light;
	VPECamera Camera = VPECreateNewCamera();

	std::setlocale(LC_ALL, "C");
	Camera->InitCamera(0.001f, 1000.0f, 45.0f);
	VPEInit_And_CheckGLFW();

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(WindowConfig::Width, WindowConfig::Height, VPE_DEFAULT_WINDOW_TITLE, nullptr, nullptr);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glfwSetMouseButtonCallback(window, VPECallbacks::BindMouseButtonCallbacks);
	glfwSetCursorPosCallback(window, VPECallbacks::BindMouseMotionCallbacks);
	glfwSetFramebufferSizeCallback(window, VPEResizeWindowFun);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(VPE_VSYNC_OFF);

	VPEInit_And_CheckGLEW();
	VPEShowPcInfo();

	VPEResizeWindowFun(window, WindowConfig::Width, WindowConfig::Height);

	glClearColor(VPE_LIGHT_BLUE_BACKGROUND);

	glLineWidth(VPE_DEFAULT_LINE_WIDTH);
	glPointSize(VPE_DEFAULT_POINT_SIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glActiveTexture(GL_TEXTURE0);
		
	//VPEHUD hud = VPEHUDEngine::CreateNewHud(window);
	//hud->VAO = VPEHUDEngine::GetVAO(Camera, hud->WindowSize);

	VPESimpleObject simpleCube = load.CreateNewObject<VPESimpleObject>();
	simpleCube->ObjectData = load.LoadSimpleObjData("mesh/SimpleCube.obj");
	simpleCube->VAO = load.GetSimpleVAO(simpleCube);
	simpleCube->Position = glm::vec3{ 0.0f, 0.0f, -6.0f };

	/*VPEAdvancedObject fooObj = load.CreateNewObject<VPEAdvancedObject>();
	fooObj->ObjectDataMap = load.LoadAdvancedObjData("mesh/axe.obj", 'o', &fooObj->Index);
	fooObj->VAOmap = load.GetAdvancedVAO(fooObj->ObjectDataMap, fooObj->Index);*/
	
	/*VPEInstancedObject i_obj = load.CreateNewObject<VPEInstancedObject>();
	i_obj->Instances.push_back(glm::translate(Id, glm::vec3{ 2.0f, 0.0, 0.0f }));
	i_obj->Instances.push_back(glm::translate(Id, glm::vec3{ 4.0f, 0.0, 0.0f }));
	i_obj->InstancedObjectData = load.LoadSimpleObjData("mesh/SimpleCube.obj");
	i_obj->VAO = load.GetSimpleInstancedVAO(i_obj);*/

	//VPEShader DefaultShader;
	//DefaultShader.LoadShader("shaders/vert.glsl", "shaders/frag.glsl");

	//VPETexture DefaultTexture;
	//DefaultTexture.LoadTexture("textures/bark_0021.jpg");


	// Framebuffer for Shadow Map
	unsigned int shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);

	// Texture for Shadow Map FBO
	unsigned int shadowMapWidth = 1024, shadowMapHeight = 1024;
	unsigned int shadowMap;
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//Prevents darkness outside the frustrum
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Needed since we don't touch the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Matrices needed for the light's perspective
	const glm::vec3 lightPos = { 0.5f, 0.5f, 0.5f };

	glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, Camera->Near, Camera->Far);
	glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjection = orthgonalProjection * lightView;

	VPEShader shadowMapShader;
	shadowMapShader.LoadShader("shaders/shadowsbeta/shadowVert.glsl", "shaders/shadowsbeta/shadowFrag.glsl");
	
	//shadowMapShader.UseShader();
		
	appTime.zeroTime = glfwGetTime();
	do
	{
		//viper engine only
		appTime.DeltaTime = appTime.GetDeltaTime();
		appTime.TickRateFactor = appTime.GetTickRateFactor(VPE_64_TICK_RATE);
		Camera->TickRateFactor = appTime.TickRateFactor;

		appTime.UpdateFPSWindowTitle(window);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		callbacks->SetMouseButtonCallBacks(window);
		callbacks->SetMouseMotionCallBacks(Camera, appTime.DeltaTime);
		callbacks->KeyboardCallbacks(Camera, window, appTime.DeltaTime);
				
		fc.SetOnOffFullscreen(window, monitor, mode, VPEResolution{ WindowConfig::Width, WindowConfig::Height }, GLFW_KEY_F11);
		
		shadowMapShader.UseShader();
		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.ShaderID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
		glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.ShaderID, "model"), 1, GL_FALSE, glm::value_ptr(Camera->GetViewProjection(WindowConfig::AspectRatio)));

		glBindVertexArray(simpleCube->VAO);
		glDrawElements(GL_TRIANGLES, simpleCube->VertexAmount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//simpleCube->Draw(shadowMapShader, Camera->GetViewProjection(WindowConfig::AspectRatio));

		//DefaultShader.UseShader();
		//DefaultTexture.UseTexture();
		//simpleCube->Draw(DefaultShader, Camera->GetViewProjection(WindowConfig::AspectRatio));

		glfwPollEvents();
		glfwSwapBuffers(window);
	} while (!glfwWindowShouldClose(window));

	load.DeleteObject<VPESimpleObject>(simpleCube);
	VPEDeleteCamera(Camera);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

VPEcFloat& GetTickRateFactor(VPETickRate& TickRate, VPEcDouble& DeltaTime)
{
	static std::vector<VPEdouble> TickValues;
	static VPEdouble TickFactor = DeltaTime;
	
	TickValues.push_back(DeltaTime);
	
	if (TickValues.size() == TickRate)
	{
		std::for_each(TickValues.begin(), TickValues.end(), [](VPEcDouble& Tick)->void
					  {
						  TickFactor += Tick;
						  return;
					  });

		TickValues.clear();
		TickFactor /= TickRate;
	}
	return static_cast<VPEfloat>(TickFactor);
}

void VPEResizeWindowFun(GLFWwindow* window, int NewWidth, int NewHeight)
{
	if (NewWidth != 0 && NewHeight != 0)
	{
		WindowConfig::Width = NewWidth;
		WindowConfig::Height = NewHeight;

		WindowConfig::AspectRatio = static_cast<float>(WindowConfig::Width) / WindowConfig::Height;
		glViewport(0, 0, WindowConfig::Width, WindowConfig::Height);
	}
	return;
}