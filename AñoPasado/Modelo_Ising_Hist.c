/*
 *  PROGRAMA PARA EL PROCESO DE TERMALIZACI�N
 *
 *  Manuel �lvarez
 *  Andr�s Bea
 *  Ander Gabarr�s
 *  Hern�n Gracia
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define L 128
#define V L*L
#define fran rand()/((double)RAND_MAX+1)
#define NMAX 10000

#define numIntervalos 30


void flag(float *DATOS); // Lee de un fichero flag los datos necesarios
int spinrandom(void); // Genera 1 o -1 de manera random
void iniciarCongif(int *S); // Inicia la primera config
int Npasos(float bi,float bf,float db); // Calcula pasos de beta
void prob(float beta,float *p); // Actualiza el array de probabilidades
void offsets(int *xp,int *yp,int *xm,int *ym,int *s);   // Establece los pasos direccionales
void metropolis(int *s,float *exp);    // Algoritmo de metr�polis recorriendo secuencialmente toda la red
double omega(void); // Genera un valor entre 0 y 1
void inidata(float *estm,float *e,float *esq, float *msq,int N);    // Inicializa a 0 los vectores de observables, no es estrictamente necesario
void calculos(float *estm,float *e,float *esq, float *msq,int *S,int *xp,int *yp);  // Calcula e,m,e2,m2 y los guarda en arrays

// Llama a med_var y guarda los valores devueltos de los observables en el array bidimensional de resultados
void medyerr(float *estm,float *e,float *esq, float *msq,int N,double resultados[][9],int ind,float beta);

void med_var(float *serie, int N, float *med, float *var);  // Halla la media y varianza de un array
float energia(int *xp,int *yp,int *S);  // Calcula la energ�a intensiva
float magnetoest(int *S);   // Calcula la magnetizaci�n intensiva

// Construye un histograma
void Histograma(float *datos, double *H, int numDatos, double *del, double *mi, double *ma);

// Vectores de desplazamiento
int xp[L],yp[L],xm[L],ym[L];

int main(){
    float DATA[6]; // 1-> Beta inicla 2-> Beta final 3-> Delta de Beta 4-> Pasos de Termalizaci�n 5-> Medidas 6-> MonteCarlo

    // �ndices de iteraci�n
    int i,sentido,Nbeta,N_term,mc,N_m;

    // Array de spines, beta, array de probabilidades
    int S[V];
    float beta;
    float probabilidad[5];

    int LL=V;

    // Arrays para guardar observables y resultados finales
    float estm[NMAX],e[NMAX],erre[NMAX],errm[NMAX],esq[NMAX],msq[NMAX];
    double resultados[NMAX][9];

    // Fichero de salida, no es necesario en este programa
    //FILE *fout=fopen("Resultados.txt","w");

    // Para el histograma
    double H[numIntervalos],delta,min,max;


    // Inicializaci�n del sistema

    srand(time(NULL));
    flag(DATA); //Toma los datos necesarios
    int N_med=(int)DATA[4];
    int N_mte=(int)DATA[5];
    int N_t=(int)DATA[3];
    float dbeta=DATA[2];
    int N_pasos=(Npasos(DATA[0],DATA[1],DATA[2])); //Para cuatas betas vamos a calcular


    iniciarCongif(S); // Creamos la primera config, aleatoria

    //________________________________________________________________________________________________________________________
    // Sobreescribimos el valor de beta: para este c�digo no hay bucle en beta
    // Variables anteriores no son necesarias, pero este c�digo es modificaci�n del completo

    // Este valor var�a para las distintas fases de estudio
    beta=0.44068;

    //N_med=10000;

    offsets(xp,yp,xm,ym,S); //Inicializamos offsets


    //if (fout==NULL) printf("ERROR");
    //else{

        printf("Beta\t\t<e>\t\tError de e\t<|m|>\t\tError de |m|\t<e^2>\t\t<m^2>\t\tCv\t\tX\t\n");

        // Bucle sin sentido, sirve para ver hist�resis cuando el tope es 2
        for(sentido=0;sentido<1;sentido++){ //Recorrerlas betas hacia delante y hacia atras.

                // No realizamos barrido en beta
                //for(Nbeta=0;Nbeta<N_pasos;Nbeta++){

                prob(beta,probabilidad);

                // Este bucle no se realiza (no descartamos termalizaci�n), ya que N_t=0
                for(N_term=0;N_term<N_t;N_term++){
                    metropolis(S,probabilidad);
                }

                inidata(estm,e,esq,msq,N_med);

                // Para la beta escogida, realizamos N_med medidas de los observables, realizando N_mte pasos de MC entre cada una
                for(N_m=0;N_m<N_med;N_m++){

                        for(mc=0;mc<N_mte;mc++);
                            metropolis(S,probabilidad);

                        calculos(&estm[N_m],&e[N_m],&esq[N_m],&msq[N_m],S,xp,yp);

                }


                /* Parte del programa principal

                medyerr(estm,e,esq,msq,N_med,resultados,Nbeta,beta); // Guarda en el array resultados los valores de media y error de cada magnitud

                for(i=0;i<9;i++){
                    printf("%.4f\t\t",resultados[Nbeta][i]);
                    fprintf(fout,"%.4f\t\t",resultados[Nbeta][i]);
                }
                */

                printf("\n");
                //fprintf(fout,"\n");

                //beta+=dbeta;
            //}

            //dbeta=-dbeta;
        }


        // Histograma de energias

        Histograma(e,H,N_med,&delta,&min,&max);

        FILE *fEnergias = fopen("histEnergias.txt","w");
        for(i=0;i<numIntervalos;i++)
            fprintf(fEnergias,"%f %f\n",i*delta+min+.5*delta,H[i]);
        fclose(fEnergias);


        // Histograma de magnetizaciones

        Histograma(estm,H,N_med,&delta,&min,&max);

        FILE *fMagnetizaciones = fopen("histMagnetizaciones.txt","w");
        for(i=0;i<numIntervalos;i++)
            fprintf(fMagnetizaciones,"%f %f\n",i*delta+min+.5*delta,H[i]);
        fclose(fMagnetizaciones);
    //}
    //fclose(fout);
    return 0;
}

