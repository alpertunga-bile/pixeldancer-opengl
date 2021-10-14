#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main()
{
    TexCoords = texCoords;
    WorldPos = vec3(model_matrix * vec4(position, 1.0));
    Normal = mat3(model_matrix) * normal;

    gl_Position = proj_matrix * view_matrix * vec4(WorldPos, 1.0);
}