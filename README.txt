BIBLIOGRAFIA:

Para la dirección y distancia de la linterna, hemos consultado la documentación de clase.

Para realizar la tarea asignada, hemos uso de fuentes externas / IA para los siguientes casos:

- El cálculo de el inner y outer cone de la linterna.

El prompt que hemos utilizado en Claude es el siguiente:

Cual seria la mejor manera para aplicar el inner y outer cone, y explicame por que es la manera mas óptima + código linterna nuestro.

Respuesta: 

La lógica
1. ¿Qué ángulo comparamos?
directionLight (que ya calculas) apunta desde el fragmento hacia la linterna. Para saber si ese fragmento está dentro del cono, necesitas compararlo con la dirección en la que el jugador mira (flashlightDir), pero invertida — porque flashlightDir apunta hacia fuera (del jugador hacia la escena) y directionLight apunta hacia dentro (del fragmento hacia el jugador):

glslfloat theta = dot(directionLight, normalize(-flashlightDir));

El dot de dos vectores normalizados te da directamente el coseno del ángulo entre ellos. Eso es clave: trabajamos en espacio de cosenos, no de grados, para evitar llamar a acos() en el shader (operación cara, innecesaria).

2. ¿Por qué guardar flashlightInnerCone/flashlightOuterCone como cosenos?

Porque así la comparación es directa. Desde C++ le pasas cos(radians(angulo)) como uniform, y en el shader solo comparas theta contra esos dos valores ya convertidos. Es más rápido que convertir en cada fragmento, y evita ese cálculo trigonométrico extra multiplicado por miles de píxeles.

3. ¿Por qué smoothstep y no un if cortante?

Si hicieras if (theta > innerCone) intensidad = 1.0; else if (theta > outerCone) intensidad = algo; else intensidad = 0.0; tendrías un borde duro y antiestético entre el inner y el outer cone. smoothstep(outerCone, innerCone, theta) te da una interpolación suave: 0.0 fuera del outer cone, 1.0 dentro del inner cone, y una transición gradual en medio. Es el método estándar (LearnOpenGL lo usa así) precisamente porque resuelve el problema de los bordes duros con una sola línea, sin ramas condicionales costosas.

Usé la fórmula manual clamp((theta - outer) / epsilon, 0.0, 1.0) en vez de smoothstep directamente porque es exactamente lo que hace LearnOpenGL y te da control explícito sobre el rango — pero si prefieres más legibilidad, smoothstep(flashlightOuterCone, flashlightInnerCone, theta) hace lo mismo en una línea.


Sabiendo esto, hemos consultado la página https://learnopengl.com/Lighting/Light-casters para acabar de hacer la linterna con el cono correctamente.