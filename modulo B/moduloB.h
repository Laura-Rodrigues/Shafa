#ifndef SHAFA_MODULOB_H
#define SHAFA_MODULOB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//int numBlocos (FILE *f);
//int tamBlocos (FILE *f, int bloco);
void freqToArray (int freqs[], FILE *f);
void swap(int v[], int x, int y);
void decrescenteSortSimb (int v[], int s[], int N);
void swapChar(unsigned char *v[], int x, int y);
void crescenteSortSimb (unsigned char *v[], int s[], int N);
int soma(const int freq[], int i, int j);
int melhorDiv(int freqs[],int i,int j);
void add_bit_to_code(char *bit, unsigned char *codes[], int start, int end);
void shannon (int freqs[],  unsigned char *codes[], int start, int end);

#endif //SHAFA_MODULOB_H
