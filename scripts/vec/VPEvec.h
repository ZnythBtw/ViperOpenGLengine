#pragma once
#include <string>

typedef float VPEfloat;
typedef unsigned short VPEushort;

enum _VPEVecSet
{
	VEC1_SET = 1,
	VEC2_SET = 2,
	VEC3_SET = 3,
};

template<VPEushort VecSize, typename VecType> struct VPEvec;
template<> struct VPEvec<_VPEVecSet::VEC1_SET, VPEfloat>
{
	VPEfloat x = 0.0f;

	VPEfloat Sizeof = sizeof(VPEfloat) * _VPEVecSet::VEC1_SET;
};
template<> struct VPEvec<_VPEVecSet::VEC2_SET, VPEfloat>
{
	VPEfloat
		x = 0.0f,
		y = 0.0f;

	VPEfloat Sizeof = sizeof(VPEfloat) * _VPEVecSet::VEC2_SET;
};
template<> struct VPEvec<_VPEVecSet::VEC3_SET, VPEfloat>
{
	VPEfloat
		x = 0.0f,
		y = 0.0f,
		z = 0.0f;

	VPEfloat Sizeof = sizeof(VPEfloat) * _VPEVecSet::VEC3_SET;
};

typedef VPEvec<_VPEVecSet::VEC1_SET, VPEfloat> VPEvec1;
typedef VPEvec<_VPEVecSet::VEC2_SET, VPEfloat> VPEvec2;
typedef VPEvec<_VPEVecSet::VEC3_SET, VPEfloat> VPEvec3;
typedef const VPEvec<_VPEVecSet::VEC1_SET, VPEfloat> VPEcVec1;
typedef const VPEvec<_VPEVecSet::VEC2_SET, VPEfloat> VPEcVec2;
typedef const VPEvec<_VPEVecSet::VEC3_SET, VPEfloat> VPEcVec3;

template<class T> std::string VPEvecToString(const T& Vec);
template<> std::string VPEvecToString(VPEcVec1& Vec)
{
	return std::to_string(Vec.x);
}
template<> std::string VPEvecToString(VPEcVec2& Vec)
{
	return std::to_string(Vec.x) + ", " + std::to_string(Vec.y);
}
template<> std::string VPEvecToString(VPEcVec3& Vec)
{
	return std::to_string(Vec.x) + ", " + std::to_string(Vec.y) + ", " + std::to_string(Vec.z);
}