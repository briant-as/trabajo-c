#include <math.h>
#include <stdio.h>
#include "simulacion.h"


double alturaAdicional(double v2, double g) {
    return (v2 * v2) / (2.0 * g);
}

double alturaMaxima(double h, double deltaH) {
    return h + deltaH; 
}

double tiempoSubida(double v2, double g) {
    return v2 / g;   
}

int simularVueloLibre(Historial *hist, double tApagado, double hApagado,
                      double v2, double g, double dt)
{
    double duracion, pasos, t, hMax;
    int i, total;
    Estado e;
    if (!hist || hist->cantidad < 0 || hist->cantidad > MAX_ESTADOS ||
        !isfinite(tApagado) || tApagado < 0 || !isfinite(hApagado) || hApagado < 0 ||
        !isfinite(v2) || v2 <= 0 || !isfinite(g) || g <= 0 ||
        !isfinite(dt) || dt <= 0) return 0;
    duracion = tiempoSubida(v2, g);
    hMax = alturaMaxima(hApagado, alturaAdicional(v2, g));
    pasos = ceil(duracion / dt);
    if (!isfinite(duracion) || duracion <= 0 || !isfinite(hMax) ||
        !isfinite(tApagado + duracion) || !isfinite(pasos) || pasos < 1 ||
        pasos > MAX_ESTADOS - hist->cantidad) return 0;
    total = (int)pasos;
    puts("\n=== FASE 3: VUELO LIBRE ===");
    puts("Tiempo [s] | Altura [m] | Velocidad [m/s]");
    for (i = 1; i <= total; i++) {
        t = i == total ? duracion : i * dt;
        e.tiempo = tApagado + t;
        e.velocidad = i == total ? 0.0 : v2 - g * t;
        e.altura = i == total ? hMax : hApagado + t * (v2 - 0.5 * g * t);
        e.fase = FASE_LIBRE;
        if (!isfinite(e.altura) || !agregarEstado(hist, e)) return 0;
        printf("%10.2f | %12.2f | %12.2f%s\n", e.tiempo, e.altura,
               e.velocidad, i == total ? " <- ALTURA MAXIMA" : "");
    }
    return 1;
}
