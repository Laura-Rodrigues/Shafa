#include "moduloB.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*Função que recebe um array freqs e um ficheiro.
* Esta lê os valores das frequências do ficheiro e guarda-as no array freqs.
*/
void freqToArray (int freqs[], FILE *f) {
    char c, aux[10];
    int i=0, j=0, r=0;
    if (f != NULL){
        while ((c = fgetc(f)) != '@') { //enquanto não chega ao final do bloco

                if ( c == ';'){ 
                    if ( r != 0 ) {
                        freqs[j] = atoi(aux);   // se chegar a um ; guarda a freq que tinha sido lida para aux na posição respetiva do array freqs
                        j++;
                        r = 0;
                        i = 0;
                        for(int x=0;x<10;x++) aux[x]=0; // zera o que se encontrava na aux 
                    }
                    else{
                        freqs[j] = freqs[j - 1];    //se o ficheiro tiver ; seguidos repete a frequência anterior
                        j++;
                        i = 0;
                    }
                }
                else{
                    aux[i] = c;    //vai guardando em aux a frequência que está a ler carater a carater
                    i++;
                    r++;
                }

        }
    }
}
/* Função que troca valores de um array de inteiros (auxiliar da decrescenteSortSimb) */
void swap(int v[], int x, int y){
    int temp = v[x];
    v[x] = v[y];
    v[y] = temp;
}

/* Função que ordena de forma decrescente as frequências e os símbolos correspondentes */
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
/* Função que troca valores de um array de strings (auxiliar da crescenteSortSimb) */
void swapChar(unsigned char *v[], int x, int y){
    unsigned char *temp = v[x];
    v[x] = v[y];
    v[y] = temp;
}

/* Função que ordena pela ordem ordem inicial (que tinha sido guardada através dos símbolos) a codes */
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

/* Soma os valores das frequências que estão no array entre duas dadas posições */
int soma(const int freq[], int i, int j){
    int sum = 0;
    for (; i <= j; i++)
        sum += freq[i];

    return sum;
}

/* Calcula a posição para a melhor divisão do array */
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

/* Adiciona um bit a cada string de cada posiçao no array ( usada para adicionar o bit 1 ou 0 ) */
void add_bit_to_code( char *bit, unsigned char *codes[], int start, int end) {
    for(int i =start; i <= end; i++) strcat(codes[i],bit);

}
/* Função que cria o código Shannon-Fano usando a melhorDiv para dividir o array na melhor posição.
* Recorrendo à add_bit_to_code adiciona o bit 0 à primeira parte desta divisão e o bit 1 à segunda parte e assim sucessivamente até codificar o array inteiro.
* As frequências nulas são ignoradas e passadas à frente.
*/
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



/* Corpo do módulo: lẽ do ficheiro .freq as informações necessárias para depois criar um novo ficheiro .cod e escrever nele. */
void modulo_T (char *ficheiroFreq, char *ficheiroCod){
    FILE *f = fopen(ficheiroFreq, "rb");
    if(f==NULL) {
        fputs ("File error\n",stderr); // se o file .freq estiver vazio ou não existir, p.e., se for dado um nome incorreto quando o módulo é chamado
        exit (1);
    }
    else{
        printf("Módulo: t (cálculo dos códigos dos símbolos)\n"); // texto da consola

        int b=1;
        FILE *cod=fopen(ficheiroCod, "wb");
        
        unsigned long blocos;
        char rn;
        fscanf(f,"@%c@%lu@",&rn,&blocos);
        fprintf(cod,"@%c@%lu",rn,blocos); //imprime no .cod a parte inicial (R/N e o número de blocos)
        printf("Número de blocos: %lu\n", blocos); // texto da consola
        printf("Tamanho dos blocos analisados no ficheiro de símbolos: "); // texto da consola

        
        while(b <= blocos){ // contador b permite executar este código para o número de blocos certo

            unsigned long tamBloco;
            fscanf(f,"%lu@",&tamBloco);
            fprintf (cod, "@%lu@",tamBloco); //imprime no .cod o tamanho do bloco que está a ser lido
            printf("%lu",tamBloco); // texto da consola
            
            int *freqs = malloc(256 * sizeof(int)), *simbolos = malloc(256 * sizeof(int));
            freqToArray(freqs,f);
            decrescenteSortSimb(freqs,simbolos,256);


            unsigned char *codes[256];
            for(int i=0;i<256;i++) {
                codes[i]=(unsigned char *)malloc(sizeof(unsigned char *)*256);
                *(codes[i])=NULL;
            }

            shannon(freqs,codes,0,255);

            crescenteSortSimb(codes, simbolos, 256);

            for (int i = 0; i < 255; i++) fprintf(cod, "%s;", codes[i]);
            fprintf(cod, "%s", codes[255]); //imprime no .cod os códigos shannon do bloco em questão

            free(simbolos);
            free(freqs);
            
            if(b<blocos) printf("/");  // texto da consola
            b++;
        }
        printf(" bytes");  // texto da consola
        fprintf(cod,"@0"); 
        fclose(f);
        fclose(cod);
    }
}

/* Função que verifica se a extensão do ficheiro está correta. Em caso afirmativo inicia a execução do módulo e a escrita do texto de consola */
int Tmain(char *freq_name) {
    char *cod_name, *freq_extension=".freq",
            *cod_extension=".cod", *extension;

    // Escrita da extensão correta no ficheiro cod que vai ser criado
    cod_name=(char *)malloc(strlen(freq_name)+1);

    extension=freq_name+strlen(freq_name)-strlen(freq_extension);

    if (strcmp(extension,freq_extension)) printf("\n Wrong extension! \n");

    else{
        strcpy(cod_name, freq_name);
        extension=cod_name+strlen(cod_name)-strlen(freq_extension);
        strcpy(extension,cod_extension);

        clock_t begin = clock();
        modulo_T(freq_name,cod_name);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; // para escrever na consola o tempo que demorou a executar o módulo

    // Primeira linha do texto da consola está na main do projeto    
    // Texto de saída na consola - última parte
    printf("Tempo de execução do módulo (milissegundos): %f ms\n", time_spent*1000);
    printf("Ficheiro gerado: %s\n", cod_name);
}  
    return 0;
}
