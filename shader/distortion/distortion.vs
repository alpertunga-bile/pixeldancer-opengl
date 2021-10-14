#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 MV_matrix;
uniform mat4 proj_matrix;

void main()
{
    TexCoords = texCoords;

    gl_Position = proj_matrix * MV_matrix * vec4(position, 1.0);
}