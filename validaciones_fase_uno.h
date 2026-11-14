#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Devuelve 1 si obtiene un entero valido; 0 si termina la entrada. */
int leerEntero(const char *mensaje, int minimo, int maximo, int *valor);

int leerRealPositivo(const char *mensaje, double *valor);

#endif