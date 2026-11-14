/*
 * validaciones.h
 * Ingreso seguro de datos por consola.
 *
 * Convencion para TODAS las funciones de lectura:
 *   devuelven 1 si se leyo un valor valido (guardado en *valor)
 *   devuelven 0 si se llego al fin de entrada (EOF: Ctrl+D / Ctrl+Z)
 * Ante una entrada invalida informan el error y vuelven a pedir el dato.
 */
#ifndef VALIDACIONES_H
#define VALIDACIONES_H

/* Integrante 1: enteros (opciones de menu) */
int leerEntero(const char *mensaje, int minimo, int maximo, int *valor);

/* Integrante 2: reales (datos fisicos) */
int leerReal(const char *mensaje, double *valor);
int leerRealMayorQue(const char *mensaje, double limite, double *valor);
int leerRealRango(const char *mensaje, double minimo, double maximo, double *valor);

#endif /* VALIDACIONES_H */
