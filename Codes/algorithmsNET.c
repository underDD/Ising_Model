#include "head1.h"

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

    for(i=0;i<L;i++)
    {
        xp[i]=0;
        yp[i]=0;
        xm[i]=0;
        ym[i]=0;
    }

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

    float E;
    int n,x,y;

    E=0;
    n=0;

    for(y=0;y<L;y++){
        for(x=0;x<L;x++){
            E+=S[n]*(S[n+xp[x]]+S[n+yp[y]]);
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

    M=0;

    for(i=0;i<V;i++){
        M+=S[i];
    }

    //if(M<0) M=-M;

    return M/(float)V;
}



/*

    Genera el array de probabilidades

*/
void probabilidad(float *prob,float beta)
{

    prob[0] = exp(-beta*(-8.0));
    prob[1] = exp(-beta*(-4.0));
    prob[2] = exp(-beta*(0.0));
    prob[3] = exp(-beta*(4.0));
    prob[4] = exp(-beta*(8.0));

}

/*

    Aplica metropolis pasando por toda la red, realiza un paso de Montacalo

*/

void metropolis(int *s, float *prob){

    int n,x,y,IND;
    double omega;

    n=0;
    IND=0;
    Ntot=Nacep=0;

    for(y=0;y<L;y++)
    {
        for(x=0;x<L;x++)
        {
            IND = 2+s[n]*(s[n+xp[x]]+s[n+yp[y]]+s[n+xm[x]]+s[n+ym[y]])/2;
            omega=parisirapuano();

            if (prob[IND] > omega)
            {
                //printf("%lf \t %d\n", omega, IND);
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
}
