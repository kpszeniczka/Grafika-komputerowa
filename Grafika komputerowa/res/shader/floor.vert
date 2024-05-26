#version 330 core

layout(location = 0) in vec3 position;
layout(location = 3) in vec3 normals;
layout(location = 2) in vec2 texCoords;

out vec3 v_Position;
out vec3 v_Normals;
out vec2 v_TexCoord;
out vec4 v_FragPositionLight;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 u_LightProjection;

void main() 
{
    gl_Position = camMatrix * model * vec4(position, 1.0f);

	v_Position = position;
	v_Normals = normals;
	v_TexCoord = texCoords;
	v_FragPositionLight = u_LightProjection * vec4(position, 1.0f);
}