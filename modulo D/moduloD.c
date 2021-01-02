#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "moduloDmain.h"

int main(int argc, char *argv[])
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
 
if ( !strcmp(argv[2],"-m") && !strcmp(argv[3],"d") )
{
    if (argc == 6)
    {
    if ( !strcmp(argv[4],"-d") )
    {
        if ( !strcmp(argv[5],"-r") )
        descodificar_rle = 1;
    }
    }

     Dmain(shafa,cod,descodificar_rle,filename);
    
}
else printf("Erro no input\n"); 
 return 0;

}
