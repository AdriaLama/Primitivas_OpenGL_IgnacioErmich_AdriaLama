#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalFragmentShader;
in vec3 fragWorldPos;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform vec4 color;
uniform int hasTexture;

// Iluminacion global
uniform vec3 ambientColor;

// Linterna
uniform int flashlightOn;
uniform vec3 flashlightPos;
uniform vec3 flashlightDir;
uniform float flashlightInnerCone;
uniform float flashlightOuterCone;
uniform float flashlightRange;
uniform float flashlightIntensity;

void main()
{
    vec4 baseColor;

    if (hasTexture == 1)
    {
        vec2 adjustedTexCoord = vec2(uvsFragmentShader.x, 1.0 - uvsFragmentShader.y);
        baseColor = texture(textureSampler, adjustedTexCoord) * color;
    }
    else
    {
        baseColor = color;
    }

    vec3 normal = normalize(normalFragmentShader);

    // Ambient
    vec3 ambient = ambientColor * baseColor.rgb;

    // Spotlight
    vec3 spotlight = vec3(0.0);
    if (flashlightOn == 1)
    {
        vec3 lightDir = normalize(flashlightPos - fragWorldPos);
        float distance = length(flashlightPos - fragWorldPos);
        float theta = dot(lightDir, normalize(-flashlightDir));
        float epsilon = flashlightInnerCone - flashlightOuterCone;
        float intensity = clamp((theta - flashlightOuterCone) / epsilon, 0.0, 1.0);
        float attenuation = 1.0 / (distance * distance);

        // Diffuse Lambert
        float diffuseAngle = max(dot(normal, lightDir), 0.0);

        spotlight = baseColor.rgb * intensity * attenuation * diffuseAngle * flashlightIntensity;
    }

    fragColor = vec4(clamp(ambient + spotlight, 0.0, 1.0), baseColor.a);
}