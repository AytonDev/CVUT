#include <stdio.h>
#include <math.h>


int numberOfBlocks(int limit, int min){
    int i,j,k,jednotky,desitky,stovky,counter=0;
    int soucet =0;
    int sqrtMax = sqrt(limit-1);
    int sqrtDec = sqrt(limit-1)*2;

    for(k=0;k< sqrtMax;++k) {
        stovky= k+1;
        for (j = 0+k; j <sqrtDec; ++j) {
            desitky = j + 1;
            for (i = 0 + j; i < limit-1; ++i) {
                jednotky = i + 1;
                soucet=stovky*desitky*jednotky;
                if(soucet>limit)
                    break;
                if(min<= soucet && soucet <=limit){
                    counter++;
                }
            }

        }

    }
    return counter+1;

}

int main() {
    int limit,min,counter,err;
    char start, end, semicolon;

    printf("Intervaly:\n");
    while( (err =scanf(" %c %d %c %d %c", &start, &min, &semicolon,&limit,&end)) == 5) {
        if(   min <=0 || limit<=0 || limit >500000 || min> limit || start != '<'||
            end != '>' || semicolon != ';'){
            printf("Nespravny vstup.\n");
            return 0;
        }
        counter = numberOfBlocks(limit, min);
        printf("Ruznych kvadru: %d\n", counter);
    }
    if(err != EOF){
        printf("Nespravny vstup.\n");
    }

}
