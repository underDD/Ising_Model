#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define L 16
#define V L*L
#define N_DATA 10000
#define N_Inter 100
#define fran rand()/((double)RAND_MAX+1)
#define NormRAnu (2.3283063671E-10F)

#define ACEPTANCIA
#define TERMALIZACION
//#define SIMULACION

typedef struct Parameters {
    int flag;
    float b_0;
    float b_f;
    float dB;
    int Nterm;
    int Nmed;
    int Nmc;
} Parameters;

int xp[L],yp[L],xm[L],ym[L];
int Ntot, Nacep;
// alogrithmsDATA.c : algoritmos para tratar los datos

int loadParameters(Parameters *parameters);
void saveconfig(int *config);

// algorithmsNET.C : algoritmos para tratar la red

double parisirapuano(void);
void genconfig(int *config, Parameters p);
int spinrandom(void);
void offsets(int *xp,int *yp,int *xm,int *ym);
float energia(int *S);
float magneto(int *S);
void probabilidad(double *prob, double beta);
void metropolis(int *s, double *prob);
