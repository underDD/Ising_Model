#include "head1.h"

int main(){

    srand(time(NULL));

    int S[V];
    int xp[L],yp[L],xm[L],ym[L];
    //double H[N_DATA];
    Parameters p;

    loadParameters(&p);
    genconfig(S,p);
    offsets(xp,yp,xm,ym);
    saveconfig(S);
    printf("La energia de la configuracion es: %lf\n", energia(xp,yp,S));
    printf("La magnetizacion de la configuracion es: %lf\n", magneto(S));

    return 0;
    
}
