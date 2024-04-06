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
    int Nblock;

    char nameE[MAX_STR_LEN];
    char nameM[MAX_STR_LEN];
    char name[MAX_STR_LEN];

    nFich = (int)((p.b_f-p.b_0)/p.dB);

    beta = p.b_0;
    hyst = 0;
    j = 0;
    Nblock = 5;

    sprintf(nameE,"results/resultse_%d_%d.txt",Nblock,L);
    sprintf(nameM,"results/resultsm_%d_%d.txt",Nblock,L);

    foute = fopen(nameE,"wt");
    foutm = fopen(nameM,"wt");

    fprintf(foute,"beta\t<e>\terror<e>\tCv\terrorCv\n");
    fprintf(foutm,"beta\t<m>\terror<m>\tX\terrorX\n");


    #ifdef SIMULACION

            for(i=0;i<nFich;i++)
            {   
                sprintf(name,"medidas/%d/med_%d_%d_%.2f.txt",L,hyst,L,beta);
                fin = fopen(name,"rt");
                
                if (fin == NULL) {printf("ERROR leyendo el fichero de beta %f\n",beta); exit(1);}
                
                while(!feof(fin))
                {
                    
                    fscanf(fin,"%d",&j);
                    fscanf(fin,"%f%f",&e[j],&eme);
                    m[j]=fabs(eme);

                }

                bloques(e,p.Nmed,Nblock,&med1,&error1,&med2,&error2,1);
                
                fprintf(foute,"%f\t", beta);
                fprintf(foute,"%f\t%f\t%f\t%f\n",med1,error1,med2,error2);

                bloques(m,p.Nmed,Nblock,&med1,&error1,&med2,&error2,2);
                
                fprintf(foutm,"%f\t", beta);
                fprintf(foutm,"%f\t%f\t%f\t%f\n",med1,error1,med2,error2);

                beta += p.dB;
                fclose(fin);

            }

            fclose(foute);
            fclose(foutm);

    #endif //SIMULACION
    
    #ifdef HISTERESIS

        for(hyst=0; hyst<2; hyst++)
        {
            sprintf(nameE,"results/hyst/hyste_%d_%d_%d.txt",hyst,Nblock,L);
            sprintf(nameM,"results/hyst/hystm_%d_%d_%d.txt",hyst,Nblock,L);

            foute = fopen(nameE,"wt");
            foutm = fopen(nameM,"wt");

            fprintf(foute,"beta\t<e>\terror<e>\tCv\terrorCv\n");
            fprintf(foutm,"beta\t<m>\terror<m>\tX\terrorX\n");

            for(i=0;i<nFich;i++)
            {   
                sprintf(name,"medidas/hyst/hyst%d/med_%d_%d_%.2f.txt",L,hyst,L,beta);
                fin = fopen(name,"rt");
                
                if (fin == NULL) {printf("ERROR leyendo el fichero de beta %f\n",beta); exit(1);}
                
                while(!feof(fin))
                {
                    
                    fscanf(fin,"%d",&j);
                    fscanf(fin,"%f%f",&e[j],&eme);
                    m[j]=fabs(eme);

                }

                bloques(e,p.Nmed,Nblock,&med1,&error1,&med2,&error2,1);
                
                fprintf(foute,"%f\t", beta);
                fprintf(foute,"%f\t%f\t%f\t%f\n",med1,error1,med2,error2);

                bloques(m,p.Nmed,Nblock,&med1,&error1,&med2,&error2,2);
                
                fprintf(foutm,"%f\t", beta);
                fprintf(foutm,"%f\t%f\t%f\t%f\n",med1,error1,med2,error2);

                beta += p.dB;
                fclose(fin);

            }

            p.dB = -p.dB;
            fclose(foute);
            fclose(foutm);
        }    

    #endif //HISTERESIS

    #ifdef HISTOGRAMAS

        FILE *hist;
        FILE *histe;
        FILE *histm;

        float H[N_Inter];
        float max, min, delta;
        float area;

        sprintf(name,"medidas/histo_%d_%.2f.txt",L,beta);
        hist = fopen(name,"rt");
        //hist = fopen(name,"rb");

        while(!feof(hist))
        {
            
            fscanf(hist,"%d",&j);
            fscanf(hist,"%f%f",&e[j],&m[j]);
            
        }
        
        sprintf(nameE,"results/histo/histogramae_%d_%.2f.txt",L,beta);
        sprintf(nameM,"results/histo/histogramam_%d_%.2f.txt",L,beta);
        
        histe = fopen(nameE,"wt");
        histm = fopen(nameM,"wt");

        histograma(e,H,p.Nmed,&max,&min,&delta);
        area=0;

        for(i=0;i<N_Inter;i++)
        {
            fprintf(histe,"%f\t%f\n",i*delta+min, H[i]);
            area+=H[i]*delta;
        }
        
        printf("Area del histograma de energias = %f\n",area);
        area=0;

        histograma(m,H,p.Nmed,&max,&min,&delta);

        for(i=0;i<N_Inter;i++)
        {
            fprintf(histm,"%f\t%f\n",i*delta+min, H[i]);
            area+=H[i]*delta;
        }

        printf("Area del histograma de magnetizacion = %f\n",area);

        fclose(hist);
        fclose(histe);
        fclose(histm);
    
    #endif //HISTOGRAMAS
    
    printf("\n--------------------FIN--------------------\n\n");
    
    return 0;
}