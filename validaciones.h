
#ifndef VALIDACIONES_H
#define VALIDACIONES_H

int leerEntero(const char *mensaje, int minimo, int maximo, int *valor);

int leerReal(const char *mensaje, double *valor);
int leerRealMayorQue(const char *mensaje, double limite, double *valor);
int leerRealRango(const char *mensaje, double minimo, double maximo, double *valor);

#endif
