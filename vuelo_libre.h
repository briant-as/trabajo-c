#ifndef VUELO_LIBRE_H
#define VUELO_LIBRE_H

#include "simulador.h"

double alturaAdicional(double v2, double g);
double alturaMaxima(double h, double deltaH);
double tiempoSubida(double v2, double g);
int simularVueloLibre(Historial *hist, double tApagado, double hApagado,
                      double v2, double g, double dt);

#endif