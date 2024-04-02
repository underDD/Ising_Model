#include "head1.h"

int main()
{

    Parameters p; 
    loadParameters(&p);

    float e[p.Nmed], m[p.Nmed];
    float eme;
    float beta;
    float med1, error1, med2, error2;

    FILE *fin;
    FILE *foute;
    FILE *foutm;
    

    int i,j;
    int hyst;
    int nFich;

    char name[MAX_STR_LEN];

    nFich = (int)((p.b_f-p.b_0)/p.dB);

    beta = p.b_0;
    hyst = 0;
    j = 0;
    foute = fopen("results/resultse.txt","wt");
    foutm = fopen("results/resultsm.txt","wt");

    fprintf(foute,"beta\t<e>\terror<e>\tCv\terrorCv\n");
    fprintf(foutm,"beta\t<m>\terror<m>\tX\terrorX\n");
    
    #ifdef SIMULACION

        for(i=0;i<nFich;i++)
        {   
            sprintf(name,"medidas/med_%d_%d_%.2f.txt",hyst,L,beta);
            fin = fopen(name,"rt");

            if (fin == NULL) {printf("ERROR leyendo el fichero de beta %f",beta); exit(1);}
            
            while(!feof(fin))
            {
                
                fscanf(fin,"%d",&j);
                fscanf(fin,"%f%f",&e[j],&eme);
                m[j]=fabs(eme);

            }

            
            
            bloques(e,p.Nmed,20,&med1,&error1,&med2,&error2,1);
            
            fprintf(foute,"%f\t", beta);
            fprintf(foute,"%f\t%f\t%f\t%f\n",med1,error1,med2,error2);

            bloques(m,p.Nmed,20,&med1,&error1,&med2,&error2,2);
            
            fprintf(foutm,"%f\t", beta);
            fprintf(foutm,"%f\t%f\t%f\t%f\n",med1,error1,med2,error2);

            beta += p.dB;
            fclose(fin);

        }

    #endif //SIMULACION
    

    #ifdef HISTOGRAMAS

        FILE *hist;
        FILE *histe;
        FILE *histm;

        float H[N_Inter];
        float max, min, delta;

        beta = p.b_0;
        sprintf(name,"medidas/histo_%d_%.2f.txt",L,beta);
        hist = fopen(name,"rt");

        while(!feof(hist))
        {
            
            fscanf(hist,"%d",&j);
            fscanf(hist,"%f%f",&e[j],&m[j]);
            
        }

        histe = fopen("results/histogramae.txt","wt");
        histm = fopen("results/histogramam.txt","wt");

        histograma(e,H,p.Nmed,&max,&min,&delta);

        for(i=0;i<N_Inter;i++)
        {
            fprintf(histe,"%f\t%f\n",i*delta+min, H[i]);
        }

        histograma(m,H,p.Nmed,&max,&min,&delta);

        for(i=0;i<N_Inter;i++)
        {
            fprintf(histm,"%f\t%f\n",i*delta+min, H[i]);
        }

        fclose(hist);
        fclose(histe);
        fclose(histm);
    
    #endif //HISTOGRAMAS

    hyst = 1;
    j = 0;
    beta = p.b_0;

    /*
    for(i=0;i<nFich+1;i++)
    {   

        sprintf(name,"results/med_%d_%d_%.2f.txt",hyst,L,beta);
        fin = fopen(name,"rt");

        if (fin == NULL) {printf("ERROR leyendo el fichero de beta %f",beta); exit(1);}
        
        while(!feof(fin))
        {
            
            fscanf(fin,"%d",&j);
            fscanf(fin,"%f%f",&e[j],&m[j]);

        }

        em = mean(e,p.Nmed);
        mm = fabs(mean(m,p.Nmed));
        Cvm = Cv(e,p.Nmed);
        Xm = X(m,p.Nmed);
        fprintf(fout, "%d\t%f\t%f\t%f\t%f\t%f\n",hyst, beta, em, mm , Cvm, Xm);

        beta += p.dB;
        fclose(fin);

    }
    */
    

    printf("FIN");
    fclose(foute);
    fclose(foutm);
    return 0;
}