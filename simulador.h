#ifndef SIMULADOR_H
#define SIMULADOR_H

#define MAX_ESTADOS 1000

typedef struct {
    double tiempo;     
    double altura;    
    double velocidad; 
    int fase;          
} Estado;

typedef struct {
    Estado estados[MAX_ESTADOS];
    int cantidad;
} Historial;

#endif