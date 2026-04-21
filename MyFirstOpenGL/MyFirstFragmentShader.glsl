#version 440 core
uniform vec2 windowSize;
uniform int objectID;
out vec4 fragColor;
uniform float tiempo;



void main()
{
    if (objectID == 0 || objectID == 1)
    {
        if (gl_FragCoord.y > windowSize.y / 2.0)
            fragColor = vec4(1.0, 1.0, 0.0, 1.0); // Amarillo
        else
            fragColor = vec4(1.0, 0.5, 0.0, 1.0); // Naranja
    }
   else {
    
        int indiceCiclo = int(mod(tiempo / 2.0, 3.0));

        if (indiceCiclo == 0) {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0); // Rojo
        } else if (indiceCiclo == 1) {
            fragColor = vec4(0.0, 1.0, 0.0, 1.0); // Verde
        } else {
            fragColor = vec4(0.0, 0.0, 1.0, 1.0); // Azul
        }
    }

}
   
