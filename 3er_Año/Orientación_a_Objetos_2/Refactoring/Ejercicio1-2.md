# Ejercicio 1.2 - Participación en proyectos
Al revisar el siguiente diseño inicial (Figura 1), se decidió realizar un cambio para evitar lo que se consideraba un mal olor. El diseño modificado se muestra en la Figura 2. Indique qué tipo de cambio se realizó y si lo considera apropiado. Justifique su respuesta

## Respuesta:
Sí, lo considero apropiado.
En el primer diseño, había una mala asignación de responsabilidades (bad smell: feature envy). El proyecto conoce (variable de instancia) cuales personas participan en él, y es quien debe dar esa respuesta, como correctamente se detectó en el diseño revisado. Esto salta a la vista por lógica del contexto, y además porque vemos que el método participaEnProyecto lo único que hace es mandarle un mensaje a Proyecto.