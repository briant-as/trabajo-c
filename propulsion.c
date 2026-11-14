/*
 * propulsion.c
 * Integrante 2 - Fase 2: desde el desprendimiento de los SRB hasta el
 * apagado de motores a la altura h.
 *
 * Modelo físico: MRUV con aceleración neta constante a2 (ya es aceleración
 * neta del movimiento: NO se le vuelve a restar la gravedad).
 *
 *   Δy  = h - y1
 *   v2  = sqrt(v1² + 2·a2·Δy)
 *   Δt2 = (v2 - v1) / a2
 *
 * Posición y velocidad a un tiempo τ desde el desprendimiento (0 <= τ <= Δt2):
 *   y(τ) = y1 + v1·τ + ½·a2·τ²
 *   v(τ) = v1 + a2·τ
 */
#include <stdio.h>
#include <math.h>
#include "simulador.h"

/* Tolerancia para comparar tiempos con double */
#define EPSILON 1e-9

const char *nombre_fase(Fase fase)
{
    switch (fase)
    {
    case FASE_PLATAFORMA:
        return "En plataforma";
    case FASE_ASCENSO_SRB:
        return "Ascenso con SRB";
    case FASE_PROPULSION_NUCLEO:
        return "Propulsion nucleo";
    case FASE_VUELO_LIBRE:
        return "Vuelo sin propulsion";
    case FASE_ALTURA_MAXIMA:
        return "Altura maxima";
    default:
        return "Desconocida";
    }
}

/* Verifica que los datos permitan simular la fase 2.
 * Devuelve 1 si todo es válido; si no, informa cada error y devuelve 0. */
int validar_config_fase2(const Configuracion *cfg, const Estado *desprendimiento)
{
    int valido = 1;

    if (cfg == NULL || desprendimiento == NULL)
    {
        printf("[!] Error interno: datos de la fase 2 inexistentes.\n");
        return 0;
    }
    if (cfg->a2 <= 0.0)
    {
        printf("[!] La aceleracion de la fase 2 debe ser positiva "
               "(ingresada: %.2f m/s2).\n",
               cfg->a2);
        valido = 0;
    }
    if (cfg->h_apagado <= desprendimiento->altura)
    {
        printf("[!] La altura de apagado (%.2f m) debe ser mayor que la "
               "altura de desprendimiento de los SRB (%.2f m).\n",
               cfg->h_apagado, desprendimiento->altura);
        valido = 0;
    }
    if (desprendimiento->velocidad < 0.0)
    {
        printf("[!] La velocidad al desprendimiento no puede ser negativa.\n");
        valido = 0;
    }
    if (cfg->dt < DT_MIN || cfg->dt > DT_MAX)
    {
        printf("[!] El intervalo de simulacion debe estar entre %.2f y %.2f s.\n",
               DT_MIN, DT_MAX);
        valido = 0;
    }
    return valido;
}

/* Cálculo analítico de la fase 2. Supone datos ya validados. */
void calcular_fase2(const Configuracion *cfg, const Estado *desprendimiento,
                    ResultadoFase2 *res)
{
    double v1 = desprendimiento->velocidad;

    res->distancia_restante = cfg->h_apagado - desprendimiento->altura;
    res->v_apagado = sqrt(v1 * v1 + 2.0 * cfg->a2 * res->distancia_restante);
    res->duracion = (res->v_apagado - v1) / cfg->a2;
    res->tiempo_total = desprendimiento->tiempo + res->duracion;
}

static void imprimir_fila(const Estado *e, const char *nota)
{
    printf("%10.2f | %14.2f | %12.2f | %-20s %s\n",
           e->tiempo, e->altura, e->velocidad, nombre_fase(e->fase), nota);
}

/* Simula la fase 2 avanzando de a cfg->dt segundos.
 *
 * Entrada:  *estado = estado de la nave al desprenderse los SRB (integrante 1)
 * Salida:   *estado = estado de la nave al apagar los motores (para el
 *                     integrante 3); *res = resultados analíticos de la fase.
 * hist puede ser NULL si no se quiere guardar historial.
 * Devuelve 1 si la simulación se realizó, 0 si los datos eran inválidos. */
