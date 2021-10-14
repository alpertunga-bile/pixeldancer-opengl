#version 430

layout(location = 0) in vec3 position;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;

out vec3 WorldPos;

void main()
{
    WorldPos = position;

    mat4 stableView = mat4(mat3(view_matrix));
    vec4 clipPos = proj_matrix * stableView * vec4(WorldPos, 1.0);

    // z value(depth) always 1.0
    gl_Position = clipPos.xyww;
}