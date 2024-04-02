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
                //strtok para encontrar el siguiente espacio (final de la frase) tomando asi el valor de la variable en char y se transforma a float con atof
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


float e2(float e)
{
    return e*e;
}

float m2(float m)
{
    return m*m;
}


float mean(float *DATA,int NDATA)
{
    int i;
    float suma;

    suma=0;

    for(i=0;i<NDATA;i++)
    {
        suma+=DATA[i];
    }

    return suma/(float)NDATA;

}

float Var(float *DATA, int NDATA)
{
    int i=0;
    float suma;
    float media;

    suma=0;
    media=mean(DATA,NDATA);

    for(i=0;i<NDATA;i++)
    {
        suma+=pow((DATA[i]-media),2);
    }

    return suma/(float)NDATA;
}   

void histograma(float *DATA,float *H, int NDATA, float *maxi, float *mini, float *delta)
{

    int i,j;

    float deltaa,max,min;
    float norma;

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

float Cv(float *e, int NDATA)
{
    return 2*V*Var(e,NDATA);
}

float X(float *m, int NDATA)
{
    int i;
    float mm[NDATA];

    for(i=0;i<NDATA;i++)
    {
        mm[i]=fabs(m[i]);
    }

    return V*Var(mm,NDATA);    
}

void bloques(float *DATA, int NDATA, int size, float *med, float *error, float *med2, float *error2, int iden) 
{
    int i,j;
    int Nblock;

    float suma;
    float suma2;

    Nblock = (int)(NDATA/size);
    float x2[Nblock];
    float x[Nblock];
    float PEDO[Nblock];

    for(i=0;i<Nblock;i++)
    {

        suma = 0;
        suma2 = 0;
        for(j=0;j<size;j++)
        {
            suma+= DATA[i*size+j];
            suma2+= DATA[i*size+j]*DATA[i*size+j];
        }

        x2[i] = suma2/size;
        x[i] = suma/size;

        
        if (iden == 1)
            PEDO[i]=2*V*(x2[i]-x[i]*x[i]);
        else
            PEDO[i]=V*(x2[i]-x[i]*x[i]);

    }

    *med2 = mean(PEDO, Nblock);
    *error2 = sqrt(Var(PEDO, Nblock));
    
    *med = mean(x,Nblock);
    *error = sqrt(Var(x,Nblock));
    
}