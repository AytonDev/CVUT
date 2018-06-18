#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define PI 3.141592654

int main() {
    double a, b, c, x, uhloprickaAB, uhloprickaAC, uhloprickaCB, uhloprickaCtverce;

    printf("Velikost otvoru:\n");
    if( scanf("%lf", &x) != 1 || x <= 0 ) {
        printf("Nespravny vstup.\n");
        return 0;
    };
    printf("Velikost kvadru:\n");
    if(scanf("%lf%lf%lf",&a,&b,&c) !=3 || a <=0 || b <=0 || c<=0) {
        printf("Nespravny vstup.\n");
        return 0;
    }

    uhloprickaAB = sqrt(a*a + b*b);
    uhloprickaAC = sqrt(a*a + c*c);
    uhloprickaCB = sqrt(c*c + b*b);
    uhloprickaCtverce = sqrt(x*x + x*x);

    //Trojuhelnik
    if( (atan(c/(x/2-a/2))*180/PI <=60 && a < x)  || (atan(a/(x/2-b/2))*180/PI <=60 && b < x ) || (atan(b/(x/2-c/2))*180/PI <= 60 && c < x )){
        printf("Trojuhelnik: ano\n");
    }else {
        printf("Trojuhelnik: ne\n");
    }

    //KRUH
    if(uhloprickaAB < x || fabs(uhloprickaAB-x) < DBL_EPSILON*(uhloprickaAB+x)/2 || uhloprickaAC < x|| fabs(uhloprickaAC-x) < DBL_EPSILON*(uhloprickaAC+x)/2 || uhloprickaCB <= x || fabs(uhloprickaCB-x) < DBL_EPSILON*(uhloprickaCB+x)/2 ) {
        printf("Kruh: ano\n");
    } else{
        printf("Kruh: ne\n");
    }

    //Čtverec
    if((a <= x && b <= x) || (a <= x && c <= x) || (b <= x && c <= x)) {
        printf("Ctverec: ano\n");
    }
    else if( (uhloprickaCtverce-a)/2 >= b/2 || (uhloprickaCtverce-a)/2 >= c/2|| (uhloprickaCtverce-b)/2 >= a/2 ||
             (uhloprickaCtverce-b)/2 >= c/2 || (uhloprickaCtverce-c)/2 >= a/2|| (uhloprickaCtverce-c)/2 >= b/2) {
        printf("Ctverec: ano\n");
    }

    else {
        printf("Ctverec: ne\n");
    }
    return 0;
}