#version 410 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 tcc;

out VS_OUT{
    vec2 tc;
}vs_out;

out vec4 color;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

void main(void)
{
    vs_out.tc = tcc;
    gl_Position = proj_matrix * m_matrix * v_matrix * position;
    color = position * 2 + vec4(0.4, 0.3, 0.2, 0.0);
}
