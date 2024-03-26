/*
    ESTO ES UNA MIERDA NO SE PORQUE NO FUNCIONA NO PUEDO MAS
    EL VISUALSUDIO ES UNA MIERDA TAMBIEN, NO ME FUNCIONA EL JONSON NI NADA
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define L (int)128
#define V L*L
#define N_DATA 10000
#define N_Inter 100
#define fran rand()/((double)RAND_MAX+1)
#define NormRAnu (2.3283063671E-10F)
//#define ACEPTANCIA

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

// alogrithmsDATA.c : Algoritmos para tratar los datos

int loadParameters(Parameters *parameters);
void saveconfig(int *config);
void Histograma(double *I,double *H,double *delta,double *mx,double *mn);

// algorithmsNET.C : algoritmos para tratar la red

double parisirapuano(void);
void genconfig(int *config, Parameters p);
int spinrandom(void);
void offsets(int *xp,int *yp,int *xm,int *ym);
float energia(int *S);
float magneto(int *S);
void probabilidad(double *prob, double beta);
void metropolis(int *s, double *prob);


int main(){

    srand(time(NULL));

    int S[V];
    // int stepsBeta, pasoBeta;
    int pasoTerm;
    int pasoMed;
    int pasoMC;
    int hyst;

    FILE *fout;

    float beta;
    double *prob;
    prob = (double*)calloc(5, sizeof(double));

    Parameters p;

    char name[20];

    loadParameters(&p);
    printf("Beta inicial --> %f\n",p.b_0);
    printf("Beta final --> %f\n",p.b_f);
    printf("Paso beta --> %f\n",p.dB);
    printf("Pasos Montecarlo -->  %d\n",p.Nmc);
    printf("Numero medidas --> %d\n",p.Nmed);
    printf("Pasos termalización --> %d\n",p.Nterm);

    beta = 0.5;
    hyst = 0;
    // stepsBeta = (int)((p.b_f-p.b_0)/p.dB); // Calcula cuántos pasos hay que hacer para recorrer todo el paso de betas

    genconfig(S,p);
    offsets(xp,yp,xm,ym);

    //for(hyst=0;hyst<2;hyst++) 
        // for(pasoBeta=0;pasoBeta<stepsBeta;pasoBeta++)
        
            probabilidad(prob,beta);
            sprintf(name,"results/med_%d.txt",hyst);
            fout = fopen(name,"wt");
            for(pasoTerm=0;pasoTerm<p.Nterm;pasoTerm++)
            {
                metropolis(S,prob);
            }

            for(pasoMed=0;pasoMed<p.Nmed;pasoMed++)
            {
                for(pasoMC=0;pasoMC<p.Nmc;pasoMC++)
                    metropolis(S,prob);
                
                fprintf(fout,"%lf\t%lf\n",energia(S), magneto(S));
            }

            fclose(fout);
       
    


    
    free(prob);
    return 0;
    
}




/*
    Generador aleatorio de Parisi-Rapuano
    Devuelve un número aleatorio entre 0 y 1
*/
double parisirapuano(void)
{

    unsigned int irr[256];
    unsigned int ir1;
    unsigned char ind_ran,ig1,ig2,ig3;
    int i;

    ind_ran=ig1=ig2=ig3=0;

    for(i=0;i<256;i++)
        irr[i] = (rand()<<16) + rand();

    ig1 = ind_ran-24;
    ig2 = ind_ran-55;
    ig3 = ind_ran-61;
    irr[ind_ran] = irr[ig1] + irr[ig2];
    ir1 = (irr[ind_ran] ^ irr[ig3]);
    ind_ran++;

    return ir1 * NormRAnu;

}


/*
    Devuelve spin 1 o -1 de manera random
*/
int spinrandom(void)
{

    double omega;

    omega = parisirapuano();

    if (omega>0.5) return -1;

    return 1;

}

/*
    Genera una configuraciones en función de un valor 'flag' :

        - flag=0 --> random
        - flag=1 --> congelada 1 o -1
        - flag=2 --> ajedrez
        - flag=3 --> carga una configuración de un fichero
*/
void genconfig(int *config, Parameters p)
{

    int i;
    int flag;
    double omega;
    FILE *f;

    flag = p.flag;

    switch(flag)
    {

    case 0: // Random

        printf("\nflag = %d --> Configuracion random\n\n",flag);

        for(i=0;i<V;i++)
        {
            config[i] = spinrandom();
        }
        break; // Ferromagnetica

    case 1:

        printf("\nflag = %d --> Configuracion ferromagnetica\n\n",flag);

        omega = parisirapuano();

        for(i=0;i<V;i++)
        {
            if (omega > 0.5) config[i] = 1;
            if (omega < 0.5) config[i] = -1;
        }
        break;

    case 2: // Antiferromagnetica

        printf("\nflag = %d --> Configuracion antiferromagnetica\n\n",flag);

        for(i=0;i<L;i++)
        {
            if (i%2==0) config[i] = 1;
            else
                config[i] = -1;
        }

        for(i=L;i<V;i++)
        {
            config[i] = -config[i-L];
        }

        break;

    case 3: // Leer de un fichero la configuracion

        printf("\nflag = %d --> Configuracion leida de fichero\n\n",flag);

        f=fopen("savedconfig.txt","rt");

        if (f == NULL) {printf("ERROR leyendo la configuración.\n"); exit(1);}

        for(i=0;i<V;i++)
        {

            fscanf(f,"%d",&config[i]);

        }

        fclose(f);

        break;

    default:
        break;

    }

}




