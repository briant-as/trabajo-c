/*
 * validaciones.c
 * Ingreso seguro de datos por consola.
 *
 * Integrante 2: funciones de lectura de números reales.
 * (El integrante 1 agrega en este mismo archivo leer_entero.)
 *
 * Por qué no usamos scanf("%lf") directamente:
 *   si el usuario escribe una letra, scanf no la consume, queda en el buffer
 *   y el programa entra en un bucle infinito. Por eso leemos la línea entera
 *   con fgets y la convertimos con strtod, revisando que no sobre nada.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "validaciones.h"

#define TAM_LINEA 128

/* Descarta lo que quedó en stdin hasta el fin de línea (cuando la línea
 * era más larga que el buffer). */
static void descartar_resto_linea(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/* Intenta convertir el texto a double.
 * Devuelve 1 si el texto es un número real válido y completo, 0 si no. */
static int convertir_real(char *texto, double *valor)
{
    char *fin;
    double numero;
    size_t i;

    /* Aceptamos coma decimal (costumbre en Argentina): "12,5" -> "12.5" */
    for (i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] == ',')
        {
            texto[i] = '.';
        }
    }

    errno = 0;
    numero = strtod(texto, &fin);

    if (fin == texto)
    {
        return 0; /* no había ningún número */
    }
    while (isspace((unsigned char)*fin))
    {
        fin++; /* se permiten espacios al final */
    }
    if (*fin != '\0')
    {
        return 0; /* sobran caracteres: "12abc" */
    }
    if (errno == ERANGE || !isfinite(numero))
    {
        return 0; /* fuera de rango, inf o nan */
    }

    *valor = numero;
    return 1;
}

int leer_real(const char *mensaje, double *valor)
{
    char linea[TAM_LINEA];

    while (1)
    {
        printf("%s", mensaje);
        fflush(stdout);

        if (fgets(linea, sizeof(linea), stdin) == NULL)
        {
            printf("\n[!] Fin de entrada detectado.\n");
            return 0; /* EOF: el que llama decide qué hacer */
        }

        /* Si no entró el '\n', la línea era demasiado larga */
        if (strchr(linea, '\n') == NULL && !feof(stdin))
        {
            descartar_resto_linea();
            printf("[!] Entrada demasiado larga. Intente nuevamente.\n");
            continue;
        }

        if (convertir_real(linea, valor))
        {
            return 1;
        }
        printf("[!] Valor invalido: ingrese un numero real "
               "(ej: 12.5 o 12,5, sin separador de miles).\n");
    }
}

int leer_real_mayor_que(const char *mensaje, double limite, double *valor)
{
    double numero;

    while (leer_real(mensaje, &numero))
    {
        if (numero > limite)
        {
            *valor = numero;
            return 1;
        }
        printf("[!] El valor debe ser mayor que %.2f.\n", limite);
    }
    return 0; /* EOF */
}

int leer_real_rango(const char *mensaje, double min, double max, double *valor)
{
    double numero;

    while (leer_real(mensaje, &numero))
    {
        if (numero >= min && numero <= max)
        {
            *valor = numero;
            return 1;
        }
        printf("[!] El valor debe estar entre %.2f y %.2f.\n", min, max);
    }
    return 0; /* EOF */
}
