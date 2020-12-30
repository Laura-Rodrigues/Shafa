/* Fase 1:

2) Se com a compressão o ultimo bloco <1K????
NOTA: APRESENTAR INFORMAÇOES PARA CADA MODULO

NA MAIN DO TRABALHO:
-> se pedir para fazer -m f -m c, fazer modulo f e dizer que não dá para fazer o modulo c porque falta o .cod;

METODO 1: *****Passar ler um bloco e escrever logo a seguir (aka processo atual)*****
    1) Ler um bloco do ficheiro;
    2) Determinar se vale a pena fazer o RLE;
    2.1) Aplicar o metodo RLE (e gravar file .rle)
    3) Calcular as frequências dos simbolos 
    4) Escrever no novo file as frequências
    5) Repetir este processo para todos os blocos

Nota: usamos unsigned char porque cada char corresponde a 1 byte aka 8 bits
*/

#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "ModuloF.h"
#include "string.h"
#include "fsize.h"

/*
    Esta função tem como objetivo calcular a taxa de compressão. Recebe o numero de bytes do bloco original e o 
    numero de bytes do bloco pós compressão RLE
*/
float compressao (unsigned long antes, unsigned long depois){
    float c = (((antes*1.0) - (depois*1.0)) / (antes*1.0));
    return c;
}

/*
    Função que avalia se a compressão RLE compensa, retorna o valor 1 caso a taxa de compressão seja mais que 0.05 ou 5% (valor definido pelos docentes)
*/
float fazerRLE ( unsigned long bytes_lidos, unsigned long tamArrRLE){
    int result = 0;
    float aux = compressao(bytes_lidos, tamArrRLE);
    if ( aux > 0.05 ) result = 1;
    return result;
}

/*
    Função tem como propósito fazer a compressão RLE de um bloco de bytes de tamanho tamBlock e guardar o bloco covertido no array arr.
    A função analisa o bloco byte a byte, incrementando o numero de repetições a cada vez que o byte atual é igual ao seguinte.
    Quando o byte atual for diferente e igual a zero ou diferente e o numero de repetições (r) for maior que 3, o arr aumenta em 3 bytes. O primeiro byte
    guarda o byte 0, o segundo o byte que se está a repetir, e o terceiro guarda o nr de repetições.
    Na situação restante (quando o byte atual é diferente do seguinte mas o numero de repetições nao é superior a 3), o ciclo for aloca memória necessaria para
    colocar r repetições do byte atual
*/
unsigned long toRLE(unsigned char bloco[],unsigned long tamBlock,unsigned char *arr){ 
    int r = 1;
    unsigned long j = 0;
    for ( int i = 0; i<tamBlock ;i++){
        if (bloco[i] == bloco[i+1] && r != 255) r++;
        else if  (r == 255 || bloco[i] == 0 || r > 3)  { //nao esquecer da situaçao que o r é smp < 255
            arr = (unsigned char *)realloc(arr,j * sizeof (unsigned char) + 3* sizeof (unsigned char));
            arr[j]=0;
            arr[j+1] = bloco[i];
            arr[j+2] = r;
            r = 1;
            j+=3;
        }
        else{
            for (r; r>=1;r--){
                arr = (unsigned char *)realloc(arr, j * sizeof (unsigned char) + sizeof (unsigned char));
                arr[j] = bloco[i];
                j++;
            }
            r=1;
        }       
    }
    return j;
}

/*
    Função que recebe um bloco, o numero de bytes lidos e um int que sinaliza se é o primeiro bloco ou um dos blocos seguintes
    Se se tratar do 1º bloco (rep == 0) então a função fopen será chamada com "wb", caso não seja, a função fopen é chamada com "ab", 
    ambas as situações escrevem em binário
*/
void gravarRLE (char *nomeficheiro, unsigned char *bloco, unsigned long tamRLE, int rep){
    FILE *fp;
    if (rep == 0) fp = fopen (nomeficheiro, "wb");
    else fp = fopen (nomeficheiro, "ab");
    fwrite(bloco,1,tamRLE,fp);
    fclose(fp);
}

