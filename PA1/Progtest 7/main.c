#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct list {
    char *array;
    int size;
    int width;
    int used;
}LIST_t;

typedef struct results{
    char **array;
    int size;
    int used;
}RESULTS_t;

void initResults(RESULTS_t *results, int size){
    results->size=size;
    results->used=0;
    results->array=(char **)malloc(size*sizeof(char*));

}

void addResults(RESULTS_t *results, char *pointer ){
    if(results->size == results->used){
        results->size = results->size*2;
        results->array = (char**)realloc(results->array, results->size*sizeof(char*));
    }
    results->array[results->used++]= pointer;
}

void initArray(LIST_t *list, int sizeOfArray, int count){
    int i;
    list->size = sizeOfArray*count;
    list->used = 0;
    list->width = count;
    list->array = (char*) malloc(list->size*sizeof( char));
    for(i =0; i < list->size;++i){
        list->array[i]='$';
    }
}

void parcelsGenerator(RESULTS_t *results, int n,char parcel, char **list,int length, int listOffest, int width, char lastParcel, int modul, int NumberMP,int NumberMC,int NumberMI, int *count){
    char *tmp;
    tmp = *list+listOffest; 
    
    if(parcel != 'P' && NumberMP < 0)
        return;
    if(parcel == 'C'){
        if(NumberMC >=modul)
            NumberMC=0;  
        else
            return;
                  
    }
    if(lastParcel == 'I' && parcel !='I' && NumberMI <=1)
        return;
    if(lastParcel =='I' && parcel !='I')
        NumberMI=0;    
    if(parcel == 'I' && n==0 && lastParcel != 'I')
        return;    

    if(n ==0){
        tmp[0] = parcel;
        *count +=1;
        addResults(results, &tmp[0]);
        return;
    }
    int i,j=n;
    int max= pow(4,n);
    for (i=0; i< max; ++i){
        tmp[j]= parcel;
         j+=width; 
    }
   
    parcelsGenerator(results,n-1, 'P',&tmp, length/4, 0,            width,parcel, modul, modul,NumberMC+1,NumberMI, count);
    parcelsGenerator(results,n-1, 'R',&tmp, length/4, length/4 ,    width,parcel, modul, NumberMP-1,NumberMC+1,NumberMI, count);
    parcelsGenerator(results,n-1, 'C',&tmp, length/4, 2*(length/4) ,width,parcel, modul, NumberMP-1,NumberMC,NumberMI, count);
    parcelsGenerator(results,n-1, 'I',&tmp, length/4, 3*(length/4) ,width,parcel, modul, NumberMP-1,NumberMC+1,NumberMI+1, count);  
 }

void callGenerator(RESULTS_t *results,int n, char **list, int length, int width,int modul, int *count) {
    parcelsGenerator(results,n-1, 'P',list, length/4, 0, width,'@',modul,modul,modul,0, count);
    parcelsGenerator(results,n-1, 'R',list, length/4, length/4,width,'@',modul,modul-1,modul,0, count);
    parcelsGenerator(results,n-1, 'C',list, length/4, 2*(length/4) ,width,'@',modul,modul-1,modul,0, count);
    if(n !=1)
        parcelsGenerator(results,n-1, 'I',list, length/4, 3*(length/4),width,'@',modul,modul-1,modul,1, count);
 }

void printList(LIST_t *list){
    int i;
     printf("[");
    for(i=0; i < list->size; ++i){
            printf("%c", list->array[i] );
        if(((i+1) % (list->width)) == 0  ){
            printf("]");
            printf("\n");
            if(list->size-1 > i)
            printf("[");
        }   
    }
}
void printResults(RESULTS_t *results, int width){
    int i,j;
    for(i=0; i < results->used; ++i){
        printf("[");
        for(j=0; j <width;++j){
            printf("%c", results->array[i][j]);
        }
        printf("]\n");
    }
}

int countParcels(int model, int length, int count) {
    return 0;
}

void freeArray(LIST_t *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}
void freeResults(RESULTS_t *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

int main (void){
    int modul,count,err;
    int result=0;
    LIST_t list;
    RESULTS_t resultsList;
    printf("Modul:\n");
    if( 1 != scanf("%d", &modul) || modul < 1 || modul >10 || modul != (int)modul ){
        printf("Nespravny vstup.\n");
        return 0;
    } 
    printf("Vypocty:\n");
    char inputLIne[5];
    while((err =scanf("%s%d", inputLIne, &count)) == 2 ){
        result=0;      
        if((strcmp(inputLIne, "count")!=0 && strcmp(inputLIne, "list")!=0) || count < 1 || count != (int)count ){  
                printf("Nespravny vstup.\n");
                return 0;    
        }
        int sizeOfArray = pow(4, count);   
        initArray(&list, sizeOfArray, count);
        initResults(&resultsList, 10);
        callGenerator(&resultsList,list.width, &list.array, list.size, list.width, modul, &result);
        if(strcmp(inputLIne, "list")==0){
            printResults(&resultsList, count);
        }   
        printf("=> %d\n", result);
        freeArray(&list);
        freeResults(&resultsList);
       
    }
    if(err != EOF){
        printf("Nespravny vstup.\n");
        return 0;
        }
     return 0;
}


