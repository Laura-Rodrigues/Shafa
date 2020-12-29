#include <stdio.h>
#include "ModuloF.h"
#include "time.h"
#include "string.h"
#include "Manual.h"

int main (int argc, char *argv[]){
    int obrigatorio = 0;
    unsigned long tamBlock = 65536;
    char nomeficheiro[100];
    if (argc < 2){
        printf("ERRO: Informação em falta. \nEm caso de dúvida use o comando manual.\n");
        return 0;
    }
    if (strcmp (argv[1], "manual") == 0 )
        manual(1);    
    else if (argc == 2){
        FILE *fp = fopen (argv[1], "r");
        if ( fp == NULL ){
            printf("Ficheiro inexistente.\nEm caso de dúvida utilizar código manual\n");
            return 0;
        }
        fclose (fp);
        strcpy(nomeficheiro, argv[1]);
        //strcpy(nomeficheiroshaf, argv[1]);
        moduloF (argv[1], tamBlock, obrigatorio);
        strcat(nomeficheiro,".freq"); //ou .rle
        /*moduloT(nomeficheiro); //arranjar metodo de alterar os nomes dos files //Neste momento o código s'está a funcionar para o nome do file dado
        strcat(nomeficheiro,".cod");
        moduloC(nomeficheiro);
        strcat(nomeficheiroshaf,".shaf");
        moduloD(no); //tem que receber dos ficheiros diferentes....
        atenção aos nomes dos ficheiros*/
    }
    else{
        for (int i = 0; i < argc; i++){
            if (strcmp (argv[i], "-b") == 0 && i <= argc-2){
                if (strcmp (argv[i+1], "K") == 0) tamBlock = 655360; //655360;
                else if (strcmp (argv[i+1], "m") == 0) tamBlock = 8388608; //8388608;
                else if (strcmp (argv[i+1], "M") == 0) tamBlock = 67108864; //67108864;
                i++;
            }
            if (strcmp (argv[i], "-c") == 0 && i <= argc-2){
                if (strcmp (argv[i+1], "r") == 0 ) obrigatorio = 1;
                i++;
            }
            if (strcmp (argv[i], "-d") == 0 && i <= argc-2){
                if (strcmp (argv[i+1], "r") == 0);
                else if (strcmp (argv[i+1], ""))
                i++;
            }
            if (strcmp (argv[i], "-m") == 0 && i <= argc-2){
                if (strcmp (argv[i+1], "f") == 0) moduloF(argv[1], tamBlock, obrigatorio);
                else if (strcmp (argv[i+1], "t")== 0); //moduloT(argv[1]);
                else if (strcmp (argv[i+1], "c")== 0); //moduloC(argv[1]);
                else if (strcmp (argv[i+1], "d")== 0); //moduloD(argv[1], argv[2]);
                i++;
            } 
        }

    // verificar se não é .rle, .freq?    

/*
    //verificar se o ficheiro existe
    FILE *fp = fopen (fileInicial, "rb");
    if ( fp == NULL ){
        printf ("Ficheiro inválido.\n");
        return 0;
    } 
    fclose (fp);
    moduloF(fileInicial, tamBlock, obrigatorioRLE);
*/
    }
    return 0;
}