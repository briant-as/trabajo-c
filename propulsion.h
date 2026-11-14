/*
 * propulsion.h
 * Integrante 2 - Fase 2: desde el desprendimiento de los SRB hasta el
 * apagado de los motores a la altura h.
 */
#ifndef PROPULSION_H
#define PROPULSION_H

#include "simulador.h"

/* Resultados analiticos de la fase 2 */
typedef struct {
    double distanciaRestante;  /* dy  = h - y1                 [m]   */
    double velocidadApagado;   /* v2  = sqrt(v1^2 + 2*a2*dy)   [m/s] */
    double duracion;           /* dt2 = (v2 - v1) / a2         [s]   */
    double tiempoTotal;        /* t1 + dt2: vuelo propulsado   [s]   */
} ResultadoFase2;

/* Pide a2, h e intervalo (o usa los originales), validando que h > y1 y
 * que todos los intervalos de las fases 2 y 3 entren en 'lugaresLibres'
 * posiciones del historial. Devuelve 1 si obtuvo los datos, 0 si EOF. */
int configurarSegundaFase(Configuracion *config, const EstadoNave *desprendimiento,
                          int lugaresLibres);

/* Devuelve 1 si los datos permiten simular la fase 2; si no, informa y 0. */
int validarSegundaFase(const Configuracion *config, const Estado *desprendimiento);

void calcularSegundaFase(const Configuracion *config, const Estado *desprendimiento,
                         ResultadoFase2 *res);

/* Simula la fase 2 por intervalos, mostrando una tabla y guardando cada
 * intervalo en el historial. *estado entra con el desprendimiento y sale
 * con el apagado. Devuelve 1 si simulo, 0 si los datos eran invalidos. */
int simularPropulsion(const Configuracion *config, Estado *estado,
                      Historial *hist, ResultadoFase2 *res);

void mostrarResultadosFase2(const ResultadoFase2 *res);

#endif /* PROPULSION_H */
