#include <stdio.h>
#include "moduloF.c"
#include "moduloT.c"
#include "modulo_c.c"
#include "moduloDmain.c"
#include "time.h"
//#include "fsize.h"
#include "string.h"
#include "Manual.h"

int main (int argc, char *argv[]){
     if (argc < 2){
        printf("ERRO: Informação em falta. \nEm caso de dúvida use o comando manual.\n");
        return 0;
    }
    if (strcmp (argv[1], "manual") == 0 ){
        manual(1);    
        printf("Invoque novamente o programa.\n");
    }
    else{
        int obrigatorio = 0;
        unsigned long tamBlock = 65536;
        char nomeficheiro[100], nomefreq[100], nomeshaf[100];
        int descodificar_rle = 0, descodificar_sf = 0;
        int modulo = 0; // 0 -> todos os módulos, 1 -> modulo f, 2 -> modulo t, 3 -> modulo c, 4 -> modulo d
        for (int i = 1; i < argc; i++){
            if (strcmp (argv[i], "-b") == 0 && i <= argc-2){
                if (strcmp (argv[i+1], "K") == 0) tamBlock =655360;
                else if (strcmp (argv[i+1], "m") == 0) tamBlock = 8388608;
                else if (strcmp (argv[i+1], "M") == 0) tamBlock = 67108864;
                i++;
            }
            else if (strcmp (argv[i], "-c") == 0 && i <= argc-2){ 
                if (strcmp (argv[i+1], "r") == 0 ) obrigatorio = 1;
                i++;
            }
            else if (strcmp (argv[i], "-d") == 0 && i <= argc-2){
                if (strcmp (argv[i+1], "r") == 0) descodificar_rle = 1;
                else if (strcmp (argv[i+1], "s") == 0)descodificar_sf = 1;
                i++;
            }
            else if (strcmp (argv[i], "-m") == 0 && i <= argc-2){
                if (strcmp (argv[i+1], "f") == 0) modulo = 1;
                else if (strcmp (argv[i+1], "t")== 0) modulo = 2; 
                else if (strcmp (argv[i+1], "c")== 0) modulo = 3; 
                else if (strcmp (argv[i+1], "d")== 0)modulo = 4;
                i++;
            } 
            else{
                strcpy (nomeficheiro,argv[i]);
            }
        }
        FILE *fp = fopen (nomeficheiro, "r");
        if ( fp == NULL ){
            printf("Ficheiro inexistente.\nEm caso de dúvida utilizar código manual\n");
            return 0;
        }
        fclose (fp);
        //time_t now = time ( NULL );
        //struct tm *date = localtime ( &now );

        if (modulo == 1){
            moduloF(nomeficheiro, tamBlock, &obrigatorio);
        }
      
        else if (modulo == 2){
            //printf("\n\nCláudia Silva, a93177, Laura Rodrigues, a93169, MIEI/CD, %d-%d-%d\n",date->tm_mday,date->tm_mon + 1,date->tm_year + 1900);
            Tmain(nomeficheiro);
        }
        else if (modulo == 3) moduloC(nomeficheiro);
        else if (modulo == 4) Dmain1(argc,nomeficheiro, descodificar_rle, descodificar_sf);
        else {
            long size_of_last_block;
            long long nblock = fsize (fp, NULL, &tamBlock ,&size_of_last_block);
            unsigned long long sizefile = (nblock-1) * tamBlock + size_of_last_block;
            if (sizefile <1024){
            printf("Tamanho do ficheiro insuficiente!\n");
            return 0;
            }
            moduloF(nomeficheiro, tamBlock, &obrigatorio); 
            printf("\n");
            if (obrigatorio == 1){
                strcat(nomeficheiro, ".rle");
            }   
            //printf("\n\nCláudia Silva, a93177, Laura Rodrigues, a93169, MIEI/CD, %d-%d-%d\n",date->tm_mday,date->tm_mon + 1,date->tm_year + 1900);      
            strcpy(nomefreq, nomeficheiro);
            strcat(nomefreq, ".freq") ;
            Tmain(nomefreq);            
            moduloC(nomeficheiro);
            strcpy(nomeshaf, nomeficheiro);
            printf("\n");
            printf("\n");
            Dmain1(argc, strcat(nomeshaf, ".shaf"), descodificar_rle, descodificar_sf);

        }
    } 
    return 0;
}

