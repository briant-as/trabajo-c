
#include <math.h>
#include <stdio.h>
#include "propulsion.h"
#include "reportes.h"
#include "validaciones.h"
#include "vuelo_libre.h"

#define EPSILON 1e-9

static double cantidadPasos(double duracion, double dt)
{
    return ceil(duracion / dt - EPSILON);
}

static Estado estadoDesprendimiento(const Configuracion *config,
                                    const EstadoNave *nave)
{
    Estado e;

    e.tiempo = config->tiempo;
    e.altura = nave->altura;
    e.velocidad = nave->velocidad;
    e.fase = FASE_NUCLEO;
    return e;
}

static int entraEnHistorial(const Configuracion *config, const EstadoNave *nave,
                            int lugaresLibres)
{
    Estado inicio = estadoDesprendimiento(config, nave);
    ResultadoFase2 res;
    double tSubida, pasos, intervaloMinimo;

    calcularSegundaFase(config, &inicio, &res);
    tSubida = tiempoSubida(res.velocidadApagado, GRAVEDAD);

    pasos = cantidadPasos(res.duracion, config->intervalo) + cantidadPasos(tSubida, config->intervalo) + 1;
    if (pasos <= lugaresLibres)
    {
        return 1;
    }

    printf("Con un intervalo de %.2f s la mision necesita %.0f estados y el "
           "historial admite %d.\n",
           config->intervalo, pasos, lugaresLibres);
    intervaloMinimo = ceil((res.duracion + tSubida) / (lugaresLibres - 3) * 100.0) / 100.0;
    if (intervaloMinimo <= INTERVALO_MAX)
    {
        printf("Use un intervalo de al menos %.2f s.\n", intervaloMinimo);
    }
    else
    {
        puts("Reduzca la altura de apagado o la aceleracion de la fase 2.");
    }
    return 0;
}

int configurarSegundaFase(Configuracion *config, const EstadoNave *desprendimiento,
                          int lugaresLibres)
{
    int opcion;
    char mensajeAltura[96];

    puts("\n--- CONFIGURACION DE LA SEGUNDA FASE ---");
    printf("1. Usar valores originales: a2 = %.1f m/s^2; h = %.1f m; "
           "intervalo = %.1f s\n",
           ACELERACION2_ORIGINAL,
           ALTURA_APAGADO_ORIGINAL, INTERVALO_ORIGINAL);
    puts("2. Ingresar otros valores");

    if (!leerEntero("Seleccione una opcion: ", 1, 2, &opcion))
    {
        return 0;
    }

    if (opcion == 1)
    {
        config->aceleracion2 = ACELERACION2_ORIGINAL;
        config->alturaApagado = ALTURA_APAGADO_ORIGINAL;
        config->intervalo = INTERVALO_ORIGINAL;

        if (config->alturaApagado <= desprendimiento->altura)
        {
            printf("La altura de apagado original (%.2f m) no supera la altura "
                   "de desprendimiento (%.2f m).\nIngrese los valores de la "
                   "segunda fase.\n",
                   config->alturaApagado,
                   desprendimiento->altura);
        }
        else if (entraEnHistorial(config, desprendimiento, lugaresLibres))
        {
            return 1;
        }
    }

    snprintf(mensajeAltura, sizeof mensajeAltura,
             "Altura de apagado h (m, mayor que %.2f): ", desprendimiento->altura);

    while (1)
    {
        if (!leerRealMayorQue("Aceleracion a2 (m/s^2, > 0): ", 0.0,
                              &config->aceleracion2) ||
            !leerRealMayorQue(mensajeAltura, desprendimiento->altura,
                              &config->alturaApagado) ||
            !leerRealRango("Intervalo de simulacion (s, entre 0.1 y 60): ",
                           INTERVALO_MIN, INTERVALO_MAX, &config->intervalo))
        {
            return 0;
        }
        if (entraEnHistorial(config, desprendimiento, lugaresLibres))
        {
            return 1;
        }
        puts("Ingrese nuevamente los datos de la segunda fase.");
    }
}

