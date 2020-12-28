#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "moduloDmain.h"



void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 



void bubbleSort(unsigned int arr_cod[],int arr_altura[],int arr_carater[], int n) 
{ 
   int i, j; 
   int swapped; 
   for (i = 0; i < n-1; i++) 
   { 
     swapped = 0; 
     for (j = 0; j < n-i-1; j++) 
     { 
        if (arr_altura[j] > arr_altura[j+1]) 
        { 
           swap(&arr_altura[j], &arr_altura[j+1]);
           swap(&arr_cod[j], &arr_cod[j+1]);
           swap(&arr_carater[j], &arr_carater[j+1]);

        
           swapped = 1; 
        } 
     } 
  
     // IF no two elements were swapped by inner loop, then break 
     if (!swapped) 
        break; 
   } 
} 



#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 





int get_num_blocos(FILE *f)
{
    int num_blocos=-1;
    char c;

    //Carrega para num_blocos o tamanho em bytes do blocos
	fscanf(f, "@%d@",&num_blocos);
    

	return num_blocos;

}



int get_tam_bloco (FILE *f)
{
    int tam_blocos=0,i=0;

    char c, *arr=malloc(sizeof(char));
    
    //Descobre o numero de blocos do ficheiro
	fscanf(f, "%d@", &tam_blocos);
	

    return tam_blocos;

}






void escreve_ficheiro(char simbolos[],char *filename,int *posicao,int *tamanho)
{
    //Abre um ficheiro novo e é carregado com os carateres descodificados
    int i=0;
   //Cria um novo ficheiro com nome "filename"
    FILE *f = fopen(filename,"w");

    fseek(f,*posicao,SEEK_SET);
    while (i<*tamanho)
    {
        fputc(simbolos[i],f);
        i++;
    }
    *posicao = ftell(f);
    fclose(f);
    

}

/*****************************************************************
 * 
 * 
 * 
 * 
 * 
 * 
 * DESCOMPRESSÃO
 * 
 * 
 * 
 * 
 * 
 * *********************************************************************/





char  *descompressao_shafa(unsigned char arr[],int tam_bloco,NODO *codes,int *tamanho)
{
    
    
    NODO *inicio = codes;
    int i=0,altura_ciclo=1,  flag_procu_simbolo=1;
    //Percursor do bit atual que estamos a percorrer
    unsigned char percursor1 = 0x80; 
    unsigned char simbolo= 0x00;// Simbolo a ser procurado nos Nodos
    
    char simbolos[tam_bloco*8];//Simbolos descodificados
    char *ptr = simbolos;//Apontador para o inicio do array;
    *tamanho = 0;

    int perc_simbo=0;
    
    

        while (i<tam_bloco)
        {
			//quando o percursor de um byte chegar ao fim, é resetado para o inicio
            if (percursor1 == 0x00)
			{
				i++;
				percursor1 = 0x80;
			}
           
            //Fazemos o simbolo a ser procurado aumentar um digito binario
            simbolo = simbolo<<1; //0000 0000
            
            if (arr[i] & percursor1) simbolo += 0x01;
			
            percursor1 = percursor1>>1;// 0100 0000;
            

            while(flag_procu_simbolo && codes->altura <= altura_ciclo)
            {
                if (simbolo == codes->codificacao) 
                {
					
                    // Carrega para o array simbolos o correspondente carater;
                    simbolos[perc_simbo] = codes->carater;
                    perc_simbo++;
                   
                   
                    // Reseta os valores para voltar a percorrer o byte
                    simbolo    = 0x00;
                    flag_procu_simbolo =0;
                    altura_ciclo = 1;
                    (*tamanho)++;
                }
                else codes = codes->prox;
                
            }
            //percorrer mais nodos com altura superior
            if (flag_procu_simbolo) altura_ciclo++; 
            else
             {
                //acabou a procura do simbolo,reinicia as variaveis
                codes = inicio; 
                flag_procu_simbolo = 1;
             }
			
        
        }
    
    
    return ptr;
}



