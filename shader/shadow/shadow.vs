#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out VS_OUT
{
    vec3 _position;
    vec3 _normal;
    vec2 _texCoords;
    vec4 _fragPosLightSpace;
} vs_out;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform mat4 lightMatrix;

void main()
{
    vs_out._position = vec3(model_matrix * vec4(position, 1.0));
    vs_out._normal = transpose(inverse(mat3(model_matrix))) * normal;
    vs_out._texCoords = texCoords;
    vs_out._fragPosLightSpace = lightMatrix * vec4(vs_out._position, 1.0);

    gl_Position = proj_matrix * view_matrix * vec4(vs_out._position, 1.0);
}