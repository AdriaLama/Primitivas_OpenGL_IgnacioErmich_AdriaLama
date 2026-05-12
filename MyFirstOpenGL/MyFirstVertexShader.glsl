#version 440 core

layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 uvsVertexShader;

uniform mat4 transform;

out vec2 uvsFragmentShader;

void main()
{
    uvsFragmentShader = uvsVertexShader;
    gl_Position = transform * vec4(posicion, 1.0);
}
