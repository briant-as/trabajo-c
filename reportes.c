#include <stdio.h>
#include "reportes.h"

void iniciarHistorial(Historial *hist) {
    hist->cantidad = 0;
}

int agregarEstado(Historial *hist, Estado e) {
    if (hist->cantidad >= MAX_ESTADOS) {
        return 0;   
    }

    hist->estados[hist->cantidad] = e;
    hist->cantidad++;
    return 1;
}

void mostrarHistorial(const Historial *hist) {
    int i;

    if (hist->cantidad == 0) {
        printf("No hay estados guardados.\n");
        return;
    }

    printf("\n=========== HISTORIAL DE LA MISION ===========\n");
    printf("%-4s %-10s %-14s %-12s %-5s\n",
           "N", "Tiempo(s)", "Altura(m)", "Veloc(m/s)", "Fase");
    printf("----------------------------------------------\n");

    for (i = 0; i < hist->cantidad; i++) {
        printf("%-4d %-10.2f %-14.2f %-12.2f %-5d\n",
               i + 1,
               hist->estados[i].tiempo,
               hist->estados[i].altura,
               hist->estados[i].velocidad,
               hist->estados[i].fase);
    }
    printf("==============================================\n");
}

void mostrarReporteFinal(double v1, double y1, double v2,
                         double hApagado, double hMax, double tSubida) {
    printf("\n============ REPORTE FINAL DE MISION ============\n");
    printf(" Velocidad al desacople (v1):  %.2f m/s\n", v1);
    printf(" Altura al desacople (y1):    %.2f m\n", y1);
    printf(" Velocidad al apagado (v2):   %.2f m/s\n", v2);
    printf(" Altura de apagado:           %.2f m\n", hApagado);
    printf(" Altitud maxima:              %.2f m\n", hMax);
    printf(" Tiempo de subida sin motores: %.2f s\n", tSubida);
    printf("=================================================\n");
}