#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct line {
    char *array;
    int size;
    int used;
  
}LINE_t;

typedef struct text {
    LINE_t * array;
    int size;
    int used;
  
}TEXT_t;

typedef struct results {
    int * array;
    int size;
    int used;
  
}RESULTS_t;

void lineInit(LINE_t *line, int initialSize){
    line->size= initialSize;
    line->used=0;
    line->array = (char *) malloc(initialSize*sizeof(char)); 

}

void resultsInit(RESULTS_t *results, int initialSize){
    results->size= initialSize;
    results->used=0;
    results->array = (int *) malloc(initialSize*sizeof(int)); 

}

void textInit(TEXT_t *text, int initialSize){
    int i;
    text->array = (LINE_t *)malloc(initialSize*sizeof(LINE_t));
    text->size=initialSize;
    text->used=0;
    for(i=0; i <initialSize; ++i){
        text->array[i].size=initialSize;
        text->array[i].used=0;
        text->array[i].array = (char *) malloc(initialSize*sizeof(char)); 
    }

}
void newLine(TEXT_t *text){
    if(text->used == text->size-1){
        int size = text->size, i;
        text->size = text->size*2;
        text->array = (LINE_t *) realloc(text->array, text->size*sizeof(LINE_t));
        for(i=size; i < text->size; ++i){
            text->array[i].size=20;
            text->array[i].used=0;
            text->array[i].array = (char *) malloc(20*sizeof(char)); 
        }
    }
    text->used++;
}
void addText(TEXT_t *text, char letter){
    if(text->array[text->used].size == text->array[text->used].used){
        text->array[text->used].size = text->array[text->used].size*2;
        text->array[text->used].array =(char *)realloc(text->array[text->used].array,text->array[text->used].size*sizeof(char));
    }
    text->array[text->used].array[text->array[text->used].used++] = letter;
}

void addTextLine( LINE_t *line, char letter){
     if(line->size == line->used){
        line->size = line->size*2;
        line->array =(char *)realloc(line->array, line->size*sizeof(char));
    }
    line->array[line->used++] = letter;
}

void addResult( RESULTS_t *results, int value){
     if(results->size == results->used){
        results->size = results->size*2;
        results->array =(int *)realloc(results->array, results->size*sizeof(int));
    }
    results->array[results->used++] = value;
}

int readSearchTerm(LINE_t *searchTerm){
    char input;
    int space= 0;
    int charInput = 0;
    while((input = getchar()) != '\n'){
        if(input == EOF && charInput) 
            return 1;
        if(input == EOF)
            return 2;    
         if(!isalpha(input)&& space ){
            space = 0;
            addTextLine(searchTerm, ' ');    
            continue;
           
        }
        if(isalpha(input) ){
             
            charInput=1;
            input = tolower(input);
            addTextLine(searchTerm, input);
            space = 1;
        }
    }
      if(!charInput){
                printf("Neplatny dotaz\n");
                return 3;
            }
        if(searchTerm->array[searchTerm->used-1] == ' ')    
           searchTerm->used--;
    return 1;
    
}
int readLine(TEXT_t *text) {
    char input;
    int newSpace=1;
    int newOne=0;
    char lastChar='B';
    while( (input = getchar()) != EOF) {
        if(input == '\n'){
            if(newOne==1){
                return 1;
                
            }
                
            if(lastChar != ' '){
                addText(text, ' ');    
                lastChar=' ';
             } 
            newLine(text);
            newOne++;
            newSpace=1;
            continue;
        }
        if(!isalpha(input) && newSpace){
            newSpace = 0;
            newOne=0;
            if(lastChar != ' '){
                addText(text, ' ');    
                lastChar=' ';
             } 
            continue;
        }
        if(isalpha(input) ){
            lastChar='A';
            input = tolower(input);
            addText(text, input);
            newSpace = 1;
            newOne=0;
        }
            
    }
        printf("Nespravny vstup.\n");
        return 0;

}

void find(TEXT_t *text, LINE_t *searchTerm, RESULTS_t *results) {
    int i ,j,col=0, lineNumber=-1, found = 0;
   for(i=0; i < text->used; ++i) {
       for(j=0; j < text->array[i].used; ++j){
           if(text->array[i].array[j] == searchTerm->array[col]){
               if(col != 0)
                col++;
               else if(col == 0 && j ==0){
                col++;
               }
               else if(col ==0 && j !=0 && text->array[i].array[j-1] == ' '){
                    col++;
               }
                   
               if(lineNumber == -1){
                    lineNumber = i;
               }          
              if(searchTerm->used == col && text->array[i].array[j+1] == ' '){
                  found=1;
                  addResult(results, lineNumber +1);
                  col=0;
              } 
           } 
           else{
               col=0;lineNumber=-1;
           }
       }
   }
   if(!found)
    printf("Nenalezeno\n");
    else {
        printf("Nalezeno: ");
        printf("%d", results->array[0]);
        for(i = 1; i < results->used; ++i) {
           printf(", %d", results->array[i]);
        }
        printf("\n");
    }
    results->used= 0;
    searchTerm->used = 0;
}

void freeText(TEXT_t *a) {
    int i;
    for(i=0; i < a->size; ++i){
        free(a->array[i].array);
    }
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;

}
void freeLine(LINE_t *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}
void freeResults(RESULTS_t *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}


int main(void) {
    TEXT_t text;
    LINE_t searchTerm;
    RESULTS_t results;
    int readResponse, firstResponse=0;
    textInit(&text, 5);
    lineInit(&searchTerm, 10);
    resultsInit(&results, 5);
    printf("Text:\n");
    if(!readLine(&text)){
         freeText(&text);
         freeLine(&searchTerm);
         freeResults(&results);
         return 0;
    }
        
    printf("Hledani:\n");
    while( (readResponse =readSearchTerm(&searchTerm))){
       
        if(readResponse ==1){
             find(&text, &searchTerm, &results);
             firstResponse =1;
        }
        else if(readResponse == 2 && firstResponse){
            break;
        }
        else if( readResponse == 2 && !firstResponse){
           printf("Nespravny vstup.\n");
           break;
        }
        else if(readResponse == 3){
            
            firstResponse = 1;
        }
            
           
    }
    freeText(&text);
    freeLine(&searchTerm);
    freeResults(&results);
    return 0;
    
}


