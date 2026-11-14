#ifndef SIMULACION_H
#define SIMULACION_H

#define GRAVEDAD 9.80
#define ACELERACION_ORIGINAL 20.0
#define TIEMPO_ORIGINAL 120.0

typedef struct
{
    double aceleracion;
    double tiempo;
} Configuracion;

typedef struct
{
    double velocidad;
    double altura;
    double aceleracionG;
} EstadoNave;

/* Devuelve 1 si obtiene un entero valido; 0 si termina la entrada. */
int leerEntero(const char *mensaje, int minimo, int maximo, int *valor);

/* Devuelve 1 si obtiene los datos; 0 si termina la entrada. */
int configurarPrimeraFase(Configuracion *config);

/* Devuelve 0 si los datos o resultados quedan fuera del rango permitido. */
int calcularPrimeraFase(const Configuracion *config, EstadoNave *estado);
void mostrarEstado(const Configuracion *config, const EstadoNave *estado);

#endif
