#include "head1.h"

int main(){

    srand(time(NULL));

    int S[V];
    int stepsBeta, pasoBeta;
    int pasoTerm;
    int pasoMed;
    int pasoMC;
    int hyst;
    int ele;
    int xp[L],yp[L],xm[L],ym[L];

    FILE *fout;

    double beta;
    double prob[5];

    Parameters p;

    char name[20];

    loadParameters(&p);
    printf("Beta inicial --> %f\n",p.b_0);
    printf("Beta final --> %f\n",p.b_f);
    printf("Paso beta --> %f\n",p.dB);
    printf("Pasos Montecarlo -->  %d\n",p.Nmc);
    printf("Numero medidas --> %d\n",p.Nmed);
    printf("Pasos termalizacion --> %d\n",p.Nterm);

    beta = p.b_0;
    stepsBeta = (int)((p.b_f-p.b_0)/p.dB); // Calcula cuántos pasos hay que hacer para recorrer todo el paso de betas

    genconfig(S,p);
    offsets(xp,yp,xm,ym);

    #ifdef TERMALIZACION

        hyst = 0;
        probabilidad(prob,beta);
        sprintf(name,"results/term_%d_%d_%.2lf.txt",p.Nterm,L,beta);
        fout = fopen(name,"wt");
        for(pasoTerm=0;pasoTerm < p.Nterm;pasoTerm++)
        {
            metropolis(S,prob);
            fprintf(fout,"%d\t%f\t%f\n",pasoTerm, energia(S), magneto(S));
        }
        fclose(fout);

    #endif // TERMALIZACION

    #ifdef SIMULACION

    for(hyst=0;hyst<2;hyst++)
        for(pasoBeta=0;pasoBeta<stepsBeta;pasoBeta++)
        {
            probabilidad(prob,beta);
            sprintf(name,"results/med_%d_%d_%lf.txt",hyst,L,beta);
            fout = fopen(name,"wt");
            for(pasoTerm=0;pasoTerm<p.Nterm;pasoTerm++)
            {
                metropolis(S,prob);
                //printf("Paso de termalizacion %d comletado\n",pasoTerm);
            }

            for(pasoMed=0;pasoMed<p.Nmed;pasoMed++)
            {
                for(pasoMC=0;pasoMC<p.Nmc;pasoMC++)
                    metropolis(S,prob);

                 fprintf(fout,"%d\t%lf\t%lf\n",pasoMed, energia(S), magneto(S));
            }
            fclose(fout);
        }

    #endif

    #ifdef ACEPTANCIA

        printf("Aceptancia = %lf", (double)Nacep/Ntot);

    #endif // ACEPTANCIA
    return 0;

}
