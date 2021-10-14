#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 FragmentPosition;
out vec2 TexCoords;
out vec3 Normal;

uniform bool invertedNormals;

uniform mat4 MV_matrix;
uniform mat4 proj_matrix;

void main()
{
    vec4 viewPos = MV_matrix * vec4(position, 1.0);
    FragmentPosition = viewPos.xyz;
    TexCoords = texCoords;

    mat3 normalMatrix = transpose(inverse(mat3(MV_matrix)));
    Normal = normalMatrix * (invertedNormals ? -normal : normal);

    gl_Position = proj_matrix * viewPos;
}