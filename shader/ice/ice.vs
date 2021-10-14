#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec3 FragmentPosition;
out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

struct Light
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

uniform Light light;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform vec3 cameraPos;

void main()
{
    FragmentPosition = vec3(model_matrix * vec4(position, 1.0));
    TexCoords = texCoords;

    mat3 norm_matrix = transpose(inverse(mat3(model_matrix)));
    vec3 T = normalize(norm_matrix * tangent);
    vec3 N = normalize(norm_matrix * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    TangentLightPos = TBN * light.position;
    TangentViewPos = TBN * cameraPos;
    TangentFragPos = TBN * FragmentPosition;

    gl_Position = proj_matrix * model_matrix * view_matrix * vec4(position, 1.0);
}