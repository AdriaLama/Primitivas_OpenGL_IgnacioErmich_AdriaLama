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

// Sol
uniform vec3 sunDirection;
uniform float sunIntensity;

// Linterna
uniform int flashlightOn;
uniform vec3 flashlightPos;
uniform vec3 flashlightDir;
uniform float flashlightInnerCone;
uniform float flashlightOuterCone;
uniform float flashlightRange;

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

    // Sol (diffuse direccional)
    float diff = max(dot(normal, normalize(sunDirection)), 0.0);
    diff = max(diff, 0.15); // minimo 15%
    vec3 sun = baseColor.rgb * diff * sunIntensity;

    // Linterna
    vec3 spotlight = vec3(0.0);
    if (flashlightOn == 1)
    {
        vec3 toLight = normalize(flashlightPos - fragWorldPos);
        float dist = length(flashlightPos - fragWorldPos);
        float angle = dot(toLight, normalize(-flashlightDir));
        float coneBlend = clamp((angle - flashlightOuterCone) / (flashlightInnerCone - flashlightOuterCone), 0.0, 1.0);
        float range = 1.0 / (dist/flashlightRange * dist/flashlightRange);

        spotlight = baseColor.rgb * coneBlend * range;
    }

    fragColor = vec4(clamp(ambient + sun + spotlight, 0.0, 1.0), baseColor.a);
}