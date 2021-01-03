#include <stdio.h>
#include <stdlib.h>
#include "moduloB.h"
#include <time.h>

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

int Tmain(char *freq_name) {
    char *cod_name, *freq_extension=".freq",
            *cod_extension=".cod", *extension;

    //escrita da extensão correta no ficheiro cod que vai ser criado
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

    // Texto de saída na consola
    printf("Tempo de execução do módulo (milissegundos): %f ms\n", time_spent*1000);
    printf("Ficheiro gerado: %s\n", cod_name);
}  
    return 0;
}