/*
    Construye los offsets necesarios para movernos por la red
*/
void offsets(int *xp,int *yp,int *xm,int *ym){

    int i;

    for(i=0;i<L;i++){
        xp[i]=1;
        yp[i]=L;
        xm[i]=-1;
        ym[i]=-L;
    }
    xp[L-1]=1-L;
    yp[L-1]=-L*(L-1);
    xm[0]=L-1;
    ym[0]=L*(L-1);

}

/*
    Calcula la energia intensiva de una configuracion
*/
float energia(int *S){

    double E;
    int n,i,j;

    E=0;
    n=0;

    for(i=0;i<L;i++){
        for(j=0;j<L;j++){
            E+= S[n] * (S[n+xp[j]] + S[n+yp[i]]);
            n++;
        }
    }

    return -E/(2*V);
}

/*

    Calcula la magnetizacion intensiva de una configuracion

*/
float magneto(int *S){

    int M;
    int i;
    int LL=V;

    M=0;

    for(i=0;i<V;i++){
        M+=S[i];
    }

    //if(M<0) M=-M;

    return M/(double)LL;
}


void probabilidad(double *prob,double beta)
{
    prob[0]=exp(8.0*beta);
    prob[1]=exp(4.0*beta);
    prob[2]=exp(0.0*beta);
    prob[3]=exp(-4.0*beta);
    prob[4]=exp(-8.0*beta);

}

void metropolis(int *s, double *prob){

    int n,x,y,IND;

    n=0;
    IND=0;
    Ntot=Nacep=0;

    for(y=0;y<L;y++)
        for(x=0;x<L;x++)
        {
            IND = s[n]*(s[n+xp[x]]+s[n+yp[y]]+s[n+xm[x]]+s[n+ym[y]])/2+2;
            if (parisirapuano() < prob[IND])
            {
                s[n] = -s[n];
                #ifdef ACEPTANCIA
                    Nacep++;
                #endif
            }
            #ifdef ACEPTANCIA
                Ntot++;
            #endif
            n++;
        }
}

/*
    Lee de un fichero "flag" los datos necesarios para la simulación
*/
int loadParameters(Parameters *parameters) {

    char line[100];
    FILE *fin;
    fin=fopen("flag.txt","r");

    if (fin==NULL)  return -1;

    while (!feof(fin)) { // Recorre cada línea hasta el final del fichero.

        fgets(line,sizeof(line),fin); // Guarda la linea en line
        char *token = strtok(line," "); // El puntero token se posiciona donde aparece el primer espacio

        if (token != NULL) { // Se comprueba que ha leido al menos un espacio

            if (strcmp(token,"flag") == 0)
            { //Se compara el valor del token con el del nombre de la variable
                //strtok para encontrar el siguiente espacio (final de la frase) tomando asi el valor de la variable en char y se transforma a double con atof
                parameters->flag=atof(strtok(NULL," "));

            } else if (strcmp(token,"Beta_0")==0)
            {

                parameters->b_0=atof(strtok(NULL," "));

            } else if (strcmp(token,"Beta_f")==0)
            {

                parameters->b_f=atof(strtok(NULL," "));

            } else if (strcmp(token,"dBeta")==0)
            {

                parameters->dB=atof(strtok(NULL," "));

            } else if (strcmp(token,"Nterm")==0)
            {

                parameters->Nterm=atof(strtok(NULL," "));

            } else if (strcmp(token,"Nmed")==0)
            {

                parameters->Nmed=atof(strtok(NULL," "));

            } else if (strcmp(token,"Nmc")==0)
            {

                parameters->Nmc=atof(strtok(NULL," "));
            }

        }
    }

    fclose(fin);

    return 0;
}


/*
    Construye un histograma
*/
void Histograma(double *I,double *H,double *delta,double *mx,double *mn)
{

    int i,j;
    double max,min,d,norma;

    max=-100000000;
    min=+100000000;

    for(i=0;i<N_Inter;i++) H[i]=0;

    for(i=0;i<N_DATA;i++)
    {
        if (I[i]>max) max=I[i];
        if (I[i]<min) min=I[i];
    }

    d=(max-min)/N_Inter;
    norma=1.0/(N_DATA*d);

    for(i=0;i<N_DATA;i++)
    {
        j=(I[i]-min)/d;
        if (j==N_Inter) j=j-1;
        H[j]++;
        //printf("%d\n",j);
    }

    for(i=0;i<N_Inter;i++)
        H[i]*=norma;

    *delta=d;
    *mx=max;
    *mn=min;

}

/*
    Guarda la configuración en un fichero
*/
void saveconfig(int *config) // La guardo alreves que en teoría, es decir, la posición 0 corresponde a la esquina superior izq, por comodidad.
{

    int i;
    FILE *f;

    f=fopen("savedconfig.txt","wt");

    for(i=0;i<V;i++)
    {
        fprintf(f, "%d%c", config[i], (i+1)%L==0? '\n':' ');

        //if((i+1)%L==0) fprintf(f,"\n");
        //fprintf(f,"%d ",config[i]);

    }

    fclose(f);

}
