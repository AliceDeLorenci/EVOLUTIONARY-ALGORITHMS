#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(){
    FILE *Arq;
    Arq = fopen("./generation.txt","r");
    if(Arq == NULL){
        printf("WARNING");
        exit(0);
    }

    int media = 0;
    int n = 0;
    int temp;

    while(feof(Arq) == 0){
        fscanf(Arq,"%d",&temp);
        media += temp;
        n++;
    }
    media = media/n;
    printf("%d",media);
}