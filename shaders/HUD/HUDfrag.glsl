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
in vec2 UV;

out vec4 OutColor;

void main()
{	
	vec3 texture0 = texture(sampler0, UV).rgb;
	if(texture(sampler0, UV).a < 0.1){
		discard;
	}
	if(texture(sampler1, UV).a < 0.1){
		discard;
	}
	if(texture(sampler2, UV).a < 0.1){
		discard;
	}
	if(texture(sampler3, UV).a < 0.1){
		discard;
	}
	if(texture(sampler4, UV).a < 0.1){
		discard;
	}
	if(texture(sampler5, UV).a < 0.1){
		discard;
	}
	if(texture(sampler6, UV).a < 0.1){
		discard;
	}
	if(texture(sampler7, UV).a < 0.1){
		discard;
	}
	if(texture(sampler8, UV).a < 0.1){
		discard;
	}
	if(texture(sampler9, UV).a < 0.1){
		discard;
	}
	if(texture(sampler10, UV).a < 0.1){
		discard;
	}
	if(texture(sampler11, UV).a < 0.1){
		discard;
	}
	if(texture(sampler12, UV).a < 0.1){
		discard;
	}
	if(texture(sampler13, UV).a < 0.1){
		discard;
	}
	if(texture(sampler14, UV).a < 0.1){
		discard;
	}
	if(texture(sampler15, UV).a < 0.1){
		discard;
	}
	if(texture(sampler16, UV).a < 0.1){
		discard;
	}
	if(texture(sampler17, UV).a < 0.1){
		discard;
	}
	if(texture(sampler18, UV).a < 0.1){
		discard;
	}
	if(texture(sampler19, UV).a < 0.1){
		discard;
	}
	if(texture(sampler20, UV).a < 0.1){
		discard;
	}
	if(texture(sampler21, UV).a < 0.1){
		discard;
	}
	if(texture(sampler22, UV).a < 0.1){
		discard;
	}
	if(texture(sampler23, UV).a < 0.1){
		discard;
	}
	if(texture(sampler24, UV).a < 0.1){
		discard;
	}
	if(texture(sampler25, UV).a < 0.1){
		discard;
	}
	if(texture(sampler26, UV).a < 0.1){
		discard;
	}
	if(texture(sampler27, UV).a < 0.1){
		discard;
	}
	if(texture(sampler28, UV).a < 0.1){
		discard;
	}
	if(texture(sampler29, UV).a < 0.1){
		discard;
	}
	if(texture(sampler30, UV).a < 0.1){
		discard;
	}
	if(texture(sampler31, UV).a < 0.1){
		discard;
	}
	if(texture(sampler32, UV).a < 0.1){
		discard;
	}
	OutColor = vec4(texture0, 1.0);
	return;
}