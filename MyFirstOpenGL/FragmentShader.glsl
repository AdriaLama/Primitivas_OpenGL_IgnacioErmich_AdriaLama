#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalFragmentShader;
in vec3 fragWorldPos;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform sampler2D dayNightSampler;
uniform vec4 color;
uniform int hasTexture;

uniform float dayNightTime;

// Linterna
uniform int flashlightOn;
uniform vec3 flashlightPos;
uniform vec3 flashlightDir;
uniform float flashlightInnerCone;
uniform float flashlightOuterCone;

void main()
{
    vec4 baseColor;

    vec2 dayNightTextCoord = vec2(dayNightTime, 0.5);

    if (hasTexture == 1)
    {
        vec2 adjustedTexCoord = vec2(uvsFragmentShader.x, 1.0 - uvsFragmentShader.y);
        baseColor = texture(textureSampler, adjustedTexCoord) * texture(dayNightSampler, dayNightTextCoord) * color;
    }
    else
    {
        baseColor = texture(dayNightSampler, dayNightTextCoord) * color;
    }

    // Linterna
    vec3 spotlight = vec3(0.0);
    if (flashlightOn == 1)
    {
        vec3 directionLight = normalize(flashlightPos - fragWorldPos); //Direccion hacia la linterna
        float distance = length(flashlightPos - fragWorldPos); //Distancia real a la linterna
        float attenuation = 1.0 / distance * distance;   //Atenuacion cuadratica por distancia, pierde intensidad según la distancia

        // Calcula el coseno del ángulo entre la dirección hacia la luz y la dirección de la linterna invertida
        float theta = dot(directionLight, normalize(-flashlightDir)); 

        // Diferencia entre el coseno del cono interno y el externo, define el ancho de la zona de transición 
        float epsilon = flashlightInnerCone - flashlightOuterCone;

        // Normaliza theta dentro del rango [outerCone, innerCone] y lo limita entre 0 y 1 para suavizar el borde del cono
        float intensityCone = clamp((theta - flashlightOuterCone) / epsilon, 0.0, 1.0);

        spotlight = baseColor.rgb * attenuation * intensityCone;
    }

    fragColor = vec4(clamp(baseColor.rgb + spotlight, 0.0, 1.0), baseColor.a);
}