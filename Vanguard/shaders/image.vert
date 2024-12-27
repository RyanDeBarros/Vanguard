#version 440 core

layout(location = 0) in vec2 iPosition;
layout(location = 1) in int iTexSlot;
layout(location = 2) in vec2 iUVs;
layout(location = 3) in vec4 iColor;

flat out int tTexSlot;
out vec2 tUVs;
out vec4 tColor;

void main() {
	gl_Position.xy = iPosition;
	tTexSlot = iTexSlot;
	tUVs = iUVs;
	tColor = iColor;
}
