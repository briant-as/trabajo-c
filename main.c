#include <stdio.h>
#include "simulacion.h"

static void mostrarInstrucciones(void)
{
    puts("\n--- INSTRUCCIONES ---");
    puts("Esta simulacion calcula la primera fase del ascenso del SLS.");
    puts("La nave parte del reposo y desde una altura de cero metros.");
    puts("1. Inicie la simulacion desde el menu principal.");
    puts("2. Elija los valores originales o ingrese aceleracion y tiempo.");
    puts("3. Ingrese aceleracion en m/s^2 y tiempo en segundos, ambos > 0.");
    puts("   Para decimales puede usar punto o coma, sin separar miles.");
    puts("4. Se mostrara el estado de la nave al desprender los SRB.");
    puts("Se usa aceleracion constante y gravedad g = 9.80 m/s^2.");
    puts("Formulas: v1 = a1*t1; y1 = 0.5*a1*t1*t1; a en g = a1/g.");
    puts("El programa vuelve al menu despues de cada simulacion.");
}

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
