#version 430 

layout(location = 0) in vec3 position;

uniform mat4 lightMatrix;
uniform mat4 model_matrix;

void main()
{
    gl_Position = lightMatrix * model_matrix * vec4(position, 1.0);
}