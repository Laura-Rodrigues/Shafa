#ifndef MODULOF_H
#define MODULOF_H

float compressao (unsigned long antes, unsigned long depois);

float fazerRLE ( unsigned long bytes_lidos, unsigned long tamArrRLE);

unsigned long toRLE(unsigned char bloco[],unsigned long tamBlock,unsigned char *arr);

void gravarRLE (char *nomeficheiro, unsigned char *bloco, unsigned long tamRLE, int rep);

void gravarFrequencias (char *nomeficheiro, unsigned long tamBloco, unsigned long *freq);

void acrescentaNumFile (char *nomeficheiro, int n);

void calculaFreqs (unsigned char *bufferBloco, unsigned long *freq, unsigned long tamBloco);

unsigned long fileToBloco (char *fileInicial, unsigned long tamBloco, unsigned long posicao, unsigned char *bufferBloco);

void inicializaFile (char *nomefile, int modulo, char *nomeFinal , long long nblock);

unsigned long long  tamfileAux (char *file, unsigned long *tamBlock);

int moduloF (char *fileInicial, unsigned long tamBlock, int obrigatorioRLE);

#endif //MODULOF_H