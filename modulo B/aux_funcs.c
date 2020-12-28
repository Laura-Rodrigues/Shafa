#include "moduloB.h"



/*
int numBlocos (FILE *f){
    char c ;
    int blocos=0;
    if(f==NULL) {
        printf("erro");
        fclose(f);
    }
    else (fscanf (f,"@%c@%d@", &c, &blocos));
    return blocos;
}


int tamBlocos (FILE *f, int bloco){
    int contador=0, rep = 2*bloco,tamBloco;
    char c ;
    if (f != NULL && bloco != 0){
        while ((c = fgetc(f)) != EOF && contador <= rep) {
            if (contador == rep) {
                fscanf(f,"%d@",&tamBloco);
            }
            if (c == '@') contador++;
        }
    }
    return tamBloco;
}
*/


void freqToArray (int freqs[], FILE *f) {
    char c, aux[10];
    int i=0, j=0, r=0;
    if (f != NULL){
        while ((c = fgetc(f)) != '@') {

                if ( c == ';'){
                    if ( r != 0 ) {
                        freqs[j] = atoi(aux);
                        j++;
                        r = 0;
                        i = 0;
                        for(int x=0;x<10;x++) aux[x]=0;
                    }
                    else{
                        freqs[j] = freqs[j - 1];
                        j++;
                        i = 0;
                    }
                }
                else{
                    aux[i] = c;
                    i++;
                    r++;
                }

        }
    }
}

void swap(int v[], int x, int y){
    int temp = v[x];
    v[x] = v[y];
    v[y] = temp;
}

void decrescenteSortSimb (int v[], int s[], int N){
    for(int i=0; i<N;i++) s[i]=i;
    for(int i=0; i<N-1; i++){
        for (int j=i+1; j<N; j++){
            if (v[i]<v[j]) {
                swap (v,i,j);
                swap (s,i,j);
            }
        }
    }
}

void swapChar(unsigned char *v[], int x, int y){
    unsigned char *temp = v[x];
    v[x] = v[y];
    v[y] = temp;
}


void crescenteSortSimb (unsigned char *v[], int s[], int N){
    for(int i=0; i<N-1; i++){
        for (int j=i+1; j<N; j++){
            if (s[i]>s[j]) {
                swap (s,i,j);
                swapChar (v,i,j);
            }
        }
    }
}


int soma(const int freq[], int i, int j){
    int sum = 0;
    for (; i <= j; i++)
        sum += freq[i];

    return sum;
}

int melhorDiv (int freq[],int i,int j){
    int mindif, dif, indice, div, g1, total;
    div=i; g1=0;
    total= mindif = dif = soma(freq,i,j);
    while (dif == mindif) {
        g1=g1+freq[div];
        dif=abs(2*g1-total);
        if (dif<mindif) {
            div=div+1;
            mindif=dif;
        }
        else dif=mindif+1;
    }
    indice = div-1;
    return indice;
}

void add_bit_to_code( char *bit, unsigned char *codes[], int start, int end) {
    for(int i =start; i <= end; i++) strcat(codes[i],bit);

}

void shannon (int freqs[], unsigned char *codes[], int start, int end){
    while (!freqs[end] && end>start) end--;
    if (start != end){
        int div = melhorDiv(freqs,start,end);
        add_bit_to_code("0",codes,start,div);
        add_bit_to_code("1",codes,div+1,end);
        shannon(freqs,codes,start,div);
        shannon(freqs,codes,div+1,end);
    }
}
