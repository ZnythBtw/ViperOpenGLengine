#version 330 core

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
uniform sampler2D sampler4;
uniform sampler2D sampler5;

in vec3 Position;
in vec3 Normal;
in vec2 UV;

out vec4 OutColor;

void main()
{	
	vec3 texture0 = texture(sampler0, UV).rgb;
	OutColor = vec4(texture0, 1.0);
	
	return;
}