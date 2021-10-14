#version 430

layout(location = 0) in vec3 position;

out vec3 localPos;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;

void main()
{
    localPos = position;

    gl_Position = proj_matrix * view_matrix * vec4(localPos, 1.0);
}