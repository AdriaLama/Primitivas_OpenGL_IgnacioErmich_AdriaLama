#version 440 core

layout(location = 0) in vec3 posicion;
layout(location = 1) in vec2 uvsVertexShader;
layout(location = 2) in vec3 normalVertexShader;

out vec2 uvsFragmentShader;
out vec3 normalFragmentShader;
out vec3 fragWorldPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    uvsFragmentShader = uvsVertexShader;

    // Transformamos la normal con la matriz modelo para que rote con el objeto
    normalFragmentShader = mat3(transform) * normalVertexShader;

    // Posicion del vertice en world space, necesaria para calcular distancia y direccion a la linterna
    fragWorldPos = vec3(transform * vec4(posicion, 1.0));

    gl_Position = projection * view * transform * vec4(posicion, 1.0);
}