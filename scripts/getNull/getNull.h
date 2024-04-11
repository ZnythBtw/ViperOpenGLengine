#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define getnullv1 GetNull<glm::vec1>
#define getnullv2 GetNull<glm::vec2>
#define getnullv3 GetNull<glm::vec3>
#define getnullv4 GetNull<glm::vec4>

#define getnulliv1 GetNull<glm::ivec1>
#define getnulliv2 GetNull<glm::ivec2>
#define getnulliv3 GetNull<glm::ivec3>
#define getnulliv4 GetNull<glm::ivec4>

#define getnulldv1 GetNull<glm::dvec1>
#define getnulldv2 GetNull<glm::dvec2>
#define getnulldv3 GetNull<glm::dvec3>
#define getnulldv4 GetNull<glm::dvec4>

template<typename T> T GetNull();

template <> glm::vec1 GetNull()
{
	return glm::vec1{ 0.0f };
}
template <> glm::vec2 GetNull()
{
	return glm::vec2{ 0.0f, 0.0f };
}
template <> glm::vec3 GetNull()
{
	return glm::vec3{ 0.0f, 0.0f, 0.0f };
}
template <> glm::vec4 GetNull()
{
	return glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f };
}

template <> glm::ivec1 GetNull()
{
	return glm::ivec1{ 0 };
}
template <> glm::ivec2 GetNull()
{
	return glm::ivec2{ 0, 0 };
}
template <> glm::ivec3 GetNull()
{
	return glm::ivec3{ 0, 0, 0 };
}
template <> glm::ivec4 GetNull()
{
	return glm::ivec4{ 0, 0, 0, 0 };
}

template <> glm::dvec1 GetNull()
{
	return glm::dvec1{ 0.0 };
}
template <> glm::dvec2 GetNull()
{
	return glm::dvec2{ 0.0, 0.0 };
}
template <> glm::dvec3 GetNull()
{
	return glm::dvec3{ 0.0, 0.0, 0.0 };
}
template <> glm::dvec4 GetNull()
{
	return glm::dvec4{ 0.0, 0.0, 0.0, 0.0 };
}