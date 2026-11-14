#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulacion.h"

int leerEntero(const char *mensaje, int minimo, int maximo, int *valor)
{
    char linea[128];
    char *fin;
    long numero;
    int caracter;

    while (1)
    {
        printf("%s", mensaje);
        fflush(stdout);

        /* fgets permite revisar toda la respuesta antes de aceptarla. */
        if (fgets(linea, sizeof linea, stdin) == NULL)
        {
            return 0;
        }

        if (strchr(linea, '\n') == NULL && !feof(stdin))
        {
            /* Descartar el resto evita que afecte la proxima lectura. */
            while ((caracter = getchar()) != '\n' && caracter != EOF)
            {
            }
            puts("Entrada demasiado larga. Intente nuevamente.");
            continue;
        }

        errno = 0;
        numero = strtol(linea, &fin, 10);

        if (fin == linea || errno == ERANGE)
        {
            puts("Entrada invalida: escriba un numero entero.");
            continue;
        }

        /* Se permiten espacios finales, pero no texto ni decimales. */
        while (isspace((unsigned char)*fin))
        {
            fin++;
        }

        if (*fin != '\0' || numero < INT_MIN || numero > INT_MAX)
        {
            puts("Entrada invalida: escriba un numero entero.");
            continue;
        }

        if (numero < minimo || numero > maximo)
        {
            printf("Opcion invalida: ingrese un valor entre %d y %d.\n",
                   minimo, maximo);
            continue;
        }

        *valor = (int)numero;
        return 1;
    }
}
