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
extern float X(float *m, int NDATA);
extern float Cv(float *e, int NDATA);
extern void histograma(float *DATA,float *H, int NDATA, float *maxi, float *mini, float *delta);
extern float e2(float e);
extern float m2(float m);
extern float mean(float *DATA,int NDATA);
extern float Var(float *DATA, int NDATA);
void bloques(float *DATA, int NDATA, int size, float *med, float *error, float *med2, float *error2);

// algorithmsNET.C : algoritmos para tratar la red

extern double parisirapuano(void);
extern void genconfig(int *config, Parameters p);
extern int spinrandom(void);
extern void offsets();
extern float energia(int *S);
extern float magneto(int *S);
extern void probabilidad(float *prob, float beta);
extern void metropolis(int *s, float *prob);
