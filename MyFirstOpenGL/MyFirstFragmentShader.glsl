#version 440 core
uniform vec2 windowSize;
uniform int objectID;
out vec4 fragColor;

void main()
{
    if (objectID == 0)
    {
        if (gl_FragCoord.y > windowSize.y / 2.0)
            fragColor = vec4(1.0, 1.0, 0.0, 1.0); // Amarillo
        else
            fragColor = vec4(1.0, 0.5, 0.0, 1.0); // Naranja
    }
    else
    {
        fragColor = vec4(1.0, 1.0, 1.0, 1.0); 
    }
}
   
