#include "head1.h"

/*

    Generadir aleatorio de Parisi-Rapuano
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

    int i,j;
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

    Calcula la energia extensiva de una configuracion

*/
float energia(int *xp,int *yp,int *S){

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

    Calcula la magnetizacion extensiva de una configuracion

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

/*

    Realiza todos los calculos necesarios

*/
void calculos(float *estm,float *e,float *esq, float *msq,int *S,int *xp,int *yp){

    *estm=(magneto(S));      // Sin valor absoluto
    *e=energia(xp,yp,S);
    *esq=*e* *e;
    *msq=*estm* *estm;

}
