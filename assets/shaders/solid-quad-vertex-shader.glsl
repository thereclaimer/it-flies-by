#version 330 core

layout (location = 0) in vec2 vertex_position;
layout (location = 2) in vec4 vertex_color;

uniform mat3 transform;

out vec4 fragment_color;

void main()
{
    gl_Position = vec4(transform * vec3(vertex_position, 1.0), 1.0);

    fragment_color = vertex_color;
}