char  *descompressao_shafa_rle(unsigned char arr[],int tam_bloco,NODO *codes,int *tamanho)
{
    
    
    NODO *inicio = codes;
    int i=0,altura_ciclo=1,  flag_procu_simbolo=1;
    //Percursor do bit atual que estamos a percorrer
    unsigned char percursor1 = 0x80; 
    unsigned char simbolo= 0x00;// Simbolo a ser procurado nos Nodos
    
    char simbolos[tam_bloco*8];//Simbolos descodificados
    char *ptr = simbolos;//Apontador para o inicio do array;

    int perc_simbo=0;
    
    int sequencia = 1;
    char char_anterior;
    char char_atual;
    *tamanho = 0;
    
    

        while (i<tam_bloco)
        {
			//quando o percursor de um byte chegar ao fim, é resetado para o inicio
            if (percursor1 == 0x00)
			{
				i++;
				percursor1 = 0x80;
			}
           
            //Fazemos o simbolo a ser procurado aumentar um digito binario
            simbolo = simbolo<<1; //0000 0000
            
            if (arr[i] & percursor1) simbolo += 0x01;
			
            percursor1 = percursor1>>1;// 0100 0000;
            

            while(flag_procu_simbolo && codes->altura <= altura_ciclo)
            {
                //foi encontrada a codificação
                if (simbolo == codes->codificacao) 
                {
                    char_atual = codes->carater;
					
                    
                    //printf("sequencia = %d\n",sequencia);
                    //quando o carater anterior for igual ao atual
                    if ( char_anterior == char_atual)
                    {
                        if (sequencia<255) sequencia++;
                        else 
                        {
                        simbolos[perc_simbo] = 0;
                        simbolos[perc_simbo++] = char_atual;
                        simbolos[perc_simbo++] = sequencia;
                        perc_simbo++;
                        sequencia = 1;

                        }
                    }
                    else if ( sequencia > 3)
                    {
                        //codigo para inserir rle
                        simbolos[perc_simbo] = 0;
                        simbolos[perc_simbo++] = char_atual;
                        simbolos[perc_simbo++] = sequencia;
                        perc_simbo++;
                        //reseta o valor de sequencia
                        

                        sequencia = 1;

                    }
                    else
                    {
                        
                        /*
                        ciclo for para introduzir no array carater repetido no 
                        maximo 3 vezes
                        */
                       if (codes->carater == 0)
                       {
                           simbolos[perc_simbo] = 0;
                           simbolos[perc_simbo++] = 0;
                           simbolos[perc_simbo++] = sequencia;
                           perc_simbo++;
                           sequencia = 1;
                           
                       }
                        else 
                        {
                        for (;sequencia > 0; sequencia--)
                        {
                         simbolos[perc_simbo] = char_atual;
                         perc_simbo++;
                        }
                        //reseta o valor de sequencia
                        sequencia = 1;
                        }

                        
                    }
                    
                   
                    // Reseta os valores para voltar a percorrer o byte
                    simbolo    = 0x00;
                    flag_procu_simbolo =0;
                    altura_ciclo = 1;
                    char_anterior =char_atual;
                    (*tamanho)++;
                    
                }
                else codes = codes->prox;
                
            }
            //percorrer mais nodos com altura superior
            if (flag_procu_simbolo) altura_ciclo++; 
            else
             {
                //acabou a procura do simbolo,reinicia as variaveis
                codes = inicio; 
                flag_procu_simbolo = 1;
             }
			
        
        }
    
    
    return ptr;
}







/*

Dado um ficheiro e o seu tamanho do bloco,
usando os nodos organizados grava o 
resultado em ficheiro "teste" já descomprimido




*/
int get_info_bloco(FILE *f,int tam_bloco,NODO* nodo,int flag_rle)
{
    int i=0,codigo_binario;
       
    
    char c, arr[tam_bloco];



    
    fread(arr, sizeof(unsigned char),tam_bloco,f);
    
  
int tamanho;
char *array_simbolos = (flag_rle) ? descompressao_shafa_rle(arr,tam_bloco,nodo,&tamanho) 
                                  : descompressao_shafa(arr,tam_bloco,nodo,&tamanho);



char nome_file[6] = "teste";
char *ptr_nome = &nome_file[0];
int posicao=0;
 
escreve_ficheiro(array_simbolos,ptr_nome,&posicao,&tamanho);
        
    
return tamanho;

}





//Descobre a codificacao de um array de carateres e a sua altura
unsigned int arr_to_unsigned(char *codi,int N)
{
    unsigned int r=0;

    for (int i = 0; i < N; i++)
    {
        r = r<< 1;
        if(codi[i] == '1') r++;
    }
    
    return r;
}








