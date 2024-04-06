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
        sprintf(name,"results/terma/term_%d_%d_%.2f.txt",p.Nterm,L,beta);
        fout = fopen(name,"wt");
        //fout = fopen(name,"wb");

        e = m = 0;
        pasoTerm = 0;
        e = energia(S);
        m = fabs(magneto(S));
        fprintf(fout,"%d\t%f\t%f\n",0, e, m);

        //fwrite(&pasoTerm, sizeof(int), 1, fout);
        //fwrite(&e, sizeof(float), 1, fout);
        //fwrite(&m, sizeof(float), 1, fout);

        for(pasoTerm=1;pasoTerm < p.Nterm;pasoTerm++)
        {
            e = m = 0;

            metropolis(S,prob); 
            e = energia(S);
            m = fabs(magneto(S));
            
            fprintf(fout,"%d\t%f\t%f\n",pasoTerm, e, m);

            //fwrite(&e, sizeof(float), 1, fout);
            //fwrite(&m, sizeof(float), 1, fout);
        }

        saveconfig(S);
        fclose(fout);


    #endif // TERMALIZACION

    #ifdef SIMULACION

         //for(hyst=0;hyst<2;hyst++)
         
            for(pasoBeta=0;pasoBeta<stepsBeta;pasoBeta++)
            {

                probabilidad(prob,beta);
                sprintf(name,"medidas/1282/med_%d_%d_%.2f.txt",L,hyst,L,beta);
                fout = fopen(name,"wt");
                //fout = fopen(name,"wb");

                for(pasoTerm=0;pasoTerm < p.Nterm;pasoTerm++)
                    metropolis(S,prob); 
            

                for(pasoMed=0;pasoMed<p.Nmed;pasoMed++)
                {
                    
                    for(pasoMC=0;pasoMC<p.Nmc;pasoMC++)
                        metropolis(S,prob);
                    
                    
                    fprintf(fout,"%d\t%f\t%f\n",pasoMed, energia(S), magneto(S));
                    //fwrite(&e, sizeof(float), 1, fout);
                    //fwrite(&m, sizeof(float), 1, fout);
                }

                beta+=p.dB;
                saveconfig(S);
                fclose(fout);

            }

             //p.dB = -p.dB;
         
        

    #endif // SIMULACION

    #ifdef HISTOGRAMAS

        beta = p.b_0;
        probabilidad(prob,beta);
        sprintf(name,"medidas/histo_%d_%.2f.txt",L,beta);
        fout = fopen(name,"wt");
        //fout = fopen(name,"wb");

        for(pasoTerm=0;pasoTerm < p.Nterm;pasoTerm++)
            metropolis(S,prob); 

        for(pasoMed=0;pasoMed<p.Nmed;pasoMed++)
        {
            e = m = 0;
            for(pasoMC=0;pasoMC<p.Nmc;pasoMC++)
            {
                metropolis(S,prob);
            }
            
            e = energia(S);
            m = magneto(S);

            fprintf(fout,"%d\t%f\t%f\n",pasoMed, e, m);
            //fwrite(&e, sizeof(float), 1, fout);
            //fwrite(&m, sizeof(float), 1, fout);
        }

        fclose(fout);

    #endif //HISTOGRAMAS


    #ifdef ACEPTANCIA

        printf("Aceptancia = %lf", (double)Nacep/Ntot);

    #endif // ACEPTANCIA

    time = clock() - time;
    double time_taken = ((double)time)/CLOCKS_PER_SEC;

    printf("\nTiempo de CPU (s) --> %lf\n ", time_taken);

    return 0;

}
