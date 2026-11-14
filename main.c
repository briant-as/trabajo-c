#include <stdio.h>
#include "simulacion.h"
#include "validaciones_fase_uno.h"
#include "interfaz.h"

int main(void)
{
    int opcion;
    Configuracion config;
    EstadoNave estado;

    while (1)
    {
        puts("\n=== MISION SLS / ORION ===");
        puts("1. Iniciar simulacion");
        puts("2. Consultar instrucciones");
        puts("3. Salir");

        if (!leerEntero("Seleccione una opcion: ", 1, 3, &opcion))
        {
            puts("\nFin de la entrada. Programa finalizado.");
            return 0;
        }

        switch (opcion)
        {
        case 1:
            if (!configurarPrimeraFase(&config))
            {
                puts("\nFin de la entrada. Programa finalizado.");
                return 0;
            }
            if (calcularPrimeraFase(&config, &estado))
            {
                mostrarEstado(&config, &estado);
            }
            else
            {
                puts("Los valores exceden la precision o el rango de calculo.");
                puts("Vuelva a iniciar con valores dentro del rango de calculo.");
            }
            break;
        case 2:
            mostrarInstrucciones();
            break;
        case 3:
            puts("Programa finalizado.");
            return 0;
        }
    }
}
