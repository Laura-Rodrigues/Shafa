
/*
    Módulo F - parte do trabalho shafa grupo 16, UC de Comunicação de Dados MIEI 2020-2021
    Período de desenvolvimento de trabalho: 2 de dezembro de 2020 a 3 de janeiro de 2021
    Autores (por ordel alfabética):
    Francisco Toldy   a93226
    Mariana Rodrigues a93306;

    Este módulo inclui funções como "toRLE": compressão RLE; "gravarFrequências": grava as frequências no file .freq; "calculaFreqs": calcula a frequẽncia de cada byte; "inicializaFile": criação dos files que serão output do programa;
 "funcaoFREQ": função auxiliar para obter os ficheiros .freq; "printTerminal": escrita das informações no terminal; "moduloF": principal função do modulo.
*/



#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "ModuloF.h"
#include "string.h"
#include "fsize.h"


float compressao (unsigned long antes, unsigned long depois){
    float c = (((antes*1.0) - (depois*1.0)) / (antes*1.0));
    return c;
}


float fazerRLE ( unsigned long bytes_lidos, unsigned long tamArrRLE){
    int result = 0;
    float aux = compressao(bytes_lidos, tamArrRLE);
    if ( aux > 0.05 ) result = 1;
    return result;
}


unsigned long toRLE(unsigned char bloco[],unsigned long tamBlock,unsigned char *arr){ 
    int r = 1;
    unsigned long j = 0;
    for ( unsigned long i = 0; i<tamBlock ;i++){
        if (bloco[i] == bloco[i+1] && r != 255) r++;
        else if  (r == 255 || bloco[i] == 0 || r > 3)  { 
            arr[j]=0;
            arr[j+1] = bloco[i];
            arr[j+2]  = r;
            r = 1;
            j+=3;
        }
        else{
            while (r>=1){
                arr[j] = bloco[i];
                j++;
                r--;
            }
            r=1;
        }       
    }
    return j;
}

void gravarFrequencias (FILE *fp, unsigned long tamBloco, unsigned long *freq){
    if (fp != NULL){
        fprintf ( fp, "@%lu@",tamBloco);
        for (int i = 0; i < 256; i++) {
            if ( i == 255 && freq[i] == freq[i-1]);
            else if (i > 0 && freq[i] == freq[i-1]) fprintf(fp, ";");
            else if (i == 255) fprintf(fp, "%lu",freq[i]);
            else fprintf(fp, "%lu;", freq[i]);
        }
    }
}

void calculaFreqs (unsigned char *bufferBloco, unsigned long *freq, unsigned long tamBloco){
    unsigned long i;
    for (i = 0; i < tamBloco; i++){
        freq[bufferBloco[i]]++;
    }
}

void inicializaFile (char *nomefile, int modulo, char *nomeFinal , long long nblock){
    strcpy(nomeFinal, nomefile); 
    if (modulo == 0) strcat(nomeFinal, ".rle");
    if (modulo == 1) strcat(nomeFinal, ".rle.freq");
    if (modulo == 2) strcat(nomeFinal, ".freq");
    FILE *fp = fopen(nomeFinal, "w+");
    if (fp != NULL){
        if (modulo == 1){
            fprintf(fp, "@R");
        }
        if (modulo == 2){
            fprintf(fp, "@N");
        }
        if ( modulo == 1 || modulo == 2 ) fprintf(fp,"@%lld",nblock);
        fclose (fp);
    }    
}

unsigned long long  tamfile (char *file, unsigned long *tamBlock){
    long size_of_last_block;
    FILE *fp = fopen (file, "rb");
    long long nblock = fsize (fp, NULL, tamBlock ,&size_of_last_block);
    fclose(fp);
    unsigned long long sizefile = (nblock-1) * (*tamBlock) + size_of_last_block;
    return sizefile;
}


void funcaoFREQ (FILE *fileF, unsigned long tamanho, unsigned char *bloco){
    unsigned long freq[256];
    for (int i = 0; i < 256; i++) freq[i] = 0;
    calculaFreqs(bloco,freq, tamanho);
    gravarFrequencias(fileF, tamanho, freq );
}


void printfTerminal (unsigned long *arrayTamRLE, long long nblock, unsigned long tamBlock, unsigned long size_of_last_block, char *fileFREQ, char *fileRLE, int obrigatorio,  double time_spent, unsigned long long sizefile ){
    time_t now = time ( NULL );
    struct tm *date = localtime (&now);

    printf("Francisco Toldy,a93226; Mariana Rodrigues,a93306 MIEI-CD,%d-%d-%d\n",date->tm_mday,date->tm_mon + 1,date->tm_year + 1900);
    printf("Módulo: f \nNúmero de blocos: %lld\n", nblock);
    printf("Tamanho dos blocos analisados: %lld blocos de %lu bytes + 1 bloco de %lu bytes\n", nblock-1, tamBlock, size_of_last_block); 
   
    if ( obrigatorio == 1){
        unsigned long long sizefileRLE = tamfile (fileRLE, &tamBlock);
        float compFile = compressao(sizefile,sizefileRLE);
        printf("Compressão RLE: %s (%0.2f %% compressão)\n",fileRLE, compFile*100);
        printf("Tamanho dos blocos analisados no ficheiro RLE:");
        for ( int i = 0; i < nblock; i++){
        if (i == nblock-1) printf("%lu\n", arrayTamRLE[i]);
        else printf("%lu/",arrayTamRLE [i]);
        }
    }
    printf("Tempo de execução do módulo:%0.2f ms\n", time_spent*1000);//função que calcula o tempo de execução
    printf("Ficheiros gerados: %s \n", fileFREQ);
}


