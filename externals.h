#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <vector>
#include <array>
#include <map>
#include <limits>
#include <span>
#include <filesystem>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include "scripts/configs/configs.h"
#include "scripts/utils/utils.h"

struct VertexData
{
	std::vector<glm::vec3> Position;
	std::vector<glm::ivec3> Index;
};
struct UVData
{
	std::vector<glm::vec2> UV;
	std::vector<glm::ivec3> Index;
};
struct NormalData
{
	std::vector<glm::vec3> Normal;
	std::vector<glm::ivec3> Index;
};

struct VPEShadow
{
	VPEuint Shader = VPE_NULL;
	VPEuint ShadowVAO = VPE_NULL;
	VPEuLongLong VertexAmount = VPE_NULL;
	std::vector<glm::vec3> ShadowLocs;
};
struct OBJData
{
	VertexData VData;
	NormalData NData;
	UVData UVData;
};
struct VAOInfo
{
	VPEuint VAO = VPE_NULL;
	VPEuLongLong VertexAmount = VPE_NULL;
};

typedef unsigned int VPEShaderID;
typedef std::string VPE_MVP_Name;

typedef	unsigned int VPETextureID;

class VPEShader
{
private:
	std::string ReadFile(const char* FilePath) const;
public:
	VPEShaderID ShaderID;
	VPE_MVP_Name MVPName = VPE_DEFAULT_MVP_SHADER_ID;

	void LoadShader(VPEFileDir VertexShaderDir, VPEFileDir FragmentShaderDir, const bool& ShowDebug = false);
	void UseShader() const;
	void DeleteShader();
};
class VPETexture
{
public:
	VPETextureID Texture;

	void LoadTexture(VPEFileDir TextureDir, const bool& FilpVertically = true, const bool& ShowDebug = false);
	void UseTexture(const GLenum& Target = GL_TEXTURE_2D);
	void DeleteTexture();
};

struct __VPESimpleObject
{
	glm::vec3 Position = VPEGetNull<glm::vec3>();
	OBJData ObjectData;
	VPEuint VAO = VPE_NULL;
	VPEuLongLong VertexAmount = 0;

	void Draw(const VPEShader& Shader, const glm::mat4& ViewProjection) const;
};
struct __VPEInstancedObject
{
	OBJData InstancedObjectData;
	VPEuint VAO = VPE_NULL;
	VPEuLongLong VertexAmount = 0;
	std::vector<glm::mat4> Instances;
	
	void Draw(const VPEShader& Shader, const glm::mat4& ViewProjection) const;
};

typedef std::map<VPEStringIndex, OBJData> VPEObjectDataMap;
typedef std::map<VPEStringIndex, VAOInfo> VPEVaoMap;

struct VPETextureMap
{
	std::map<VPEStringIndex, VPEcUint> Map;
	VPEDefaultFun Insert(VPEStringIndex& VAOMapIndex, VPEcUint& Texture);
	VPEDefaultFun Remove(VPEStringIndex& Index);
};

struct __VPEAdvancedObject
{
	glm::vec3 Position = VPEGetNull<glm::vec3>();
	VPETextureMap* TextureMap = new VPETextureMap;
	std::vector<std::string> Index;
	VPEObjectDataMap ObjectDataMap;
	VPEVaoMap VAOmap;

	void Draw(const VPEShader& Shader, const glm::mat4& ViewProjection);
	void DrawSingle(VPEStringIndex& Index, const VPEShader& Shader, const glm::mat4& ViewProjection);
};

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
};

typedef __VPESimpleObject* VPESimpleObject;
typedef __VPEInstancedObject* VPEInstancedObject;
typedef __VPEAdvancedObject* VPEAdvancedObject;

class VPEObjectEngine
{
private:
	VPEDefaultFun CheckStringValues(const VPEMode& Mode, const std::string& value, const std::string& filePath) const;
	std::optional<std::string> GetFilteredLine(const std::string& InputLine) const;
	std::string GetFilteredIndex(const std::string& InputLine) const;
	glm::ivec3 StringToIndex(const std::string& x, const std::string& y, const std::string& z, const VPEuint& Decrease) const;
public:
	template<class VPEObjectType> VPEObjectType CreateNewObject() const;
	template<> VPESimpleObject CreateNewObject() const
	{
		return new __VPESimpleObject;
	}
	template<> VPEInstancedObject CreateNewObject() const
	{
		return new __VPEInstancedObject;
	}
	template<> VPEAdvancedObject CreateNewObject() const
	{
		return new __VPEAdvancedObject;
	}

	template<class VPEObjectType> void DeleteObject(VPEObjectType Object) const
	{
		delete Object;
		return;
	}

	OBJData LoadSimpleObjData(const std::string& filePath, const std::string& skip_chars = "gmuos\#", const std::string& mode_chars = "vtf") const;
	
	VPEObjectDataMap LoadAdvancedObjData(const std::string& filePath, VPEcChar& IndexIdentifier, std::vector<std::string>* IndexOut, VPEcBool& ModifyIndex = true, const std::string& skip_chars = "gmous\#", const std::string& mode_chars = "vtf") const;
	VPEVaoMap GetAdvancedVAO(VPEObjectDataMap& ObjDataMap, const std::span<std::string>& Index) const;

	VPEuint& GetSimpleVAO(VPESimpleObject object, VPEcBool& ShowLog = false) const;
	VPEuint& GetSimpleInstancedVAO(VPEInstancedObject object, VPEcBool& ShowLog = false) const;
};
