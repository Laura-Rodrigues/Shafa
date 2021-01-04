#include <stdio.h>
#include "string.h"

int manual(int a){
    int opcao = -1;
    char modulo[2] = "n";
    while (opcao != 0){
        printf ("Introduza o número correspondente a um dos módulos:\n  1. Erro: ficheiro inexistente. \n  2. O programa não faz o que é pedido.\n  3. O programa não funciona quando se tenta executar todos os módulos.\n 0. Para sair do manual e returnar ao programa shafa.\n\n");
        scanf("%d", &opcao);
        if (opcao ==1) printf ("Para evitar erros de _inexistência de ficheiros_ tenha em atenção os seguintes critérios:\n-Por favor, verifique se o documento existe na diretoria em que se encontra o programa Shafa.\n-Por favor, tenha em consideração que o nome do ficheiro não deverá um nome com mais de 90 caracteres.");
        else if (opcao == 2) printf("-Atenção: Uma vez mais este problema pode estar relacionado com a ordem os inputs do programa. \n-É também importante que dentro da diretória do programa estejam os documentos necessários para cada módulo.\n-Se forem invocados módulos especificos é importante que os inputs tenham a seguinte estrutura: -m f, por exemplo, isto é, os espaços são importantes. ");
        else if (opcao == 3) printf("A main atual permite executar todos os módulos separadamente. Infelizmente, apesar da tentativa ambiciosa de executar todos os módulos consecutiva, esta pode ter erros na execução de todos os módulos. Assim, pedimos que execute cada módulo separadamente e verifique com a atenção aqueles em que se encontram os erros\n");
        else if (opcao != 0) printf("Opcao inesxistente. Tente novamente \n");

        printf ("\n");
   
    }
    return 0;
   
}
