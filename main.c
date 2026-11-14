#include <stdio.h>
#include "simulacion.h"
#include "validaciones.h"

int main(void)
{
    int opcion, completa = 0;
    Configuracion config = {0};
    EstadoNave nave = {0};
    Estado estado;
    Historial historial;
    ResultadoFase2 resultado;
    double hMax, tSubida;
    iniciarHistorial(&historial);
    while (1) {
        puts("\n=== MISION ARTEMIS ===");
        puts("1. Iniciar mision\n2. Instrucciones\n3. Historial\n4. Salir");
        if (!leerEntero("Opcion: ", 1, 4, &opcion)) break;
        switch (opcion) {
        case 1:
            completa = 0;
            iniciarHistorial(&historial);
            if (!configurarPrimeraFase(&config)) return 0;
            if (!calcularPrimeraFase(&config, &nave)) {
                puts("Calculo fuera de rango. Reinicie con otros valores.");
                break;
            }
            mostrarEstado(&config, &nave);
            estado = (Estado){0, 0, 0, FASE_PLATAFORMA};
            if (!agregarEstado(&historial, estado)) break;
            estado = (Estado){config.tiempo, nave.altura, nave.velocidad, FASE_SRB};
            if (!agregarEstado(&historial, estado)) break;
            if (!configurarSegundaFase(&config, &nave, MAX_ESTADOS - historial.cantidad)) return 0;
            if (!simularPropulsion(&config, &estado, &historial, &resultado)) {
                puts("No se pudo completar la fase 2. Historial parcial.");
                break;
            }
            mostrarResultadosFase2(&resultado);
            if (!simularVueloLibre(&historial, estado.tiempo, estado.altura,
                                  estado.velocidad, GRAVEDAD, config.intervalo)) {
                puts("No se pudo completar el vuelo libre. Historial parcial.");
                break;
            }
            tSubida = tiempoSubida(estado.velocidad, GRAVEDAD);
            hMax = alturaMaxima(estado.altura, alturaAdicional(estado.velocidad, GRAVEDAD));
            mostrarReporteFinal(nave.velocidad, nave.altura, estado.velocidad,
                                estado.altura, hMax, tSubida, nave.aceleracionG);
            completa = 1;
            puts("MISION COMPLETA: altura maxima alcanzada.");
            break;
        case 2: mostrarInstrucciones(); break;
        case 3:
            if (historial.cantidad && !completa) puts("Aviso: historial de una mision incompleta.");
            mostrarHistorial(&historial);
            break;
        case 4: return 0;
        }
    }
    puts("Fin de entrada. Programa finalizado.");
    return 0;
}
