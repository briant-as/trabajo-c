# trabajo-c
## Fase 2: propulsión del núcleo y apagado de motores

**Responsable:** briant lopez
**Archivos:** `propulsion.c`, `propulsion.h` y las funciones de lectura de números reales de `validaciones.c`

### Qué resuelve esta fase

La fase 2 empieza en el desprendimiento de los cohetes SRB y termina cuando se apagan los motores, a la altura `h`. Desde ese momento solo empuja el motor del núcleo principal, con una aceleración neta constante `a2`. Es un MRUV, y la nave **no** parte del reposo: arranca con la velocidad `v1` y la altura `y1` que calcula la fase 1.

Responde la **pregunta b** del enunciado: la rapidez máxima `v2` al apagar los motores.

### Fórmulas

```
Distancia restante hasta el apagado:   Δy  = h - y1
Velocidad al apagar los motores:       v2  = √(v1² + 2·a2·Δy)
Duración de la fase 2:                 Δt2 = (v2 - v1) / a2
Tiempo total de vuelo propulsado:      t_total = t1 + Δt2
```

> `a2 = 12,5 m/s²` se toma como aceleración neta del movimiento, como dice el enunciado. Por eso no se le resta la gravedad.

### Resultados con los valores originales

| Dato | Valor |
|---|---|
| Velocidad al desprendimiento (entrada, `v1`) | 2.400,00 m/s |
| Altura al desprendimiento (entrada, `y1`) | 144.000,00 m |
| Distancia restante `Δy` | 16.000,00 m |
| Velocidad de apagado `v2` | **2.481,93 m/s** (≈ 8.935 km/h) |
| Duración de la fase 2 `Δt2` | 6,55 s |
| Tiempo total de vuelo propulsado | 126,55 s |

### Simulación por intervalos

Además del cálculo analítico, el programa muestra cómo evoluciona la nave paso a paso. En cada intervalo `dt` (1 s por defecto, configurable entre 0,1 y 60 s) imprime tiempo, altura, velocidad y fase:

```
Tiempo [s] |     Altura [m] |   Vel. [m/s] | Fase
-----------+----------------+--------------+---------------------
    120.00 |      144000.00 |      2400.00 | Propulsion nucleo    <- desprendimiento SRB
    121.00 |      146406.25 |      2412.50 | Propulsion nucleo
    ...
    126.00 |      158625.00 |      2475.00 | Propulsion nucleo
    126.55 |      160000.00 |      2481.93 | Propulsion nucleo    <- APAGADO (ultimo dt = 0.55 s)
```

Decisiones de implementación:

- **Posición calculada desde el inicio de la fase.** En cada paso se calcula `τ = paso · dt`, y con eso `y(τ) = y1 + v1·τ + ½·a2·τ²` y `v(τ) = v1 + a2·τ`. No se suma `dt` en cada vuelta, porque con `double` el error de redondeo se va acumulando.
- **Ajuste del último intervalo.** Si el siguiente paso supera la duración `Δt2`, se acorta para terminar exactamente en `h`. Con los datos originales, el último paso dura 0,55 s en lugar de 1 s.
- **Control de límites del historial.** Cada intervalo se guarda en el historial de la misión (`agregarEstado`, del integrante 3). Al configurar la fase 2 (`configurarSegundaFase`) se verifica que los intervalos de las fases 2 y 3 entren en el historial (`MAX_ESTADOS` = 1000). Si no entran, el programa sugiere un intervalo mínimo y vuelve a pedir los datos. Así también se evita imprimir millones de filas.
- **Paso de parámetros por punteros.** `simularPropulsion` recibe por puntero la configuración (`const`, solo lectura), el estado de la nave, el historial y los resultados. El estado entra con los datos del desprendimiento y sale con los datos del apagado, que usa la fase 3.

```c
int simularPropulsion(const Configuracion *config, Estado *estado,
                      Historial *hist, ResultadoFase2 *res);
```

### Validaciones de la fase 2

Antes de simular, `validarSegundaFase` verifica que:

- la aceleración `a2` sea positiva;
- la altura de apagado `h` sea **mayor** que la altura de desprendimiento `y1` (si no, la nave ya habría pasado esa altura durante la fase 1);
- el intervalo `dt` esté entre 0,1 y 60 s.

