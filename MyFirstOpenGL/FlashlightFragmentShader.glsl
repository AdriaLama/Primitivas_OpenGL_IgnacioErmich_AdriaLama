#version 440 core

in vec2 uvsFragmentShader;
in vec3 normalFragmentShader;
in vec3 fragWorldPos;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform vec4 color;
uniform int hasTexture;

uniform vec3 ambientColor;

uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform int sunActive; 

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

    vec3 ambient = ambientColor * baseColor.rgb;

    vec3 diffuse = vec3(0.0);
    if (sunActive == 1)
    {
        float diffuseAngle = max(dot(normal, normalize(sunDirection)), 0.15);
        diffuse = sunColor * baseColor.rgb * diffuseAngle;
    }
    else
    {
        diffuse = baseColor.rgb * 0.15;
    }

    vec3 spotlight = vec3(0.0);
    if (flashlightOn == 1)
    {
        vec3 toFragment = fragWorldPos - flashlightPos;
        float distance = length(toFragment);

        if (distance < flashlightRange)
        {
            vec3 toFragmentDir = normalize(toFragment);

            // Angulo entre la direccion de la linterna y el vector al fragmento
            float theta = dot(toFragmentDir, normalize(flashlightDir));

            // Interpolamos suavemente entre inner y outer cone
            float intensity = clamp((theta - flashlightOuterCone) / (flashlightInnerCone - flashlightOuterCone),0.0, 1.0);

            // Atenuacion por distancia (cuadrado inverso)
            float attenuation = 1.0 / (distance * distance);

            float diffuseAngle = max(dot(normal, -toFragmentDir), 0.0);

            spotlight = baseColor.rgb * intensity * attenuation * diffuseAngle * flashlightIntensity;
;
        }
    }

    vec3 result = ambient + diffuse + spotlight;
    fragColor = vec4(result, baseColor.a);
}