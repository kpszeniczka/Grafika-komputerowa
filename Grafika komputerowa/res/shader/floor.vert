#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 position;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	vec3 crntPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = camMatrix * vec4(crntPos, 1.0f);
}