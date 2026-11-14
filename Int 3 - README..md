Proyecto Artemis
Integrantes y comisión
Integrante 3: Marcos Sebastian Marquez
Comisión: 1.1


Descripción del escenario y reglas del juego 
El proyecto consiste en una simulación relacionada con una misión Artemis desarrollándose las diferentes fases del vuelo de una nave espacial 
Escenario correspondiente: Completar con la descripción detallada del escenario Artemis correspondiente a la cátedra Funcionamiento del programa 
El programa simula las diferentes fases de la misión calculando y archivando los valores correspondientes a los que describe el movimiento de la nave.


Vuelo sin propulsión*
Se implementó la etapa de vuelo sin propulsión. Si se apagan los motores, la nave continúa subiendo impulsada por la velocidad recuperada, mientras que se va frenando de forma progresiva por la acción de la gravedad hasta que su velocidad llega a cero.
Se realizaron los cálculos de la altura máxima alcanzada y del tiempo del vuelo. También se elaboró la simulación paso a paso de dicha etapa.


Historial del vuelo*
Se implementó un sistema de registro del historial a través de un arreglo de hasta 1000 estructuras. Cada estructura contiene la información correspondiente a un paso de la simulación.
La gestión de los registros se efectúa mediante punteros. Al cierre, se imprime una tabla con el historial de los pasos realizados.

Reporte final
Se elaboró el proceso de generación del reporte final de la misión, mostrando los resultados obtenidos a través de la simulación y la información relativa al vuelo.


Makefile
Se elaboró el Makefile para automatizar la compilación del proyecto, permitiendo compilar todos los módulos pulsando tan solo un comando.


Compilación y ejecución
Para compilar y ejecutar el proyecto se deben seguir los siguientes pasos:
1. Ubicarse en la carpeta raíz del repositorio.
2. Ejecutar el comando de compilación:
   make
3. Ejecutar el programa utilizando el comando correspondiente:
  [Completar con el nombre del ejecutable]


Pruebas y verificación
Se realizaron pruebas utilizando los valores proporcionados en el enunciado.
Como resultado de las pruebas del vuelo sin propulsión, se obtuvo una altitud máxima de **474.285,71 metros**, coincidiendo con el valor esperado.
Esto permitió verificar el correcto funcionamiento de los cálculos correspondientes a esta etapa de la simulación.


Registro del uso de Inteligencia Artificial
Se utilizó Claude como soporte para la realización del proyecto en las siguientes tareas:
1.Generación de código: asistió en la generación de algunas partes del programa.
2.Corrección de errores: revisó el código hasta localizar y corregir errores.
3.Resolver dudas:servía para contestar preguntas sobre la lógica del programa, los cálculos físicos del vuelo sin propulsión y el funcionamiento de las distintas implementaciones.
