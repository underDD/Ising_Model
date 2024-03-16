
/*
 *  PROGRAMA PARA LOS RESULTADOS Y ERRORES
 *
 *  Manuel Álvarez
 *  Andrés Bea
 *  Ander Gabarrús
 *  Hernán Gracia
 *
 */

 // CÓDIGO SIN COMENTAR

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define L 32
#define V L*L
#define fran rand()/((double)RAND_MAX+1)
#define NMAX 10000

void flag(float *DATOS); // Lee de un fuchero flag los datos necesarios
int spinrandom(void); // Genera 1 o -1 de maner random
void iniciarCongif(int *S); // Inicia la primera config
int Npasos(float bi,float bf,float db); // Calcula cuantos pasos de
void prob(float beta,float *p);
void offsets(int *xp,int *yp,int *xm,int *ym,int *s);
void metropolis(int s[],float *exp);
double omega(void);
void inidata(float *estm,float *e,float *esq, float *msq,int N);
void calculos(float *estm,float *e,float *esq, float *msq,int *S,int *xp,int *yp);
void medyerr(float *estm,float *e,float *esq, float *msq,int N,double resultados[][11],int ind,float beta);

void med_var(float *serie, int N, float *med, float *var);
float energia(int *xp,int *yp,int *S);
float magnetoest(int *S);

float Cv_o_ji(int N, float *e);
float error_Cv(float *e, int N_med, int P);
float error_bloques(float *e, int N_m, int P);
float error_ji(float *e, int N_med, int P);

int xp[L],yp[L],xm[L],ym[L];

int main(){
    float DATA[6]; // 1-> Beta inicla 2-> Beta final 3-> Delta de Beta 4-> Pasos de Termalización 5-> Medidas 6-> MonteCarlo
    int i,j,sentido,Nbeta,N_term,mc,N_m;
    //int L=8;
    int S[V];
    float beta;
    float probabilidad[5];
    int LL=V;
    float estm[NMAX],e[NMAX],erre[NMAX],errm[NMAX],esq[NMAX],msq[NMAX];
    double resultados[NMAX][11];
    FILE *fout=fopen("Resultados.txt","w");

    srand(time(NULL));
    flag(DATA); //Toma los datos necesarios
    int N_med=(int)DATA[4];
    int N_mte=(int)DATA[5];
    int N_t=(int)DATA[3];
    float dbeta=DATA[2];
    int N_pasos=(Npasos(DATA[0],DATA[1],DATA[2])); //Para cuatas betas vamos a calcular


   // srand(time(NULL));
    iniciarCongif(S); //Creamos la primera config o leemos
    beta=DATA[0];
    offsets(xp,yp,xm,ym,S); //Inicializamos offsets
    //N_t=0;

    if (fout==NULL) printf("ERROR");
    else{

        printf("Beta\t\t<e>\t\tError de e\t<|m|>\t\tError de |m|\t<e^2>\t\t<m^2>\t\tCv\t\tX\t\n");

        //for(j=0;j<=4;j++){

        for(sentido=0;sentido<1;sentido++){ //Recorrerlas betas hacia delante y hacia atras.

            for(Nbeta=0;Nbeta<N_pasos;Nbeta++){ //Caulculos para cada beta
                prob(beta,probabilidad);

                for(N_term=0;N_term<N_t;N_term++){ //Tras la termalización se supone que obtenemos configuraciones mas centradas en la media pero hay que hallar el valor de N_t
                    metropolis(S,probabilidad);
                }

                inidata(estm,e,esq,msq,N_med);

                for(N_m=0;N_m<N_med;N_m++){

                        for(mc=0;mc<N_mte;mc++);
                            metropolis(S,probabilidad);

                        calculos(&estm[N_m],&e[N_m],&esq[N_m],&msq[N_m],S,xp,yp);

                }
                //printf("%f\n",error_bloques(e,N_med,N_med));
                medyerr(estm,e,esq,msq,N_med,resultados,Nbeta,beta); //Guarda en el array resultados los valores de media y error de cada magnitud.
                for(i=0;i<11;i++){
                    //printf("%.4f\t\t",resultados[Nbeta][i]);
                    fprintf(fout,"%.4f\t\t",resultados[Nbeta][i]);
                }

                //printf("\n");
                fprintf(fout,"\n");
                beta+=dbeta;
            }
            dbeta=-dbeta;
        }
  //  }
        //L+=4;
    }
    fclose(fout);
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

    *estm=fabs(magnetoest(S));
    *e=energia(xp,yp,S);
    *esq=*e* *e;
    *msq=*estm* *estm;

}


void medyerr(float *estm,float *e,float *esq, float *msq,int N,double resultados[][11],int ind,float beta){

        float med,var;
        int LL=V;

        resultados[ind][0]=beta;
        med=var=0;

        med_var(e,N,&med,&var); //Calcula media y varianza de la energia intensiva
        //printf("%f\n%f\n",med,var);
        resultados[ind][1]=med;
        resultados[ind][2]=sqrt(var);

        med=var=0;

        med_var(estm,N,&med,&var); //Calcula media y varianza de estimador de la magnetización
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

        resultados[ind][7]=2*Cv_o_ji(N,e);
        resultados[ind][8]=Cv_o_ji(N,estm);

        resultados[ind][9]=error_Cv(e,N,200);//error Cv
        resultados[ind][10]=error_ji(estm,N,200);//error ji

}

float error_bloques(float *e, int N_med, int P){
    int N = N_med / P;//P (el número de bloques) tiene que ser divisor de N_med, N es el número de datos por bloque
    float prov[N],media_bloques[P],med,var;

    for(int p=0;p<P;p++){
        for(int i=0;i<N;i++){
            prov[i]=e[p*N+i];
        }
        med_var(prov,N,&media_bloques[p],&var);//ahora vale para calcular e y m, habría que cambiar esta línea si se quiere con Cv y ji, pero no hay subalgoritmos que lo hagan :(
    }
    med_var(media_bloques,P,&med,&var);
    float result=sqrt(var);

    return result;
}

float Cv_o_ji(int N, float *e){//multiplicar por 2 si es Cv
    float med,var;
    med_var(e,N,&med,&var);
    return V*var;
}

float error_Cv(float *e, int N_med, int P){//le metes e
    int N = N_med / P;//P (el número de bloques) tiene que ser divisor de N_med, N es el número de datos por bloque
    float prov[N],media_bloques[P],med,var;

    for(int p=0;p<P;p++){
        for(int i=0;i<N;i++){
            prov[i]=e[p*N+i];
        }
        media_bloques[p]=2*Cv_o_ji(N,prov);//ahora vale para calcular Cv
    }
    med_var(media_bloques,P,&med,&var);
    float result=sqrt(var);

    return result;
}

float error_ji(float *e, int N_med, int P){//le metes m
    int N = N_med / P;//P (el número de bloques) tiene que ser divisor de N_med, N es el número de datos por bloque
    float prov[N],media_bloques[P],med,var;

    for(int p=0;p<P;p++){
        for(int i=0;i<N;i++){
            prov[i]=e[p*N+i];
        }
        media_bloques[p]=Cv_o_ji(N,prov);//ahora vale para calcular ji
    }
    med_var(media_bloques,P,&med,&var);
    float result=sqrt(var);

    return result;
}