Además, `configurarSegundaFase` ya pide la altura de apagado con `leerRealMayorQue(..., y1, ...)`, así que el usuario no puede ingresar una altura menor que la de desprendimiento. Si eligió los valores originales y la fase 1 personalizada dejó la nave por encima de 160 km, se le avisa y se le piden los datos.

### Lectura segura de números reales

| Función | Uso |
|---|---|
| `leerReal(mensaje, &valor)` | Lee cualquier número real |
| `leerRealMayorQue(mensaje, limite, &valor)` | Exige `valor > limite` (ej.: aceleraciones positivas) |
| `leerRealRango(mensaje, minimo, maximo, &valor)` | Exige `min ≤ valor ≤ max` (ej.: intervalo `dt`) |

Todas devuelven `1` si leyeron un valor válido y `0` si se llegó al **fin de entrada** (Ctrl+D en Linux/Mac, Ctrl+Z + Enter en Windows).

**Por qué no usamos `scanf("%lf")`:** si el usuario escribe una letra, `scanf` no la consume, la deja en el buffer y el programa entra en un bucle infinito. En su lugar:

1. Se lee la línea completa con `fgets`.
2. Si la línea no entró en el buffer, se descarta el resto y se informa el error.
3. Se convierte con `strtod` y se verifica que **no sobren caracteres** (`"12abc"` se rechaza).
4. Se rechazan valores fuera de rango, infinitos y `nan`.
5. Se acepta coma decimal (`12,5`) además de punto (`12.5`).

Ante cualquier entrada inválida se muestra un mensaje y se vuelve a pedir el dato. La lectura de la línea (`leerLinea`) es una función auxiliar compartida con `leerEntero` del integrante 1, para no duplicar código; `ascenso.c` también usa `leerRealMayorQue` para a1 y t1.

### Pruebas realizadas

| Entrada | Resultado esperado |
|---|---|
| Valores originales | v2 = 2.481,93 m/s, Δt2 = 6,55 s, altura final = 160.000 m |
| `abc`, `12abc`, `nan`, `1e999` | "Valor invalido" y se vuelve a pedir |
| `-3` como aceleración | "El valor debe ser mayor que 0.00" |
| `12,5` (coma) | Se acepta como 12.5 |
| Línea de 300 caracteres | "Entrada demasiado larga" |
| `h = 100` (menor que y1) | Error de altura y no se simula |
| `dt = 0.05` | "El valor debe estar entre 0.10 y 60.00" |
| Duración múltiplo exacto de `dt` | Termina sin paso extra de duración cero |
| Fin de entrada (Ctrl+D) | Se detecta y el programa termina de forma ordenada |

---

## Registro de uso de IA (integrante 2)

**Herramienta:** Claude (Anthropic).

**Para qué la usé:**

- Entender qué tareas me correspondían a partir del enunciado y de la división del grupo, y qué datos tenía que acordar con mis compañeros.
- Generar una primera versión de `propulsion.c`, de las funciones de lectura de reales de `validaciones.c`, una propuesta de `simulador.h` / `validaciones.h` y un programa de prueba de mi módulo.

**Consultas realizadas (resumen):**

1. *"Tengo que hacer este trabajo, nos tocó la situación 1, somos tres y soy el integrante 2."* (adjuntando el enunciado y la división de tareas)
2. *"¿Eso me podes dar un ejemplo de como puedo hacer hacer?"* (pidiendo el código de la fase 2 y la lectura de reales)


**Cómo validé y adapté el código:**

- Calculé a mano v2, Δt2 y el tiempo total, y los comparé con la salida del programa y con la tabla de valores esperados de la cátedra.
- Compilé con `gcc -Wall -Wextra -std=c99` sin advertencias.
- Ejecuté el programa de prueba con todas las entradas de la tabla de pruebas.
- [Completar: qué cambié del código generado, por ejemplo nombres de campos o prototipos acordados con el grupo.]
- Revisé y comprendí cada función, en especial el uso de `fgets` + `strtod`, el ajuste del último intervalo y el paso de parámetros por punteros.
