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
uniform float flashlightRange;

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
        vec3 toLight = normalize(flashlightPos - fragWorldPos);
        float dist = length(flashlightPos - fragWorldPos);
        float angle = dot(toLight, normalize(-flashlightDir));
        float coneBlend = clamp((angle - flashlightOuterCone) / (flashlightInnerCone - flashlightOuterCone), 0.0, 1.0);
        float range = 1.0 / (dist/flashlightRange * dist/flashlightRange);

        spotlight = baseColor.rgb * coneBlend * range;
    }

    fragColor = vec4(clamp(baseColor.rgb + spotlight, 0.0, 1.0), baseColor.a);
}