int validarSegundaFase(const Configuracion *config, const Estado *desprendimiento)
{
    int valido = 1;

    if (config == NULL || desprendimiento == NULL)
    {
        puts("Error interno: datos de la fase 2 inexistentes.");
        return 0;
    }
    if (config->aceleracion2 <= 0.0)
    {
        printf("La aceleracion de la fase 2 debe ser positiva "
               "(ingresada: %.2f m/s^2).\n",
               config->aceleracion2);
        valido = 0;
    }
    if (config->alturaApagado <= desprendimiento->altura)
    {
        printf("La altura de apagado (%.2f m) debe ser mayor que la altura "
               "de desprendimiento (%.2f m).\n",
               config->alturaApagado, desprendimiento->altura);
        valido = 0;
    }
    if (desprendimiento->velocidad < 0.0)
    {
        puts("La velocidad al desprendimiento no puede ser negativa.");
        valido = 0;
    }
    if (config->intervalo < INTERVALO_MIN || config->intervalo > INTERVALO_MAX)
    {
        printf("El intervalo debe estar entre %.2f y %.2f s.\n",
               INTERVALO_MIN, INTERVALO_MAX);
        valido = 0;
    }
    return valido;
}

void calcularSegundaFase(const Configuracion *config, const Estado *desprendimiento,
                         ResultadoFase2 *res)
{
    double v1 = desprendimiento->velocidad;

    res->distanciaRestante = config->alturaApagado - desprendimiento->altura;
    res->velocidadApagado = sqrt(v1 * v1 + 2.0 * config->aceleracion2 * res->distanciaRestante);
    res->duracion = (res->velocidadApagado - v1) / config->aceleracion2;
    res->tiempoTotal = desprendimiento->tiempo + res->duracion;
}

static void imprimirFila(const Estado *e, const char *nota)
{
    printf("%10.2f | %14.2f | %12.2f | %-18s %s\n",
           e->tiempo, e->altura, e->velocidad, "Propulsion nucleo", nota);
}

int simularPropulsion(const Configuracion *config, Estado *estado,
                      Historial *hist, ResultadoFase2 *res)
{
    Estado inicio;
    Estado actual;
    double tau;
    double tauAnterior = 0.0;
    long paso = 0;
    int ultimo = 0;
    char nota[48];

    if (estado == NULL || res == NULL || hist == NULL ||
        !validarSegundaFase(config, estado))
    {
        return 0;
    }

    inicio = *estado;
    calcularSegundaFase(config, &inicio, res);

    if (cantidadPasos(res->duracion, config->intervalo) > MAX_ESTADOS - hist->cantidad)
    {
        puts("No hay lugar en el historial para todos los intervalos de la fase 2.");
        return 0;
    }

    printf("\n=== FASE 2: PROPULSION DEL NUCLEO (intervalo %.2f s) ===\n",
           config->intervalo);
    printf("%10s | %14s | %12s | %-18s\n",
           "Tiempo [s]", "Altura [m]", "Vel. [m/s]", "Fase");
    puts("-----------+----------------+--------------+-------------------");

    actual = inicio;
    actual.fase = FASE_NUCLEO;
    imprimirFila(&actual, "<- desprendimiento SRB");

    while (!ultimo)
    {
        paso++;

        tau = paso * config->intervalo;

        if (tau >= res->duracion - EPSILON)
        {
            tau = res->duracion;
            ultimo = 1;
        }

        actual.tiempo = inicio.tiempo + tau;
        if (ultimo)
        {

            actual.altura = config->alturaApagado;
            actual.velocidad = res->velocidadApagado;
        }
        else
        {
            actual.altura = inicio.altura + inicio.velocidad * tau + 0.5 * config->aceleracion2 * tau * tau;
            actual.velocidad = inicio.velocidad + config->aceleracion2 * tau;
        }

        if (ultimo && (tau - tauAnterior) < config->intervalo - EPSILON)
        {
            snprintf(nota, sizeof nota, "<- APAGADO (ultimo dt = %.2f s)",
                     tau - tauAnterior);
            imprimirFila(&actual, nota);
        }
        else
        {
            imprimirFila(&actual, ultimo ? "<- APAGADO" : "");
        }

        agregarEstado(hist, actual);
        tauAnterior = tau;
    }

    *estado = actual;
    return 1;
}

void mostrarResultadosFase2(const ResultadoFase2 *res)
{
    puts("\n--- RESULTADOS DE LA SEGUNDA FASE ---");
    printf("Distancia recorrida hasta el apagado: %12.2f m\n", res->distanciaRestante);
    printf("Velocidad al apagar los motores (v2): %12.2f m/s (%.2f km/h)\n",
           res->velocidadApagado, res->velocidadApagado * 3.6);
    printf("Duracion de la segunda fase:          %12.2f s\n", res->duracion);
    printf("Tiempo total de vuelo propulsado:     %12.2f s\n", res->tiempoTotal);
}
