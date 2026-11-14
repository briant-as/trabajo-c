#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulacion.h"
#include "validaciones_fase_uno.h"

int configurarPrimeraFase(Configuracion *config)
{
    int opcion;

    puts("\n--- CONFIGURACION DE LA PRIMERA FASE ---");
    puts("1. Usar valores originales: a1 = 20.0 m/s^2; t1 = 120.0 s");
    puts("2. Ingresar otros valores");

    if (!leerEntero("Seleccione una opcion: ", 1, 2, &opcion))
    {
        return 0;
    }

    if (opcion == 1)
    {
        config->aceleracion = ACELERACION_ORIGINAL;
        config->tiempo = TIEMPO_ORIGINAL;
        return 1;
    }

    if (!leerRealPositivo("Aceleracion a1 (m/s^2, > 0): ",
                          &config->aceleracion))
    {
        return 0;
    }
    return leerRealPositivo("Tiempo hasta el desprendimiento (s, > 0): ",
                            &config->tiempo);
}

int calcularPrimeraFase(const Configuracion *config, EstadoNave *estado)
{
    EstadoNave resultado;

    if (!isfinite(config->aceleracion) || config->aceleracion <= 0.0 ||
        !isfinite(config->tiempo) || config->tiempo <= 0.0)
    {
        return 0;
    }

    /* MRUV con v0 = 0 e y0 = 0. La aceleracion ya es la resultante. */
    resultado.velocidad = config->aceleracion * config->tiempo;
    resultado.altura = (0.5 * resultado.velocidad) * config->tiempo;
    resultado.aceleracionG = config->aceleracion / GRAVEDAD;

    if (!isfinite(resultado.velocidad) || resultado.velocidad <= 0.0 ||
        !isfinite(resultado.altura) || resultado.altura <= 0.0 ||
        !isfinite(resultado.aceleracionG) || resultado.aceleracionG <= 0.0)
    {
        return 0;
    }

    /* Solo guardar el estado cuando todos los resultados son validos. */
    *estado = resultado;
    return 1;
}

void mostrarEstado(const Configuracion *config, const EstadoNave *estado)
{
    puts("\n--- ESTADO AL DESPRENDER LOS SRB ---");
    printf("Tiempo transcurrido:       %.2f s\n", config->tiempo);
    printf("Aceleracion de la fase:    %.2f m/s^2\n", config->aceleracion);
    printf("Velocidad de la nave:      %.2f m/s (hacia arriba)\n",
           estado->velocidad);
    printf("Altitud sobre plataforma: %.2f m (%.2f km)\n",
           estado->altura, estado->altura / 1000.0);
    printf("Aceleracion expresada en g: %.2f g\n", estado->aceleracionG);
    puts("Estado: SRB desprendidos. Primera fase completada.");
}