/*
    Função que recebe um array de frequencias de um bloco, o tamanho do mesmo bloco e, com um ciclo for, grava no ficheiro as frequencias desse bloco
    e o tamanho do bloco na notação apropriada
*/
void gravarFrequencias (char *nomeficheiro, unsigned long tamBloco, unsigned long *freq){
    FILE *fp = fopen(nomeficheiro, "a");
    if (fp != NULL){
        fprintf ( fp, "@%lu@",tamBloco);
        for (int i = 0; i < 256; i++) {
            if ( i == 255 && freq[i] == freq[i-1]);
            else if (i > 0 && freq[i] == freq[i-1]) fprintf(fp, ";");
            else if (i == 255) fprintf(fp, "%lu",freq[i]);
            else fprintf(fp, "%lu;", freq[i]);
        }
    }
    fclose(fp);
}

/*
    Função que acrescenta um apendice inteiro a um ficheiro
*/
void acrescentaNumFile (char *nomeficheiro, int n){
    FILE *fp = fopen(nomeficheiro, "a");
    if (fp != NULL)
        fprintf(fp, "@%d",n);
    fclose (fp);
}

/*
    Função que calcula a frequência de cada caracter
*/
void calculaFreqs (unsigned char *bufferBloco, unsigned long *freq, unsigned long tamBloco){
    int i;
    for (i = 0; i < tamBloco; i++){
        freq[bufferBloco[i]]++;
    }
}

/*
    Função que lê um ficheiro e passa os dados guardados nos bytes correspondentes ao tamanho de um bloco e passa para um buffer bloco desse mesmo tamanho
*/
unsigned long fileToBloco (char *fileInicial, unsigned long tamBloco, unsigned long posicao, unsigned char *bufferBloco){
    FILE *fp = fopen(fileInicial, "rb");
    unsigned long  bytes_lidos;

    if (fp == NULL ){
        printf("erro relacionado com o file\n");
        return 0;
    }
    fseek (fp, posicao, SEEK_SET);
    bytes_lidos = fread (bufferBloco,sizeof(unsigned char), tamBloco,fp); 

    fclose(fp);
    return bytes_lidos;
}

/*
    Função que define o sufixo do nome do ficheiro gerado e tambem o prefixo @R ou @N dentro do ficheiro, consoante a opção escolhida indicada pelo int
    modulo.
    modulo = 0 -> ficheiro .RLE
    modulo = 1 -> ficherio .RLE.FREQ
    modulo = 2 -> ficheiro .FREQ
*/
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
        fclose (fp);
    }    
    if ( modulo == 1 || modulo == 2 ) acrescentaNumFile(nomeFinal, nblock);
}

unsigned long long  tamfileAux (char *file, unsigned long *tamBlock){
    long size_of_last_block;
    FILE *fp = fopen (file, "rb");
    long long nblock = fsize (fp, NULL, tamBlock ,&size_of_last_block);
    fclose(fp);
    unsigned long long sizefile = (nblock-1) * (*tamBlock) + size_of_last_block;
    return sizefile;
}