int simular_propulsion(const Configuracion *cfg, Estado *estado,
                       Historial *hist, ResultadoFase2 *res)
{
    Estado inicio;
    Estado actual;
    double tau; /* tiempo transcurrido desde el desprendimiento */
    double tau_anterior = 0.0;
    long paso = 0;
    int ultimo = 0;
    int aviso_lleno = 0;

    if (estado == NULL || res == NULL || !validar_config_fase2(cfg, estado))
    {
        return 0;
    }

    inicio = *estado; /* copia: no perdemos los datos de partida */
    calcular_fase2(cfg, &inicio, res);

    /* Control de límites: la cantidad de intervalos debe entrar en el
     * historial (y evita imprimir millones de filas). */
    if (ceil(res->duracion / cfg->dt - EPSILON) > MAX_ESTADOS)
    {
        /* redondeado hacia arriba a 2 decimales para que alcance */
        double dt_necesario = ceil(res->duracion / MAX_ESTADOS * 100.0) / 100.0;

        printf("[!] Con un intervalo de %.2f s la fase 2 tendria %.0f pasos "
               "(maximo %d).\n",
               cfg->dt, ceil(res->duracion / cfg->dt),
               MAX_ESTADOS);
        if (dt_necesario <= DT_MAX)
        {
            printf("    Use un intervalo de al menos %.2f s.\n", dt_necesario);
        }
        else
        {
            printf("    La fase dura %.2f s: reduzca la altura de apagado o "
                   "aumente la aceleracion.\n",
                   res->duracion);
        }
        return 0;
    }

    printf("\n=== FASE 2: PROPULSION DEL NUCLEO (intervalo %.2f s) ===\n", cfg->dt);
    printf("%10s | %14s | %12s | %-20s\n",
           "Tiempo [s]", "Altura [m]", "Vel. [m/s]", "Fase");
    printf("-----------+----------------+--------------+---------------------\n");

    /* Fila inicial: el instante del desprendimiento (ya registrado por la fase 1) */
    actual = inicio;
    actual.fase = FASE_PROPULSION_NUCLEO;
    imprimir_fila(&actual, "<- desprendimiento SRB");

    while (!ultimo)
    {
        paso++;
        /* tau = paso * dt en vez de sumar dt cada vuelta: evita acumular
         * error de redondeo de los double. */
        tau = paso * cfg->dt;

        /* Ajuste del último intervalo: no pasarse de la altura de apagado */
        if (tau >= res->duracion - EPSILON)
        {
            tau = res->duracion;
            ultimo = 1;
        }

        actual.tiempo = inicio.tiempo + tau;
        if (ultimo)
        {
            /* Valores exactos al apagado (sin error de redondeo) */
            actual.altura = cfg->h_apagado;
            actual.velocidad = res->v_apagado;
        }
        else
        {
            actual.altura = inicio.altura + inicio.velocidad * tau + 0.5 * cfg->a2 * tau * tau;
            actual.velocidad = inicio.velocidad + cfg->a2 * tau;
        }
        actual.fase = FASE_PROPULSION_NUCLEO;

        if (ultimo && (tau - tau_anterior) < cfg->dt - EPSILON)
        {
            char nota[48];
            snprintf(nota, sizeof(nota), "<- APAGADO (ultimo dt = %.2f s)",
                     tau - tau_anterior);
            imprimir_fila(&actual, nota);
        }
        else
        {
            imprimir_fila(&actual, ultimo ? "<- APAGADO" : "");
        }

        if (hist != NULL && !historial_agregar(hist, &actual) && !aviso_lleno)
        {
            printf("[!] Historial lleno (%d estados): los siguientes intervalos "
                   "se muestran pero no se guardan.\n",
                   MAX_ESTADOS);
            aviso_lleno = 1;
        }
        tau_anterior = tau;
    }

    *estado = actual; /* se entrega el estado de apagado */
    return 1;
}

void mostrar_resultados_fase2(const ResultadoFase2 *res)
{
    printf("\n--- Resultados de la fase 2 ---\n");
    printf("Distancia recorrida hasta el apagado : %12.2f m\n", res->distancia_restante);
    printf("Velocidad al apagar los motores (v2) : %12.2f m/s (%.2f km/h)\n",
           res->v_apagado, res->v_apagado * 3.6);
    printf("Duracion de la fase 2                : %12.2f s\n", res->duracion);
    printf("Tiempo total de vuelo propulsado     : %12.2f s\n", res->tiempo_total);
}