NODO * cria_NODOS(unsigned int arr_codigos[],int arr_carater[],int arr_alturas[],int tam)
{
    NODO *novo = malloc(sizeof(NODO));
    NODO *inicio = novo;
    int i;
    
    
    
    
    //Ordenar por alturas as codificações
    bubbleSort(arr_codigos,arr_alturas,arr_carater,tam);
    
    // Ciclo para criar os nodos já ordenados
    for ( i = 0; i < tam; i++)
    {
        novo->altura = arr_alturas[i];
        novo->carater = arr_carater[i];
        novo->codificacao = arr_codigos[i];


        novo->prox = malloc(sizeof(NODO));
        novo = novo->prox;
    }
    
    return inicio;
    

}




NODO* putinha(FILE *f,int tam_bloco)
{
    int contador=0,i=0;
    char c;
    
    int posicao=0;
    int altura_cod=1;
    unsigned int *array_cod= malloc(256 *sizeof(int));
    int *arr_caracter =malloc(256 *sizeof(int));
    int *arr_altura = malloc(256 *sizeof(int));
    char arr_byte_bin[16];
    
    int percursor=0;
    
    
    while(contador < 255)
    {
        if ((c = fgetc(f)) == ';') contador++;
        else
        {
            unsigned int fake;
            
            arr_byte_bin[i] = c;
            i++;
            
            while((c = fgetc(f)) != ';')
            {
                arr_byte_bin[i] = c;
                i++;
                altura_cod++;

            }
            

                    

            
            
            unsigned int novo = arr_to_unsigned(arr_byte_bin,i);
            arr_altura[percursor] = altura_cod;
            
            array_cod[percursor]   = novo; //novo unsigned int
            
            arr_caracter[percursor] = contador;
            
            percursor++;
            
            contador++;
            altura_cod = 1;
            i=0;
            
            


        }
        
        
    }

    NODO *nodo = cria_NODOS(array_cod,arr_caracter,arr_altura,percursor);
    free(arr_altura);
    free(array_cod);
    free(arr_caracter);
    
    
    
    return nodo;
    
}



/*
____________________________________\___________________________\______________________________________

____________________________________/___________________________/______________________________________

*/

void Dmain(char nome_shaf[],char nome_cod[])
{
    int flag_rle,tam_bloco_cod,num_blocos,num_bloco_cod;
    char c;
    clock_t start, end;
    
    start = clock();
    //Lê o ficheiro shaf
    FILE *fileshaf = fopen(nome_shaf,"r");
    


    FILE *filecod = fopen(nome_cod,"r");
    if (!filecod || !fileshaf)
    {
        printf("Ficheiro Vazio\n");
        return;
    }
    
    
	//Descobre se foi usada compressao RLE
    fscanf(filecod, "@%c", &c);
    if ( c == 'R') flag_rle = 1;
	else flag_rle = 0;
    
    //Descobre o numero de blocos do ficheiro
    num_blocos    = get_num_blocos(fileshaf);
    num_bloco_cod = get_num_blocos(filecod);



    int tam_bloco;
    int tam_antes[num_blocos],tam_depois[num_blocos];

    //Ciclo principal para descodificao dos blocos 
    for(int i=0;i<num_blocos;i++)
    {
        tam_bloco_cod = get_tam_bloco(filecod);
        
        NODO *nodinho = putinha(filecod,tam_bloco_cod);
        c = fgetc(filecod);//carrega outro arroba;
        /*
        *
        * 
        * 
        * 
        * 
        */
        tam_antes[i] = get_tam_bloco(fileshaf);
        
        //Guarda o tamanho do bloco em um array
        
        
        //Depois de descodificar o ficheiro shafa ele devolve o novo tamanho descomprimido
        tam_depois[i] = get_info_bloco(fileshaf,tam_antes[i],nodinho,flag_rle);
        
        free(nodinho);
        c = fgetc(fileshaf);// Lê o proximo arroba '@'
        
    }

 end = clock();
 fclose(fileshaf);

 double cpu_time_used = ((double) (end - start)/1800);
 
 


 /*
 *
 * 
 * Resultado Final
 * 
 */
 printf("André Pinto,a93173,Gonçalo Peres,a93309,MIEI/CD, 1-jan-2021\n");
 printf("Módulo: d (descodificação dum ficheiro shaf)\n");
 printf("Número de blocos: %d\n",num_blocos);
 for ( int k = 0; k < num_blocos; k++)
 {
    printf("Tamanho antes/depois do ficheiro gerado (bloco %d): %d/%d\n",k+1,tam_antes[k],tam_depois[k]);
 }
 
 printf("TEMPO : %f ms\n",cpu_time_used);
 
 return;
}






