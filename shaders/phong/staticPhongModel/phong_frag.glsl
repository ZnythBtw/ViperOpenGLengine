#version 330 core

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
uniform sampler2D sampler4;
uniform sampler2D sampler5;
uniform sampler2D sampler6;
uniform sampler2D sampler7;
uniform sampler2D sampler8;
uniform sampler2D sampler9;
uniform sampler2D sampler10;
uniform sampler2D sampler11;
uniform sampler2D sampler12;
uniform sampler2D sampler13;
uniform sampler2D sampler14;
uniform sampler2D sampler15;
uniform sampler2D sampler16;
uniform sampler2D sampler17;
uniform sampler2D sampler18;
uniform sampler2D sampler19;
uniform sampler2D sampler20;
uniform sampler2D sampler21;
uniform sampler2D sampler22;
uniform sampler2D sampler23;
uniform sampler2D sampler24;
uniform sampler2D sampler25;
uniform sampler2D sampler26;
uniform sampler2D sampler27;
uniform sampler2D sampler28;
uniform sampler2D sampler29;
uniform sampler2D sampler30;
uniform sampler2D sampler31;
uniform sampler2D sampler32;

in vec3 Position;
in vec3 Normal;
in vec2 UV;

uniform vec3 LightDirection;
uniform float LightIntensity;
uniform float Alpha;

out vec4 OutColor;

void main()
{	
	vec3 N = normalize(Normal);

	vec3 L = -normalize(LightDirection);
	float Lambertian = max(dot(N, L), 0.0);

	vec3 ViewDirection = vec3(0.0, 0.0, -1.0);
	vec3 V = -ViewDirection;

	vec3 R = reflect(-L, N);

	float Specular = pow(max(dot(R, V), 0.0), Alpha);
	Specular = max(Specular, 0.0);

	vec3 bTexture = texture(sampler0, UV).rgb;
	vec3 FinalTexture = bTexture * LightIntensity * Lambertian + Specular;
	
	OutColor = vec4(FinalTexture, 1.0);
	return;
}