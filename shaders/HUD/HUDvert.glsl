#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec2 InUV;

uniform mat4 ModelViewProjection;

out vec3 Position;
out vec2 UV;

void main()
{
	Position = InPosition;
	UV = InUV;
	gl_Position = ModelViewProjection * vec4(InPosition, 1.0);

	return;
}