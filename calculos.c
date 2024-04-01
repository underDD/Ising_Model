#include "head1.h"

int main()
{

    Parameters p; 
    loadParameters(&p);

    float e[p.Nmed], m[p.Nmed];
    float beta;
    float em, mm, Cvm, Xm;

    FILE *fin;
    FILE *fout;

    int i,j;
    int hyst;
    int nFich;

    char name[MAX_STR_LEN];

    nFich = (int)((p.b_f-p.b_0)/p.dB);
    beta = p.b_0;
    hyst = 0;
    j = 0;

    fout = fopen("results/results.txt","wt");

    for(i=0;i<nFich;i++)
    {
         
        sprintf(name,"results/med_%d_%d_%.2f.txt",hyst,L,beta);
        fin = fopen(name,"rt");
        em = mm = Cvm = Xm = 0;

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
        fprintf(fout, "%f\t%f\t%f\t%f\t%f\n", beta, em, mm , Cvm, Xm);

        beta += p.dB;
        fclose(fin);

    }

    printf("FIN");
    fclose(fout);
    return 0;
}