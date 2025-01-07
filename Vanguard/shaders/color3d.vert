#version 440 core

uniform mat4 uVP = mat4(1.0);

layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec4 iColor;
layout(location = 2) in mat4 iModel;

out vec4 tColor;

void main() {
	gl_Position = uVP * iModel * vec4(iPosition, 1.0);
	tColor = iColor;
}
