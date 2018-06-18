#include <stdio.h>

int main() {
    int pole[250000];

    int i,j, err, input, length =0 , between=0, possibilities=0, lastPosition=0, lowest;

    printf("Hodnoty:\n");
    while ((err = scanf ( "%d", &input)) == 1 )
    {
        pole[length]=input;
        length++;
    }
    if(err != EOF || length <2 || length > 250000){
        printf("Nespravny vstup.\n");
        return 0;
    }
    int results[250000];
    lowest= pole[0];
    for(i=0; i < length; ++i){
        if(lowest < pole[i] && i !=0)
            continue;
        else
            lowest = pole[i];
        for(j =length-1; j > i; --j){
            if(pole[i] <= pole[j]){
                if(between < j-i+1){
                    between=j-i+1;
                    possibilities =1;
                    results[0]=i;
                    results[1]=j;
                    lastPosition=2;
                }else if(between == (j-i+1)){
                    possibilities++;
                    results[lastPosition]= i;
                    results[lastPosition+1] = j;
                    lastPosition = lastPosition+2;
                }
            }
            if(j-i+1 < between)
                break;
        }
        if(length - i < between)
            break;
    }

    if(possibilities !=0){
        for(i=0; i < lastPosition; i=i+2){
            printf("%d: %d - %d\n",between, results[i], results[i+1]);
        }
        printf("Moznosti: %d\n",possibilities);
    }
    else
        printf("Nelze najit.\n");
    return 0;
}