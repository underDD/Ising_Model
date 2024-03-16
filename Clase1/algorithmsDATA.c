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

            } else if (strcmp(token,"b_0")==0)
            {

                parameters->b_0=atof(strtok(NULL," "));

            } else if (strcmp(token,"b_f")==0)
            {

                parameters->b_f=atof(strtok(NULL," "));

            } else if (strcmp(token,"dB")==0)
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

                parameters->Nmc=atof(strtok(NULL," "));
            }

        }
    }

    fclose(fin);

    return 0;
}


/*

    Construye un histograma

*/
void Histograma(double *I,double *H,double *delta,double *mx,double *mn)
{

    int i,j;
    double max,min,d,norma;

    max=-100000000;
    min=+100000000;

    for(i=0;i<N_Inter;i++) H[i]=0;

    for(i=0;i<N_DATA;i++)
    {
        if (I[i]>max) max=I[i];
        if (I[i]<min) min=I[i];
    }

    d=(max-min)/N_Inter;
    norma=1.0/(N_DATA*d);

    for(i=0;i<N_DATA;i++)
    {
        j=(I[i]-min)/d;
        if (j==N_Inter) j=j-1;
        H[j]++;
        //printf("%d\n",j);
    }

    for(i=0;i<N_Inter;i++)
        H[i]*=norma;

    *delta=d;
    *mx=max;
    *mn=min;

}
