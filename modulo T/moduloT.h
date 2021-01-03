#ifndef SHAFA_MODULOT_H
#define SHAFA_MODULOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freqToArray (int freqs[], FILE *f);
void swapInt(int v[], int x, int y);
void decrescenteSortSimb (int v[], int s[], int N);
void swapChar(unsigned char *v[], int x, int y);
void crescenteSortSimb (unsigned char *v[], int s[], int N);
int soma(const int freq[], int i, int j);
int melhorDiv(int freqs[],int i,int j);
void add_bit_to_code(char *bit, unsigned char *codes[], int start, int end);
void shannon (int freqs[], unsigned char *codes[], int start, int end);
void modulo_T (char *ficheiroFreq, char *ficheiroCod);
int Tmain(char *freq_name);

#endif //SHAFA_MODULOT_H
