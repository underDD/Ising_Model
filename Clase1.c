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
    printf("La energia del sistema es: %lf\n", energia(xp,yp,S));


}