//j há  vezes que é int outra que é unsigned long
//Se usarmos a função fsize do stor, tem uma cena fixe, porque, podemos usar o int do tamanho no ultimo bloco para a 
int moduloF (char *fileInicial, unsigned long tamBlock, int obrigatorioRLE){
    clock_t begin = clock();
    time_t now = time ( NULL );
    struct tm *date = localtime ( &now );

    char fileFREQ[100], fileRLE[100];
    long size_of_last_block;
    unsigned long bytes_lidos, freq[256], tamRLE;
    FILE *fp = fopen (fileInicial, "rb");
    long long nblock = fsize (fp, NULL, &tamBlock ,&size_of_last_block);
    fclose(fp);
    unsigned long arrayTamRLE[nblock];
    unsigned long long sizefile = (nblock-1) * tamBlock + size_of_last_block, sizefileRLE;
    unsigned char *auxRLE,*bufferBloco;  

    bufferBloco = (unsigned char*)malloc(tamBlock * sizeof(unsigned char));
    bytes_lidos = fileToBloco(fileInicial, tamBlock, 0, bufferBloco);
    auxRLE = (unsigned char *)malloc(sizeof(unsigned char));
    tamRLE = toRLE (bufferBloco, bytes_lidos, auxRLE);

    if (nblock > 1 && size_of_last_block < 1024 ){
        nblock --;
        size_of_last_block += tamBlock;
    }

    if (obrigatorioRLE == 1 || fazerRLE(bytes_lidos,tamRLE)){ 
        inicializaFile(fileInicial, 0, fileRLE, nblock);
        inicializaFile(fileInicial, 1, fileFREQ, nblock);
      
        for ( int i = 0; i < nblock; i++){
            if ( i == nblock - 1 ){
                bufferBloco = (unsigned char *)malloc(size_of_last_block * sizeof (unsigned char));
                bytes_lidos = fileToBloco(fileInicial, size_of_last_block, i*tamBlock, bufferBloco);
            }
            else{
                bufferBloco = (unsigned char *)malloc(tamBlock*sizeof (unsigned char));
                bytes_lidos = fileToBloco(fileInicial, tamBlock, i*tamBlock, bufferBloco);         
            }    
            auxRLE = (unsigned char *)malloc(sizeof(unsigned char));
            tamRLE = toRLE (bufferBloco, bytes_lidos, auxRLE);
            gravarRLE(fileRLE, auxRLE, tamRLE, i);
        
            for (int i = 0; i < 256; i++) freq[i] = 0;
            calculaFreqs(auxRLE,freq,tamRLE);
            gravarFrequencias(fileFREQ, tamRLE, freq );
            arrayTamRLE[i] = tamRLE;
        }

        acrescentaNumFile(fileFREQ, 0);
        obrigatorioRLE = 1;

        sizefileRLE = tamfileAux (fileRLE, &tamBlock);
    }
    else{
        inicializaFile(fileInicial, 2, fileFREQ, nblock);
        for ( int j = 0; j < nblock; j++ ){
            for (int i = 0; i < 256; i++) freq[i] = 0;
            if (j == nblock - 1){
                bufferBloco = malloc(size_of_last_block*sizeof (unsigned char));
                bytes_lidos = fileToBloco(fileInicial, size_of_last_block, j*tamBlock, bufferBloco);
            }
            else{
                bufferBloco = malloc(tamBlock * sizeof (unsigned char));
                bytes_lidos = fileToBloco(fileInicial, tamBlock, j*tamBlock, bufferBloco);
            }
            calculaFreqs(bufferBloco,freq,bytes_lidos);
            gravarFrequencias(fileFREQ, bytes_lidos, freq );
            free(bufferBloco);
        }
        acrescentaNumFile(fileFREQ, 0);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
 
    printf("Francisco José Martinho Toldy,a93226; Mariana Filipa Rodrigues,a93306 MIEI-CD,%d-%d-%d\n",date->tm_mday,date->tm_mon + 1,date->tm_year + 1900);
    printf("Módulo: f \nNúmero de blocos: %lld\n", nblock);//,numBlocos
    printf("Tamanho dos blocos analisados: %lld blocos de %lu bytes + 1 bloco de %lu bytes\n", nblock-1, tamBlock, size_of_last_block); 
    if (obrigatorioRLE) {
        printf("Compressão RLE: %s (%0.2f %% compressão)\n",fileRLE, compressao(sizefile,sizefileRLE)*100);//S ou N,tamanho e taxa de compressão
        printf("Tamanho dos blocos analisados no ficheiro RLE:");
        for ( int i = 0; i < nblock; i++){
            if (i == nblock-1) printf("%lu\n", arrayTamRLE[i]);
            else printf("%lu/",arrayTamRLE [i]);
        }
    }
    printf("Tempo de execução do módulo:%0.2f ms\n", time_spent*1000);//função que calcula o tempo de execução
    printf("Ficheiros gerados: %s \n", fileFREQ);// lista de ficheiros gerados


    return 0;
}

/*  printf("--%lu--\n", tamRLE);
    for ( int x = 0; x < tamRLE; x++) printf("%u,", auxRLE[x]);
    printf ("\n");
*/
