#include "head1.h"


int main(){

    srand(time(NULL));

    int S[V];
    int stepsBeta, pasoBeta;
    int pasoTerm;
    int pasoMed;
    int pasoMC;
    int hyst;

    FILE *fout;

    float beta;
    float prob[5];
    float e,m;

    Parameters p;

    clock_t time; // Esto es para calcular el tiempo de ejecucion

    char name[MAX_STR_LEN];

    loadParameters(&p);
    printf("Beta inicial --> %f\n",p.b_0);
    printf("Beta final --> %f\n",p.b_f);
    printf("Paso beta --> %f\n",p.dB);
    printf("Pasos Montecarlo -->  %d\n",p.Nmc);
    printf("Numero medidas --> %d\n",p.Nmed);
    printf("Pasos termalizacion --> %d\n",p.Nterm);

    beta = p.b_0;
    stepsBeta = (int)((p.b_f-p.b_0)/p.dB); // Calcula cuántos pasos hay que hacer para recorrer todo el paso de betas
    time = clock();

    genconfig(S,p);
    offsets();

    #ifdef TERMALIZACION

        
        probabilidad(prob,beta);
        sprintf(name,"results/term_%d_%d_%.2f.txt",p.Nterm,L,beta);
        fout = fopen(name,"wt");

        for(pasoTerm=0;pasoTerm < p.Nterm;pasoTerm++)
        {
            e = m = 0;

            for(pasoMC=0;pasoMC<p.Nmc;pasoMC++)
                    metropolis(S,prob);

            e = energia(S);
            m = magneto(S);
            
            fprintf(fout,"%d\t%f\t%f\n",pasoTerm*p.Nmc, e, m);
        }

        saveconfig(S);
        fclose(fout);


    #endif // TERMALIZACION

    #ifdef SIMULACION

   // for(hyst=0;hyst<2;hyst++)
        for(pasoBeta=0;pasoBeta<stepsBeta;pasoBeta++)
        {
            probabilidad(prob,beta);
            sprintf(name,"results/med_%d_%d_%.2f.txt",hyst,L,beta);
            fout = fopen(name,"wt");

            for(pasoTerm=0;pasoTerm<p.Nterm;pasoTerm++)
            {
                metropolis(S,prob);
                //printf("Paso de termalizacion %d completado\n",pasoTerm);
            }

            for(pasoMed=0;pasoMed<p.Nmed;pasoMed++)
            {
                for(pasoMC=0;pasoMC<p.Nmc;pasoMC++)
                    metropolis(S,prob);

                 //fprintf(fout,"%d\t%lf\t%lf\n",pasoMed, energia(S), magneto(S));
            }

            fclose(fout);
        }

    #endif // SIMULACION

    #ifdef ACEPTANCIA

        printf("Aceptancia = %lf", (double)Nacep/Ntot);

    #endif // ACEPTANCIA

    time = clock() - time;
    double time_taken = ((double)time)/CLOCKS_PER_SEC;

    printf("\nTiempo de CPU (s) --> %lf\n ", time_taken);

    return 0;

}
