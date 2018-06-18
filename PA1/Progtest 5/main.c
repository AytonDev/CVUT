#include <stdio.h>
#include <stdlib.h>


 typedef struct {
    int *array;
    int size;
    int used;
}Array;

long long int gcd(long long int x, long long int y) {
    long long int a,b,tmp;
    if(x >= y){
        a = x;
        b = y;
    }else{
        a=y;
        b=x;
    }
    while(b !=0){
        tmp=a;
        a=b;
        b=tmp%b;
    }
    return a;
}

void initArray(Array *a, int initialSize) {
    a->array = (int *)malloc(initialSize*sizeof(int));
    a->used=0;
    a->size = initialSize;
}

void addToArray(Array *a, int element) {
    if(a->used == a->size){
        a->size= a->size*2;
        a->array = (int *)realloc(a->array,a->size*sizeof(int));
    }
     a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}


 
long long int lcm(long long int x, long long int y) {
    long long int result =  x/gcd(x,y)*y;
    return result;
}


long long int countCars(Array *a, int from, int to){
    int i;
    long long int tmp=0;
    if(to-from ==1)
        return a->array[from];
    tmp = lcm(a->array[from], a->array[from+1]);    
    for(i =from+2; i <to; ++i){
        if(tmp % a->array[i] != 0)
        tmp = lcm(tmp, a->array[i]);
    }
    
    return tmp;
}

int main(int argc, char *argv[]){
    int err,line, from, to;
    char first, comma;
    printf("Pocty pruhu:\n");
    Array a;
    initArray(&a, 10);  // initially 5 elements
     /*
    for (i = 0; i < 25; i++)
        addToArray(&a, i);  // automatically resizes as necessary

   
    printf("%d\n", a.array[25]);  // print 10th element
    printf("%d\n", a.used);  // print number of elements    
    printf("%d\n", a.size);  // print number of elements    
    printf("%ld\n", cars(&a, 10, 25));  // print number of elements  
    */ 

   while (scanf("%c", &first) == 1){
        if( first == '{')
            break;
        else if(first != ' ' && first != '\n'){
            printf("Nespravny vstup.\n");
            freeArray(&a);
            return 0;
        }
    }
    while((err =scanf(" %d %c", &line, &comma)) ==2) {
        if(line <= 0){
            printf("Nespravny vstup.\n");
            freeArray(&a);
            return 0;
        }
        addToArray(&a, line);
        if(comma != ','  && comma !='\n' && comma != '}'){
            printf("Nespravny vstup.\n");
            freeArray(&a);
            return 0;
        }
        else if(comma == '}'){
            printf("Trasy:\n");
            while((err =scanf("%d %d", &from, &to))==2) {
                if(from >=to || a.used < to || from <0 || to <0){
                    printf("Nespravny vstup.\n");
                    freeArray(&a);
                    return 0;

                }
                 printf("Vozidel: %lld\n", countCars(&a, from, to));
            }
            if(err != EOF){
             printf("Nespravny vstup.\n");
             freeArray(&a);
             return 0;
            }
        }
     }
     freeArray(&a);
     if(err != EOF){
             printf("Nespravny vstup.\n");
             return 0;
    }

    return 0;
}
