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
    vec4 worldPosition = model * vec4(position, 1.0f);
    gl_Position = camMatrix * worldPosition;

    v_Position = worldPosition.xyz; // Transform to world space
    v_Normals = mat3(transpose(inverse(model))) * normals; // Transform normals to world space
    v_TexCoord = texCoords;
    v_FragPositionLight = u_LightProjection * worldPosition;
}
