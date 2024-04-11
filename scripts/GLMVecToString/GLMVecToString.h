#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define VPE_DEFAULT_DIV_CHAR ','

template<typename T> std::string VPEGLMVecToString(const T& vec, const char& divChar = VPE_DEFAULT_DIV_CHAR);
template<typename T> T VPEGLMStringToVec(const std::string& str, const char& divChar = VPE_DEFAULT_DIV_CHAR);

template<> std::string VPEGLMVecToString(const glm::dvec1& vec, const char& divChar)
{
	return std::to_string(vec.x);
}
template<> std::string VPEGLMVecToString(const glm::dvec2& vec, const char& divChar)
{
	return std::to_string(vec.x) + divChar + std::to_string(vec.y);
}
template<> std::string VPEGLMVecToString(const glm::dvec3& vec, const char& divChar)
{
	return std::to_string(vec.x) + divChar + std::to_string(vec.y) + divChar + std::to_string(vec.z);
}

template<> std::string VPEGLMVecToString(const glm::ivec1& vec, const char& divChar)
{
	return std::to_string(vec.x);
}
template<> std::string VPEGLMVecToString(const glm::ivec2& vec, const char& divChar)
{
	return std::to_string(vec.x) + divChar + std::to_string(vec.y);
}
template<> std::string VPEGLMVecToString(const glm::ivec3& vec, const char& divChar)
{
	return std::to_string(vec.x) + divChar + std::to_string(vec.y) + divChar + std::to_string(vec.z);
}

template<> std::string VPEGLMVecToString(const glm::vec1& vec, const char& divChar)
{
	return std::to_string(vec.x);
}
template<> std::string VPEGLMVecToString(const glm::vec2& vec, const char& divChar)
{
	return std::to_string(vec.x) + divChar + std::to_string(vec.y);
}
template<> std::string VPEGLMVecToString(const glm::vec3& vec, const char& divChar)
{
	return std::to_string(vec.x) + divChar + std::to_string(vec.y) + divChar + std::to_string(vec.z);
}

template<> glm::dvec1 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	return glm::dvec1{ std::stod(str) };
}
template<> glm::dvec2 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	std::string
		x = "",
		y = "";

	x.insert(x.begin(), str.begin(), str.begin() + str.find_first_of(divChar));
	y.insert(y.begin(), str.begin() + str.find_first_of(divChar) + 1, str.end());

	return { std::stod(x), std::stod(y) };
}
template<> glm::dvec3 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	std::string
		x = "",
		y = "",
		z = "";

	x.insert(x.begin(), str.begin(), str.begin() + str.find_first_of(divChar));
	y.insert(y.begin(), str.begin() + str.find_first_of(divChar) + 1, str.begin() + str.find_last_of(divChar));
	z.insert(z.begin(), str.begin() + str.find_last_of(divChar) + 1, str.end());

	return { std::stod(x), std::stod(y), std::stod(z) };
}

template<> glm::ivec1 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	return glm::ivec1{ std::stoi(str) };
}
template<> glm::ivec2 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	std::string
		x = "",
		y = "";

	x.insert(x.begin(), str.begin(), str.begin() + str.find_first_of(divChar));
	y.insert(y.begin(), str.begin() + str.find_first_of(divChar) + 1, str.end());

	return { std::stoi(x), std::stoi(y) };
}
template<> glm::ivec3 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	std::string
		x = "",
		y = "",
		z = "";

	x.insert(x.begin(), str.begin(), str.begin() + str.find_first_of(divChar));
	y.insert(y.begin(), str.begin() + str.find_first_of(divChar) + 1, str.begin() + str.find_last_of(divChar));
	z.insert(z.begin(), str.begin() + str.find_last_of(divChar) + 1, str.end());

	return { std::stoi(x), std::stoi(y), std::stoi(z) };
}

template<> glm::vec1 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	return glm::vec1{ std::stof(str) };
}
template<> glm::vec2 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	std::string
		x = "",
		y = "";

	x.insert(x.begin(), str.begin(), str.begin() + str.find_first_of(divChar));
	y.insert(y.begin(), str.begin() + str.find_first_of(divChar) + 1, str.end());

	return { std::stof(x), std::stof(y) };
}
template<> glm::vec3 VPEGLMStringToVec(const std::string& str, const char& divChar)
{
	std::string
		x = "",
		y = "",
		z = "";

	x.insert(x.begin(), str.begin(), str.begin() + str.find_first_of(divChar));
	y.insert(y.begin(), str.begin() + str.find_first_of(divChar) + 1, str.begin() + str.find_last_of(divChar));
	z.insert(z.begin(), str.begin() + str.find_last_of(divChar) + 1, str.end());

	return { std::stof(x), std::stof(y), std::stof(z) };
}

//vec to string macros
#define VPEGLM_vec1ToString VPEGLMVecToString<glm::vec1>
#define VPEGLM_vec2ToString VPEGLMVecToString<glm::vec2>
#define VPEGLM_vec3ToString VPEGLMVecToString<glm::vec3>

#define VPEGLM_dvec1ToString VPEGLMVecToString<glm::dvec1>
#define VPEGLM_dvec2ToString VPEGLMVecToString<glm::dvec2>
#define VPEGLM_dvec3ToString VPEGLMVecToString<glm::dvec3>

#define VPEGLM_ivec1ToString VPEGLMVecToString<glm::ivec1>
#define VPEGLM_ivec2ToString VPEGLMVecToString<glm::ivec2>
#define VPEGLM_ivec3ToString VPEGLMVecToString<glm::ivec3>

//string to vec macros
#define VPEGLM_StringToVec1 VPEGLMStringToVec<glm::vec1>
#define VPEGLM_StringToVec2 VPEGLMStringToVec<glm::vec2>
#define VPEGLM_StringToVec3 VPEGLMStringToVec<glm::vec3>

#define VPEGLM_StringToDVec1 VPEGLMStringToVec<glm::dvec1>
#define VPEGLM_StringToDVec2 VPEGLMStringToVec<glm::dvec2>
#define VPEGLM_StringToDVec3 VPEGLMStringToVec<glm::dvec3>

#define VPEGLM_StringToIVec1 VPEGLMStringToVec<glm::ivec1>
#define VPEGLM_StringToIVec2 VPEGLMStringToVec<glm::ivec2>
#define VPEGLM_StringToIVec3 VPEGLMStringToVec<glm::ivec3>