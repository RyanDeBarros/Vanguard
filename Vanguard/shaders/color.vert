#version 440 core

layout(location = 0) in vec2 iPosition;
layout(location = 1) in vec4 iColor;

out vec4 tColor;

void main() {
	gl_Position.xy = iPosition;
	tColor = iColor;
}
