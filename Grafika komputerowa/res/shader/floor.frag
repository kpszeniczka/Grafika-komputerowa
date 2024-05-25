#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in vec3 position;

uniform float alpha;

void main()
{
	FragColor = vec4(vec3(0.85f));
}