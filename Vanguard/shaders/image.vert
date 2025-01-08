#version 440 core

uniform mat3 uVP = mat3(1.0);

layout(location = 0) in vec2 iPosition;
layout(location = 1) in int iTexSlot;
layout(location = 2) in vec2 iUVs;
layout(location = 3) in vec4 iColor;
layout(location = 4) in mat3 iModel;

flat out int tTexSlot;
out vec2 tUVs;
out vec4 tColor;

void main() {
	gl_Position.xyz = uVP * iModel * vec3(iPosition, 1.0);
	tTexSlot = iTexSlot;
	tUVs = iUVs;
	tColor = iColor;
}
