#include "vuelo_libre.h"
#include "reportes.h"

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
                      double v2, double g, double dt) {
    double tSubida, t;
    Estado e;

    if (v2 <= 0.0 || g <= 0.0 || dt <= 0.0) {
        return 0;   
    }

    tSubida = tiempoSubida(v2, g);
    t = 0.0;

    while (t < tSubida) {
        t += dt;
        if (t > tSubida) {
            t = tSubida;   
        }

        e.tiempo = tApagado + t;
        e.velocidad = v2 - g * t;
        if (e.velocidad < 0.0) {
            e.velocidad = 0.0;   
        }
        e.altura = hApagado + v2 * t - 0.5 * g * t * t;
        e.fase = 3;

        if (!agregarEstado(hist, e)) {
            return 0;  
        }
    }
    return 1;
}