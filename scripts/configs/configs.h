#pragma once
#include <iostream>
#include <limits>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

typedef bool VPEbool;
typedef const bool VPEcBool;
typedef char VPEchar;
typedef const char VPEcChar;
typedef int VPEint;
typedef unsigned int VPEuint;
typedef float VPEfloat;
typedef double VPEdouble;
typedef short VPEshort;
typedef unsigned short VPEushort;
typedef long VPElong;
typedef unsigned long VPEuLong;
typedef long long VPELongLong;
typedef unsigned long long VPEuLongLong;
typedef const VPEint VPEcInt;
typedef const VPEfloat VPEcFloat;
typedef const VPEdouble VPEcDouble;
typedef const VPEshort VPEcShort;
typedef const VPELongLong VPEcLongLong;
typedef const VPEuint VPEcUint;
typedef const VPEushort VPEcUshort;
typedef const VPEuLongLong VPEcULongLong;
typedef const VPEfloat VPEcFloat;
typedef const VPEdouble VPEcDouble;

typedef glm::vec2 VPEResolution;
typedef const std::string VPEStringIndex;
typedef const std::string& VPEFileDir;
typedef void VPEDefaultFun;

typedef unsigned short VPEMode;
typedef VPEcUint VPETextureId;
typedef VPEcUint VPEShaderId;

#define VPE_TO_INT static_cast<VPEint>
#define VPE_TO_SHORT static_cast<VPEshort>
#define VPE_TO_FLOAT static_cast<VPEfloat>
#define VPE_TO_UINT static_cast<VPEuint>
#define VPE_TO_USHORT static_cast<VPEushort>
#define VPE_TO_LONG static_cast<VPElong>
#define VPE_TO_LL static_cast<VPELongLong>
#define VPE_TO_ULONG static_cast<VPEuLong>
#define VPE_TO_ULL static_cast<VPEuLongLong>

#define VPE_RESET_X glm::vec3{0.0f, 1.0f, 1.0f}
#define VPE_RESET_Y glm::vec3{1.0f, 0.0f, 1.0f}
#define VPE_RESET_Z glm::vec3{1.0f, 1.0f, 0.0f}

#define VPE_TRY_CATCH_EXIT std::cerr << out.what() << std::endl; exit
#define VPE_CATCH_RUNTIME_ERROR const std::runtime_error& out
#define VPE_CATCH_EXCEPTION const std::exception& out

#define VPE_DEFAULT_MVP_SHADER_ID "ModelViewProjection" 
#define VPE_DEFAULT_WINDOW_TITLE "ViperBaseProject"

#define VPE_UNLIMITED std::string::npos
#define VPE_SPACE_CHAR 32

#define VPE_NULL 0
#define VPE_NULL_STRING ""

#define VPE_ENABLE 1
#define VPE_DISABLE 0

#define	VPE_CHECK_VERTEX_MODE 0
#define	VPE_CHECK_NORMAL_MODE 1

#define	VPE_CHECK_UV_MODE 2
#define	VPE_CHECK_INDEX_MODE 3

#define VPE_VSYNC_ON 1
#define VPE_VSYNC_OFF 0

#define VPE_BIND_ALL 0
#define VPE_BIND_SINGLE 1

#define VPE_TRANSLATE_ROTATE 0
#define VPE_ROTATE_TRANSLATE 1

#define VPE_ROTATE_X glm::vec3{0.0f, 1.0f, 0.0f}
#define VPE_ROTATE_Y glm::vec3{1.0f, 0.0f, 0.0f}

#define Id glm::identity<glm::mat4>()
#define VPE_ID_MAT1 glm::identity<glm::mat1>()
#define VPE_ID_MAT2 glm::identity<glm::mat2>()
#define VPE_ID_MAT3 glm::identity<glm::mat3>()
#define VPE_ID_MAT4 glm::identity<glm::mat4>()

#define VPE_BLACK_BACKGROUND 0.0f, 0.0f, 0.0f, 1.0f
#define VPE_WHITE_BACKGROUND 1.0f, 1.0f, 1.0f, 1.0f
#define VPE_GRAY_BACKGROUND 0.5f, 0.5f, 0.5f, 1.0f
#define VPE_BLUE_BACKGROUND 0.0f, 0.0f, 1.0f, 1.0f
#define VPE_LIGHT_BLUE_BACKGROUND 0.0f, 0.5f, 1.0f, 1.0f
#define VPE_RED_BACKGROUND 1.0f, 0.0f, 0.0f, 1.0f
#define VPE_GREEN_BACKGROUND 0.0f, 1.0f, 0.0f, 1.0f

#define VPE_RESOLUTION_854x480 854, 480
#define VPE_RESOLUTION_1024x576 1024, 576
#define VPE_RESOLUTION_1280x720 1280, 720
#define VPE_RESOLUTION_1366x768 1366, 768
#define VPE_RESOLUTION_1600x900 1600, 900
#define VPE_RESOLUTION_1920x1080 1920, 1080

#define VPE_1_TICK_RATE 1
#define VPE_64_TICK_RATE 64
#define VPE_128_TICK_RATE 128

#define VPE_DEFAULT_LINE_WIDTH 1.0f
#define VPE_DEFAULT_POINT_SIZE 5.0f

#define VPE_UNBIND_ARRAY_BUFFER glBindBuffer(GL_ARRAY_BUFFER, VPE_NULL)
#define VPE_UNBIND_ELEMENT_BUFFER glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VPE_NULL)
#define VPE_UNBIND_VAO glBindVertexArray(VPE_NULL);

static constexpr const float __INTERNAL_VPE_TO_RAD = 180.0f / glm::pi<float>();
#define VPE_RAD_FACTOR __INTERNAL_VPE_TO_RAD

#define VPE_INT_MAX_LIMIT std::numeric_limits<int>().max()
#define VPE_INT_MIN_LIMIT std::numeric_limits<int>().min()

#define VPE_LONG_MAX_LIMIT std::numeric_limits<long>().max()
#define VPE_LONG_MIN_LIMIT std::numeric_limits<long>().min()

#define VPE_LONG_LONG_MAX_LIMIT std::numeric_limits<long long>().max()
#define VPE_LONG_LONG_MIN_LIMIT std::numeric_limits<long long>().min()

#define VPE_FLOAT_MAX_LIMIT std::numeric_limits<float>().max()
#define VPE_FLOAT_MIN_LIMIT std::numeric_limits<float>().min()

#define VPE_DOUBLE_MAX_LIMIT std::numeric_limits<double>().max()
#define VPE_DOUBLE_MIN_LIMIT std::numeric_limits<double>().min()