int moduloF (char *fileInicial, unsigned long tamBlock, int *obrigatorioRLE){
    clock_t begin = clock();
    char fileFREQ[100], fileRLE[100];
    long size_of_last_block;
    unsigned long bytes_lidos, tamRLE;
    FILE *fileI = fopen (fileInicial, "rb");
    long long nblock = fsize (fileI, NULL, &tamBlock ,&size_of_last_block);    // cálculo do numero de blocos
    unsigned long long sizefile = (nblock-1) * tamBlock + size_of_last_block;  // tamanho do ficheiro
    unsigned char *auxRLE,*bufferBloco;  
    if (nblock > 1 && size_of_last_block < 1024){                              // condição para quando o ultimo bloco é menor que 1 Kb
        nblock--;
        size_of_last_block += tamBlock;
    }
    unsigned long arrayTamRLE[nblock];                                         // o array que regista o tamanho dos blocos da compressão RLE, para ser posteriormente apresentado no terminal
    if ( size_of_last_block > tamBlock ) bufferBloco = (unsigned char*)malloc(size_of_last_block * sizeof(unsigned char));
    else bufferBloco = (unsigned char*)malloc(tamBlock * sizeof(unsigned char));
    if (nblock == 1) bytes_lidos = fread (bufferBloco,sizeof(unsigned char), sizefile,fileI); 
    else bytes_lidos = fread (bufferBloco,sizeof(unsigned char), tamBlock,fileI);                  // leitura do bloco para o buffer
    auxRLE = (unsigned char *)malloc(bytes_lidos*3*sizeof(unsigned char));                         // alocação de memória para a compressão RLE
    tamRLE = toRLE (bufferBloco, bytes_lidos, auxRLE);                                             // compressão RLE é feita e o tamanho do bloco resultante é guardado em tamRLE
    if (*obrigatorioRLE == 1 || fazerRLE(bytes_lidos,tamRLE)){ 
        inicializaFile(fileInicial, 0, fileRLE, nblock);                                           //Criação dos novos files
        FILE *fileR = fopen (fileRLE, "wb");
        inicializaFile(fileInicial, 1, fileFREQ, nblock);
        FILE *fileF = fopen (fileFREQ, "a");
        fwrite(auxRLE,1,tamRLE,fileR);                                                             // primeiro bloco é guardado no ficheiro
        arrayTamRLE[0] = tamRLE;                                                                   // tamanho do primeiro bloco é anotado no array
        funcaoFREQ(fileF, tamRLE, auxRLE);
        for ( int i = 1; i < nblock; i++){                                                         // ciclo para o resto dos blocos, segue a seguinte estrutura:
            if ( size_of_last_block > tamBlock && i == nblock - 1) bytes_lidos = fread (bufferBloco,sizeof(unsigned char), size_of_last_block,fileI); // leitura de um novo bloco do ficheiro original
            else bytes_lidos = fread (bufferBloco,sizeof(unsigned char), tamBlock,fileI);          
            tamRLE = toRLE (bufferBloco, bytes_lidos, auxRLE);                                     // compreessão RLE desse bloco
            fwrite(auxRLE,1,tamRLE,fileR);                                                         // escrita desse bloco no novo ficheiro
            funcaoFREQ(fileF, tamRLE, auxRLE);                                                     // calculo da frequência de cada bytes e escrita num ficheiro
            arrayTamRLE[i] = tamRLE;                                                               // tamanho do é anotado no array
        }
        fprintf(fileF,"@%d",0);                                                                   // 0 no fim do ficheiro freq
        fclose(fileF);
        fclose(fileR);
        *obrigatorioRLE = 1;           
    }
    else{                                                                                         // cenário em que a compressão RLE não é efetuada
        inicializaFile(fileInicial, 2, fileFREQ, nblock);                                         // criação do file .freq
        FILE *fileF = fopen (fileFREQ, "a");
        funcaoFREQ(fileF, bytes_lidos, bufferBloco);                                              // função auxiliar que calcula e grava as frequências
        for ( int j = 1; j < nblock; j++ ){                                                       // repetição do processo para os restantes blocos:
            if ( size_of_last_block > tamBlock && j == nblock - 1) bytes_lidos = fread (bufferBloco,sizeof(unsigned char), size_of_last_block,fileI);  //leitura de um novo bloco do ficheiro original
            else bytes_lidos = fread (bufferBloco,sizeof(unsigned char), tamBlock,fileI);
            funcaoFREQ(fileF, bytes_lidos, bufferBloco);                                          // calculo da frequência de cada bytes e escrita num ficheiro
        }
        fprintf(fileF,"@%d",0);                                                                   // 0 no fim do ficheiro freq
        
        fclose(fileF);        
    }
    free(auxRLE);
    free(bufferBloco);
    fclose(fileI);
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;                                   
    printfTerminal(arrayTamRLE, nblock, tamBlock,size_of_last_block, fileFREQ, fileRLE, *obrigatorioRLE,time_spent, sizefile );       // bloco de texto para o terminal

    return 0;
}
