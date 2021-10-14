#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 _position;
out vec3 _normal;
out vec2 _texCoords;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main()
{
    _position = vec3(model_matrix * vec4(position, 1.0));
    _normal = transpose(inverse(mat3(model_matrix))) * normal;
    _texCoords = texCoords;

    gl_Position = proj_matrix * view_matrix * vec4(_position, 1.0);
}