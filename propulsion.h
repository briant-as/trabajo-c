
#ifndef PROPULSION_H
#define PROPULSION_H

#include "simulador.h"

typedef struct
{
    double distanciaRestante;
    double velocidadApagado;
    double duracion;
    double tiempoTotal;
} ResultadoFase2;

int configurarSegundaFase(Configuracion *config, const EstadoNave *desprendimiento,
                          int lugaresLibres);

int validarSegundaFase(const Configuracion *config, const Estado *desprendimiento);

void calcularSegundaFase(const Configuracion *config, const Estado *desprendimiento,
                         ResultadoFase2 *res);

int simularPropulsion(const Configuracion *config, Estado *estado,
                      Historial *hist, ResultadoFase2 *res);

void mostrarResultadosFase2(const ResultadoFase2 *res);

#endif