#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
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
  
     // se nao houve trocas entao quebra o ciclo
     if (!swapped) 
        break; 
   } 
} 





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

    char c;
    
    //Descobre o numero de blocos do ficheiro
	fscanf(f, "%d@", &tam_blocos);
	

    return tam_blocos;

}






void escreve_ficheiro(char simbolos[],char *filename,int *tamanho)
{
    //Abre um ficheiro novo e é carregado com os carateres descodificados
    int i=0;
   //Cria um novo ficheiro com nome "filename"
    FILE *f = fopen(filename,"a");

    fseek(f,0,SEEK_END);
    
    while (i<*tamanho)
    {
        fputc(simbolos[i],f);
        i++;
    }
   
    
    

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
    
    char *simbolos = malloc(tam_bloco*8*sizeof(char));//Simbolos descodificados
    
    

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
            

            while(flag_procu_simbolo   && codes->altura <= altura_ciclo)
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
                else{ codes = codes->prox;}
            
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
    
    return simbolos;
}




char  *descompressao_shafa_rle(unsigned char arr[],int tam_bloco,NODO *codes,int *tamanho)
{
    
    
    NODO *inicio = codes;
    int i=0,altura_ciclo=1,  flag_procu_simbolo=1;
    //Percursor do bit atual que estamos a percorrer
    unsigned char percursor1 = 0x80; 
    unsigned char simbolo= 0x00;// Simbolo a ser procurado nos Nodos

    
    char *simbolos= malloc(tam_bloco*8*sizeof(char) );//Simbolos descodificados

    int perc_simbo=0;
    int fase = 0;
    char char_rle;
    
    

        while (i<tam_bloco)
        {
			//quando o percursor de um byte chegar ao fim, é resetado para o inicio
            if (percursor1 == 0x00) //0000 0000
			{
				i++;
				percursor1 = 0x80;// 1000 0000
			}
           
            //Fazemos o simbolo a ser procurado aumentar um digito binario
            simbolo = simbolo<<1; //0000 0000
            

            
            if (arr[i] & percursor1) simbolo += 0x01;

			
            percursor1 = percursor1>>1;// 0100 0000;
           

            while(flag_procu_simbolo  && codes->altura <= altura_ciclo)
            {
                
                if (simbolo == codes->codificacao) 
                {
                    
					if ( codes->carater == 0)
                    {
                        if (fase == 0)
                        { 
                            fase = 1;
                        }
                    }
                    if (fase == 0)
                    {
                    // Carrega para o array simbolos o correspondente carater;
                    simbolos[perc_simbo] = codes->carater;//{0} carater numero
                    perc_simbo++;
                   
                    }
                    else if (fase == 1)
                    {
                    fase = 2;
                    }
                    else if ( fase == 2)
                    {
                    fase = 3;
                    char_rle = codes->carater; 
                    
                    }
                    else if ( fase == 3)
                    {
                        int k;
                        for ( k = 0;k < codes->carater; k++)
                        {
                            simbolos[perc_simbo] = char_rle;
                            perc_simbo++;
                        } 
                        fase = 0;

                    }
                    
                    
                    // Reseta os valores para voltar a percorrer o byte
                    simbolo    = 0x00;
                    flag_procu_simbolo =0;
                    altura_ciclo = 1;
                    (*tamanho)++; 
                    
                }
                else {
                    
            codes = codes->prox;
            }
                
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
    return simbolos;
}



/*

Dado um ficheiro e o seu tamanho do bloco,
usando os nodos organizados grava o 
resultado em ficheiro "teste" já descomprimido




*/

char  *get_info_block(FILE *f,int tam_bloco)
{
    unsigned char *arr = malloc(tam_bloco *sizeof(unsigned char));
    
    unsigned char *inicio = arr;

    
    // Le do ficheiro e guarda no array arr
    fread(arr, sizeof(unsigned char),tam_bloco,f);
    return inicio;
    
}

int get_info_bloco(char *array_simbolos,char filename[],int *tamanho)
{
          

  
escreve_ficheiro(array_simbolos,filename,tamanho);

free(array_simbolos);
    
return *tamanho;

}





/*
Descobre a codificacao de um array de carateres
*/
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




NODO* make_organizado_NODO(FILE *f,int tam_bloco)
{
    int contador=0,i=0;
    char c;
    
    int posicao=0;
    int altura_cod=1;
    unsigned int *array_cod = malloc(256 *sizeof(int));
    int *arr_caracter =malloc(256 *sizeof(int));
    int *arr_altura = malloc(256 *sizeof(int));
    char arr_byte_bin[30];
    
    int percursor=0;
    
    while((c= fgetc(f)) != '@') 
    {
        if (c == ';') contador++;
        else
        {
            
            arr_byte_bin[i] = c;
            i++;
            while((c = fgetc(f)) != ';')
            {
                if(c == '@')
                 {
                    fseek(f,-1,SEEK_CUR);
                    break;
                    }
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
            //resetar valores
            altura_cod = 1;
            i=0;

        }
        
        
    }
    
    NODO *nodo =cria_NODOS(array_cod,arr_caracter,arr_altura,percursor);
    free(arr_altura);
    free(array_cod);
    free(arr_caracter);
    
    
    
    return nodo;
    
}



/*
____________________________________\___________________________\______________________________________

____________________________________/___________________________/______________________________________

*/

void Dmain2(char nome_shaf[],char nome_cod[],int descodificar_rle,int descodificar_sf,char filename[])
{
    int flag_rle=0,tam_bloco_cod,num_blocos,num_bloco_cod;
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
    if(c == 'R') flag_rle = 1;
    if ( flag_rle && !descodificar_rle) strcat(filename,".rle");
    else if ( descodificar_rle) strcat(filename,".rle");
    


    
    
    
    //Descobre o numero de blocos do ficheiro
    num_blocos    = get_num_blocos(fileshaf);
    num_bloco_cod = get_num_blocos(filecod);



    int tam_bloco;
    int tam_antes[num_blocos],tam_depois[num_blocos];
    FILE *file_final=fopen(filename,"w");

    //Ciclo principal para descodificao dos blocos 
    
    for(int i=0;i<num_blocos;i++)
    {
        tam_bloco_cod = get_tam_bloco(filecod);
        
        NODO *nodo = make_organizado_NODO(filecod,tam_bloco_cod);
       
        
        tam_antes[i] = get_tam_bloco(fileshaf);
        
        //Guarda o tamanho do bloco em um array
        
        
        char *arr;
        arr = get_info_block(fileshaf,tam_antes[i]);

        int tamanho=0;
        char *array_simbolos;

        
        
        if (descodificar_sf)
        array_simbolos = descompressao_shafa(arr,tam_antes[i],nodo,&tamanho);
        
        else if (descodificar_rle)
        array_simbolos = descompressao_shafa_rle(arr,tam_antes[i],nodo,&tamanho);
        
        else if ( flag_rle && !descodificar_rle)
        array_simbolos = descompressao_shafa_rle(arr,tam_antes[i],nodo,&tamanho);
        else
        array_simbolos = descompressao_shafa(arr,tam_antes[i],nodo,&tamanho);
    
    
    
        
        
        //Depois de descodificar o ficheiro shafa ele devolve o novo tamanho descomprimido
        tam_depois[i] = get_info_bloco(array_simbolos,filename,&tamanho);
        free(arr);
        free(nodo);
        c = fgetc(fileshaf);// Lê o proximo arroba '@'
        
        
    }
    

 end = clock();
 fclose(file_final);
 fclose(fileshaf);
 fclose(filecod);

 double cpu_time_used = ((double) (end - start)/CLOCKS_PER_SEC * 1000);
 
 


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
 printf("Ficheiro gerado : %s\n",filename);
 
 return;
}


int Dmain1(int argc, char *argv[])
{
 int i;
 char c;
 int flag_rle=0;
 
 
 char *shafa = argv[1];
 if (!shafa)
 {
     printf("Erro no ficheiro\n");
     return 1;
 }
 
 if (i = strlen(shafa) <= 9)
 {
    printf("Nome invalido\n");
    return 1;
 }
 if (argc < 4)
 {
     printf("Poucos argumentos \n");
     return 1;
 }
 
 i = strlen(shafa)  - 5; // é removido o ".shaf"
 char flag_rle_c[4] = {shafa[i-4],shafa[i-3],shafa[i-2],shafa[i-1]};
 
 if (  !strcmp( flag_rle_c,".rle") )
 {
     flag_rle = 1;
     i = i-4;


 }
 int k;
 char cod[i+3];
 for ( k = 0; k < i; k++)
 {
     cod[k] = shafa[k];
 }

 cod[k] = '\0';
char filename[k];  
strcpy(filename,cod);

 if (flag_rle)
 {
     strcat(cod,".rle.cod");
 }
 else strcat(cod,".cod");
 

 
int descodificar_rle=0;
int descodificar_sf=0;
 
if ( !strcmp(argv[2],"-m") && !strcmp(argv[3],"d") )
{
    if (argc == 6)
    {
    if ( !strcmp(argv[4],"-d") )
    {
        if ( !strcmp(argv[5],"-r") )
        descodificar_rle = 1;
        else if(!strcmp(argv[5],"-s"))
        descodificar_sf = 1;
    }
    }
     Dmain2(shafa,cod,descodificar_rle,descodificar_sf,filename);
    
}
 else printf("Erro no input\n"); 
 return 0;

}






