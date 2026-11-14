#include <stdio.h>
#include "simulacion.h"

void iniciarHistorial(Historial *hist) {
    hist->cantidad = 0;
}

int agregarEstado(Historial *hist, Estado e) {
    if (!hist || hist->cantidad < 0 || hist->cantidad >= MAX_ESTADOS) {
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
                         double hApagado, double hMax, double tSubida, double aceleracionG) {
    printf("\n============ REPORTE FINAL DE MISION ============\n");
    printf(" Velocidad al desacople (v1):  %.2f m/s\n", v1);
    printf(" Altura al desacople (y1):    %.2f m\n", y1);
    printf(" Velocidad al apagado (v2):   %.2f m/s\n", v2);
    printf(" Altura de apagado:           %.2f m\n", hApagado);
    printf(" Altitud maxima:              %.2f m\n", hMax);
    printf(" Tiempo de subida sin motores: %.2f s\n", tSubida);
    printf(" Aceleracion primera fase: %.2f g\n", aceleracionG);
    printf("=================================================\n");
}
void mostrarInstrucciones(void)
{
    puts("Simulador Artemis: ascenso vertical por tres fases.");
    puts("1: SRB; 2: nucleo; 3: vuelo libre hasta velocidad cero.");
    puts("Se parte del reposo y de altura cero. Se omite el aire.");
    puts("Aceleraciones netas constantes; gravedad fija 9.80 m/s^2.");
    puts("Ingrese metros, segundos y m/s^2; punto o coma decimal.");
    puts("El intervalo (0.1 a 60 s) se aplica a las fases 2 y 3.");
    puts("La fase 1 registra plataforma y desprendimiento.");
    puts("La altura de apagado debe superar la de desprendimiento.");
    puts("Historial: ultima mision, hasta 1000 estados, sin guardar en disco.");
    puts("Una mision completa termina en la altura maxima, antes del descenso.");
}
