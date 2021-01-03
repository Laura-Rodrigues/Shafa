#ifndef MODULODMAIN_H_ 
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define MODULODMAIN_H_


typedef struct node {
    int altura;
    unsigned char codificacao;
	char carater;
    struct node *prox;
} NODO;

void swap(int *xp, int *yp);
void bubbleSort(unsigned int arr_cod[],int arr_altura[],int arr_carater[], int n);

int get_num_blocos(FILE *f);
int get_tam_bloco (FILE *f);

void escreve_ficheiro(char simbolos[],char *filename,int *tamanho);

char  *descompressao_shafa(unsigned char arr[],int tam_bloco,NODO *codes,int *tamanho);
char  *descompressao_shafa_rle(unsigned char arr[],int tam_bloco,NODO *codes,int *tamanho);
char *get_info_block(FILE *f,int tam_bloco);
int get_info_bloco(char *array_simbolos,char filename[],int *tamanho);
unsigned int arr_to_unsigned(char *codi,int N);

NODO *cria_NODOS(unsigned int arr_codigos[],int arr_carater[],int arr_alturas[],int tam);
NODO* make_organizado_NODO(FILE *f,int tam_bloco);

void Dmain2(char nome_shaf[],char nome_cod[],int descodificar_rle,int descodificar_sf,char filename[]);
int Dmain1(int argc, char *argv[]);

#endif
