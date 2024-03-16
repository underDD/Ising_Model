#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define L 10
#define V L*L
#define N_DATA 10000
#define N_Inter 100
#define fran rand()/((double)RAND_MAX+1)
#define NormRAnu (2.3283063671E-10F)

typedef struct Parameters {
    int flag;
    double b_0;
    double b_f;
    double dB;
    int Nterm;
    int Nmed;
    int Nmc;
} Parameters;

int loadParameters(Parameters *parameters);
void genconfig(int *config, Parameters p);
int spinrandom(void);
double parisirapuano(void);
void offsets(int *xp,int *yp,int *xm,int *ym);
void calculos(float *estm,float *e,float *esq, float *msq,int *S,int *xp,int *yp);
float energia(int *xp,int *yp,int *S);
float magneto(int *S);
void saveconfig(int *config);
void Histograma(double *I,double *H,double *delta,double *mx,double *mn);
