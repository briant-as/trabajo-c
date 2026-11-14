#ifndef SIMULACION_H
#define SIMULACION_H

#define GRAVEDAD 9.80
#define ACELERACION_ORIGINAL 20.0
#define TIEMPO_ORIGINAL 120.0

typedef struct
{
    double aceleracion;
    double tiempo;
    double aceleracion2;
    double alturaApagado;
    double intervalo;
} Configuracion;

typedef struct
{
    double velocidad;
    double altura;
    double aceleracionG;
} EstadoNave;

/* Devuelve 1 si obtiene los datos; 0 si termina la entrada. */
int configurarPrimeraFase(Configuracion *config);

/* Devuelve 0 si los datos o resultados quedan fuera del rango permitido. */
int calcularPrimeraFase(const Configuracion *config, EstadoNave *estado);
void mostrarEstado(const Configuracion *config, const EstadoNave *estado);

#define ACELERACION2_ORIGINAL 12.5
#define ALTURA_APAGADO_ORIGINAL 160000.0
#define INTERVALO_ORIGINAL 1.0
#define INTERVALO_MIN 0.1
#define INTERVALO_MAX 60.0
#define MAX_ESTADOS 1000
#define FASE_PLATAFORMA 0
#define FASE_SRB 1
#define FASE_NUCLEO 2
#define FASE_LIBRE 3

typedef struct {
    double tiempo, altura, velocidad;
    int fase;
} Estado;
typedef struct {
    Estado estados[MAX_ESTADOS];
    int cantidad;
} Historial;


/*
 * propulsion.h
 * Integrante 2 - Fase 2: desde el desprendimiento de los SRB hasta el
 * apagado de los motores a la altura h.
 */


/* Resultados analiticos de la fase 2 */
typedef struct {
    double distanciaRestante;  /* dy  = h - y1                 [m]   */
    double velocidadApagado;   /* v2  = sqrt(v1^2 + 2*a2*dy)   [m/s] */
    double duracion;           /* dt2 = (v2 - v1) / a2         [s]   */
    double tiempoTotal;        /* t1 + dt2: vuelo propulsado   [s]   */
} ResultadoFase2;

/* Pide a2, h e intervalo (o usa los originales), validando que h > y1 y
 * que todos los intervalos de las fases 2 y 3 entren en 'lugaresLibres'
 * posiciones del historial. Devuelve 1 si obtuvo los datos, 0 si EOF. */
int configurarSegundaFase(Configuracion *config, const EstadoNave *desprendimiento,
                          int lugaresLibres);

/* Devuelve 1 si los datos permiten simular la fase 2; si no, informa y 0. */
int validarSegundaFase(const Configuracion *config, const Estado *desprendimiento);

void calcularSegundaFase(const Configuracion *config, const Estado *desprendimiento,
                         ResultadoFase2 *res);

/* Simula la fase 2 por intervalos, mostrando una tabla y guardando cada
 * intervalo en el historial. *estado entra con el desprendimiento y sale
 * con el apagado. Devuelve 1 si simulo, 0 si los datos eran invalidos. */
int simularPropulsion(const Configuracion *config, Estado *estado,
                      Historial *hist, ResultadoFase2 *res);

void mostrarResultadosFase2(const ResultadoFase2 *res);




void iniciarHistorial(Historial *hist);
int agregarEstado(Historial *hist, Estado e);
void mostrarHistorial(const Historial *hist);
void mostrarReporteFinal(double v1, double y1, double v2,
double hApagado, double hMax, double tSubida, double aceleracionG);



double alturaAdicional(double v2, double g);
double alturaMaxima(double h, double deltaH);
double tiempoSubida(double v2, double g);
int simularVueloLibre(Historial *hist, double tApagado, double hApagado,
                      double v2, double g, double dt);


void mostrarInstrucciones(void);
#endif
