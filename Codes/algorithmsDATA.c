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

                parameters->Nmc=atof(strtok(NULL," "));
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
