
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validaciones.h"

#define TAM_LINEA 128

static int leerLinea(const char *mensaje, char *linea, int tam)
{
    int caracter;

    while (1)
    {
        printf("%s", mensaje);
        fflush(stdout);

        if (fgets(linea, tam, stdin) == NULL)
        {
            return 0;
        }
        if (strchr(linea, '\n') == NULL && !feof(stdin))
        {

            while ((caracter = getchar()) != '\n' && caracter != EOF)
            {
            }
            puts("Entrada demasiado larga. Intente nuevamente.");
            continue;
        }
        return 1;
    }
}

static int soloEspaciosAlFinal(const char *fin)
{
    while (isspace((unsigned char)*fin))
    {
        fin++;
    }
    return *fin == '\0';
}

int leerEntero(const char *mensaje, int minimo, int maximo, int *valor)
{
    char linea[TAM_LINEA];
    char *fin;
    long numero;

    while (leerLinea(mensaje, linea, sizeof linea))
    {
        errno = 0;
        numero = strtol(linea, &fin, 10);

        if (fin == linea || errno == ERANGE || !soloEspaciosAlFinal(fin) ||
            numero < INT_MIN || numero > INT_MAX)
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
    return 0;
}

int leerReal(const char *mensaje, double *valor)
{
    char linea[TAM_LINEA];
    char *fin;
    double numero;
    int i;

    while (leerLinea(mensaje, linea, sizeof linea))
    {

        for (i = 0; linea[i] != '\0'; i++)
        {
            if (linea[i] == ',')
            {
                linea[i] = '.';
            }
        }

        errno = 0;
        numero = strtod(linea, &fin);

        if (fin == linea || errno == ERANGE || !soloEspaciosAlFinal(fin) ||
            !isfinite(numero))
        {
            puts("Entrada invalida: escriba un numero real "
                 "(ej: 12.5 o 12,5, sin separar miles).");
            continue;
        }
        *valor = numero;
        return 1;
    }
    return 0;
}

int leerRealMayorQue(const char *mensaje, double limite, double *valor)
{
    double numero;

    while (leerReal(mensaje, &numero))
    {
        if (numero > limite)
        {
            *valor = numero;
            return 1;
        }
        printf("Entrada invalida: el valor debe ser mayor que %.2f.\n", limite);
    }
    return 0;
}

int leerRealRango(const char *mensaje, double minimo, double maximo, double *valor)
{
    double numero;

    while (leerReal(mensaje, &numero))
    {
        if (numero >= minimo && numero <= maximo)
        {
            *valor = numero;
            return 1;
        }
        printf("Entrada invalida: el valor debe estar entre %.2f y %.2f.\n",
               minimo, maximo);
    }
    return 0;
}
