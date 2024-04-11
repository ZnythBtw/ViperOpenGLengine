#version 330 core

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InUV;
layout(location = 3) in mat4 InModelMatrix;

uniform mat4 ModelViewProjection;

out vec3 Position;
out vec3 Normal;
out vec2 UV;

void main()
{
    Position = vec3(InModelMatrix * vec4(InPosition, 1.0));
    Normal = InNormal;
    UV = InUV;

    gl_Position = ModelViewProjection * InModelMatrix * vec4(InPosition, 1.0);
    return;
}