#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define L 64
#define V L*L
#define N_DATA 10000
#define N_Inter 100
#define fran rand()/((double)RAND_MAX+1)
#define NormRAnu (2.3283063671E-10F)

#define MAX_STR_LEN 256

#define ACEPTANCIA
//#define TERMALIZACION
#define SIMULACION

typedef struct Parameters {
    int flag;
    float b_0;
    float b_f;
    float dB;
    int Nterm;
    int Nmed;
    int Nmc;
} Parameters;

extern int xp[L],yp[L],xm[L],ym[L];
extern int Ntot, Nacep;

// alogrithmsDATA.c : algoritmos para tratar los datos

extern int loadParameters(Parameters *parameters);
extern void saveconfig(int *config);
extern void calculos(double *e,double *m, int NDATA, double *results);
extern double X(double *m, int NDATA);
extern double Cv(double *e, int NDATA);
extern void histograma(double *DATA,double *H, int NDATA, double *maxi, double *mini, double *delta);
extern double e2(double e);
extern double m2(double m);
extern double mean(double *DATA,int NDATA);
extern double Var(double *DATA, int NDATA);

// algorithmsNET.C : algoritmos para tratar la red

extern double parisirapuano(void);
extern void genconfig(int *config, Parameters p);
extern int spinrandom(void);
extern void offsets();
extern float energia(int *S);
extern float magneto(int *S);
extern void probabilidad(float *prob, float beta);
extern void metropolis(int *s, float *prob);
