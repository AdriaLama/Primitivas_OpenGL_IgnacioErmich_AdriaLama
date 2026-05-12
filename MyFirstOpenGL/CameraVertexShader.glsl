#version 440 core
layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 uvsVertexShader;

out vec2 uvsFragmentShader;

uniform mat4 transform;     
uniform mat4 view;          
uniform mat4 projection;    

void main()
{
    uvsFragmentShader = uvsVertexShader;
    gl_Position = projection * view * transform * vec4(posicion, 1.0);
}