void flag(float *DATOS){

    FILE *ff;
    int i;

    ff=fopen("flag.txt","r");

    if (ff==NULL){
        printf("ERROR al leer el flag.");
    }

    else{
        for(i=0;i<6;i++){
        fscanf(ff,"%f",&DATOS[i]);
    }

    fclose(ff);

    }
}

int spinrandom(void){

    if (rand()%2==0) return 1;
    else return -1;

}

void iniciarCongif(int *S){

    int i;
    FILE *config;
    config=fopen("configbin.dat","rb");

    /*
    if (config==NULL){
        printf("ERROR al leer la config.");
    }
    else{

    fread(S,sizeof(int),V,config);

    }
    */

    for(i=0;i<V;i++){
        S[i]=spinrandom();
    }

    fclose(config);
}

int Npasos(float bi,float bf,float db){

    return (int)(bf-bi)/db;

}

void prob(float beta,float *p){


    p[0]=exp(-beta*(-8));
    p[1]=exp(-beta*(-4));
    p[2]=exp(-beta*(0));
    p[3]=exp(-beta*(4));
    p[4]=exp(-beta*(8));


}

void offsets(int *xp,int *yp,int *xm,int *ym,int *s){

    int i,j;

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

double omega(void){

   // printf("%f",fran);
    return fran;

}

void metropolis(int s[],float exp[]){

    int n,y,x,i;
    int IND;

    n=0;

    for(y=0;y<L;y++){
        for(x=0;x<L;x++){
            IND=2+s[n]*(s[n+xp[x]]+s[n+yp[y]]+s[n+xm[x]]+s[n+ym[y]])/2;
            if(omega() < exp[IND])
                s[n]=-s[n];
            n++;
        }
    }

}

void inidata(float *estm,float *e,float *esq, float *msq,int N){

    int i;

    for(i=0;i<N;i++){
        estm[i]=0;
        e[i]=0;
        esq[i]=0;
        msq[i]=0;
    }

}

void med_var(float *serie, int N, float *med, float *var){
    int i = 0;
    *med = 0.0;
    *var = 0.0;

    for(i = 0; i < N; i++){
        *med += serie[i];
    }

    *med = *med / N;

    for(i = 0; i < N; i++){
        *var += pow((serie[i] - *med), 2.0);
    }

    *var = *var / N;
}

float energia(int *xp,int *yp,int *S){

    double E;
    int n,i,j;

    E=0;
    n=0;

    for(i=0;i<L;i++){
        for(j=0;j<L;j++){
            E+=S[n]*(S[n+xp[j]]+S[n+yp[i]]);
            n++;
        }
    }

    return -E/(2*V);

}

float magnetoest(int *S){

    int M=0;
    int i;
    int LL=V;

    for(i=0;i<V;i++){
        M+=S[i];
    }

    //if(M<0) M=-M;

    return M/(double)LL;
}


void calculos(float *estm,float *e,float *esq, float *msq,int *S,int *xp,int *yp){

    *estm=(magnetoest(S));      // Sin valor absoluto
    *e=energia(xp,yp,S);
    *esq=*e* *e;
    *msq=*estm* *estm;

}


void medyerr(float *estm,float *e,float *esq, float *msq,int N,double resultados[][9],int ind,float beta){

        float med,var;
        int LL=V;

        resultados[ind][0]=beta;

        med=var=0;

        med_var(e,N,&med,&var); //Calcula media y varianza de la energia intensiva
        //printf("%f\n%f\n",med,var);
        resultados[ind][1]=med;
        resultados[ind][2]=sqrt(var);

        med=var=0;

        med_var(estm,N,&med,&var); //Calcula media y varianza de estimador de la magnetizaci�n
        resultados[ind][3]=med;
        resultados[ind][4]=sqrt(var);

        med=var=0;

        med_var(esq,N,&med,&var); //Calcula media y varianza de la energia al cuadrado
        resultados[ind][5]=med;
        //resultados[5][ind]=sqrt(var);

        med=var=0;

        med_var(msq,N,&med,&var); // Calcula media y varianza de la magnetizacion al cuadrado
        resultados[ind][6]=med;
        //resultados[7][ind]=sqrt(var);

        resultados[ind][7]=2*LL*(resultados[ind][5]-resultados[ind][1]*resultados[ind][1]);
        resultados[ind][8]=LL*(resultados[ind][6]-resultados[ind][3]*resultados[ind][3]);

}


void Histograma(float *datos, double *H, int numDatos, double *del, double *mi, double *ma)
{
    int i, indice;
    double delta,min,max,norm;

    for(i=0;i<numIntervalos;i++)
        H[i]=0;

    min=1000000;
    max=-1000000;

    for(i=0;i<numDatos;i++)
    {
        if(datos[i]<min)
            min=datos[i];
        if(datos[i]>max)
            max=datos[i];
    }

    delta=(max-min)/numIntervalos;

    if(delta==0)
        exit(1);

    for (i=0;i<numDatos;i++)
    {
        indice=(datos[i]-min)/delta;
        if (indice==numIntervalos)
            indice=numIntervalos-1;
        H[indice]++;
    }

    norm=1.0/(delta*numDatos);

    for (i=0;i<numIntervalos;i++)
        H[i]*=norm;


    *del = delta;
    *mi = min;
    *ma = max;
}



