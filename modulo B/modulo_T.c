#include <stdio.h>
#include <stdlib.h>
#include "moduloB.h"
#include <time.h>


void modulo_T (char *ficheiroFreq, char *ficheiroCod){
    FILE *f = fopen(ficheiroFreq, "rb");
    if(f==NULL) {
        fputs ("File error",stderr);
        exit (1);
    }
    else{
        int b=1;
        FILE *cod=fopen(ficheiroCod, "wb");
        unsigned long blocos;
        char rn;
        fscanf(f,"@%c@%lu@",&rn,&blocos);
        fprintf(cod,"@%c@%lu",rn,blocos);

        while(b <= blocos){

            unsigned long tamBloco;
            fscanf(f,"%lu@",&tamBloco);
            fprintf (cod, "@%lu@",tamBloco);

            int *freqs = malloc(256 * sizeof(int)), *simbolos = malloc(256 * sizeof(int));
            freqToArray(freqs,f);
            decrescenteSortSimb(freqs,simbolos,256);


            unsigned char *codes[256];
            for(int i=0;i<256;i++) {
                codes[i]=(unsigned char *)malloc(sizeof(unsigned char *)*256);
                //*(codes[i])=NULL;
            }

            shannon(freqs,codes,0,255);

            crescenteSortSimb(codes, simbolos, 256);

            for (int i = 0; i < 255; i++) fprintf(cod, "%s;", codes[i]);
            fprintf(cod, "%s", codes[255]);

            free(simbolos);
            free(freqs);
            b++;
        }
        fprintf(cod,"@0");
        fclose(f);
        fclose(cod);
    }
}

int main() {
    char freq[] = {"aaa.txt.rle.freq"};
    char freq2[] = {"aaa.txt.rle.freq"};


    char filename[30], extension[8], txt[3],aux[30];
    strcpy(aux,freq);
    sscanf(freq,"%[^.].%s",filename,extension);
    sscanf(extension,"%[^.].%s",txt,extension);



    if(strcmp (extension,"freq") == 0) {
        strcat(filename,".txt.cod(2)");
    }
    else if(strcmp (extension,"rle.freq") == 0) {
        strcat(filename,".txt.rle.cod(2)");
    }
    else printf("Wrong extension - ");


    clock_t begin = clock();
    modulo_T(aux,filename);






    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    time_t now = time ( NULL );
    struct tm *date = localtime ( &now );

    printf("Cláudia Silva, a93177, Laura Rodrigues, a93169, MIEI/CD, %d-%d-%d\n",date->tm_mday,date->tm_mon + 1,date->tm_year + 1900);
    //printf("Módulo: t (cálculo dos códigos dos símbolos)\nNúmero de blocos: %d",c);
    printf("Tamanho dos blocos analisados no ficheiro de símbolos: bytes\n"); //adicionar função tamBlocos -- Modulo D (Pinto/Gonçalo)
    printf("Tempo de execução do módulo (milissegundos): %f ms\n", time_spent*1000);
    printf("Ficheiro gerado: ");
     /* Texto de saída na consola:*/
    return 0;
}
