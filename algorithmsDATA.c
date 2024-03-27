#include "head1.h"

/*
    Lee de un fichero "flag" los datos necesarios para la simulación
*/
int loadParameters(Parameters *parameters) {

    char line[100];
    FILE *fin;
    fin=fopen("flag.txt","r");

    if (fin==NULL)  return -1;

    while (!feof(fin)) { // Recorre cada línea hasta el final del fichero.

        fgets(line,sizeof(line),fin); // Guarda la linea en line
        char *token = strtok(line," "); // El puntero token se posiciona donde aparece el primer espacio

        if (token != NULL) { // Se comprueba que ha leido al menos un espacio

            if (strcmp(token,"flag") == 0)
            { //Se compara el valor del token con el del nombre de la variable
                //strtok para encontrar el siguiente espacio (final de la frase) tomando asi el valor de la variable en char y se transforma a double con atof
                parameters->flag=atof(strtok(NULL," "));

            } else if (strcmp(token,"Beta_0")==0)
            {

                parameters->b_0=atof(strtok(NULL," "));

            } else if (strcmp(token,"Beta_f")==0)
            {

                parameters->b_f=atof(strtok(NULL," "));

            } else if (strcmp(token,"dBeta")==0)
            {

                parameters->dB=atof(strtok(NULL," "));

            } else if (strcmp(token,"Nterm")==0)
            {

                parameters->Nterm=atof(strtok(NULL," "));

            } else if (strcmp(token,"Nmed")==0)
            {

                parameters->Nmed=atof(strtok(NULL," "));

            } else if (strcmp(token,"Nmc")==0)
            {

                parameters->Nmc=atof(strtok(NULL,"\n"));
            }

        }
    }

    fclose(fin);

    return 0;
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


double e2(double e)
{
    return e*e;
}

double m2(double m)
{
    return m*m;
}


double mean(double *DATA,int NDATA)
{
    int i;
    double suma;

    suma=0;

    for(i=0;i<NDATA;i++)
    {
        suma+=DATA[i];
    }

    return suma/(double)NDATA;

}

double Var(double *DATA, int NDATA)
{
    int i=0;
    double suma;
    double media;

    suma=0;
    media=mean(DATA,NDATA);

    for(i=0;i<NDATA;i++)
    {
        suma+=pow((DATA[i]-media),2);
    }

    return suma/(double)NDATA;
}   

void histograma(double *DATA,double *H, int NDATA, double *maxi, double *mini, double *delta)
{

    int i,j;

    double deltaa,max,min;
    double norma;

    max = -100000000;
    min = +100000000;

    for(i=0;i<NDATA;i++)
    {
        if(DATA[i] > max) max = DATA[i];
        if(DATA[i] < min) min = DATA[i];
    }

    deltaa = (max-min)/N_Inter;
    norma = 1/(NDATA*deltaa);

    for(i=0;i<NDATA;i++)
    {
        j=(DATA[i]-min)/deltaa;
        if (j==N_Inter) j--;

        H[j]++;
    }

    for(i=0;i<N_Inter;i++)
    {
        H[i]*=norma;
    }

    *maxi = max;
    *mini = min;
    *delta = deltaa;

}

double Cv(double *e, int NDATA)
{
    return 2*V*NDATA*Var(e,NDATA);
}

double X(double *m, int NDATA)
{
    int i;
    double mm[NDATA];

    for(i=0;i<NDATA;i++)
    {
        mm[i]=fabs(m[i]);
    }

    return V*NDATA*Var(mm,NDATA);    
}

void calculos(double *e,double *m, int NDATA, double *results)
{

    results[0] = mean(e,NDATA);
    results[1] = mean(m,NDATA);
    results[2] = Cv(e,NDATA);
    results[3] = X(m,NDATA);

}



