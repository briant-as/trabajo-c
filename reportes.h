#ifndef REPORTES_H
#define REPORTES_H

#include "simulador.h"

void iniciarHistorial(Historial *hist);
int agregarEstado(Historial *hist, Estado e);
void mostrarHistorial(const Historial *hist);
void mostrarReporteFinal(double v1, double y1, double v2,
double hApagado, double hMax, double tSubida);
#endif