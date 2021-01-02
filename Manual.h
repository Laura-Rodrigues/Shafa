#include <stdio.h>
#include "string.h"

int manual(int a){
    int opcao = -1;
    char modulo[2] = "n";
    while (opcao != 0){
        printf ("Introduza o número correspondente a um dos módulos:\n  1. Em que consiste o programa.\n  2. Erro: ficheiro inexistente. \n  3. O programa não faz o que é pedido.\n  4. Ordem dos argumentos. (Pode estar na origem de muitos erros.)\n  0. Para sair do manual e returnar ao programa shafa.\n\n");
        scanf("%d", &opcao);
        if (opcao == 1) {
            printf("A codificação de Shannon-Fano é um método estatístico de compressão sem perda de dados que gera códigos de tamanho variável para cada símbolo dos conjunto de dados a ser comprimido de acordo com sua probabilidade de ocorrência.\n");
            printf("Os passos/módulos envolvidos nesse programa são:\n-Módulo F: módulo responsável pelo cálculo das frequências de cada simbolo e pela compressão RLE.\n-Módulo T: \n-Módulo C:\n-Módulo D: \n");
            //printf ("Se pretender saber mais sobre cada módulo, indique escrevendo a letra correspondente a cada módulo, senão escreva a letra n\n");
        }
        else if (opcao == 2) printf ("Para evitar erros de _inexistência de ficheiros_ tenha em atenção os seguintes critérios:\n-Por favor, verifique se o documento existe na diretoria em que se encontra o programa Shafa.\n-A ordem dos argumentos é importante para este programa.\n");
        else if (opcao == 3) printf("-Atenção: Uma vez mais este problema pode estar relacionado com a ordem os inputs do programa. \n-É também importante que dentro da diretória do programa estejam os documentos necessários para cada módulo.\n-Se forem invocados módulos especificos é importante que os inputs tenham a seguinte estrutura: -m f, por exemplo, isto é, os espaços são importantes. ");
        else if (opcao == 4);
        else if (opcao != 0) printf("Opcao inesxistente. Tente novamente \n");

        printf ("\n");
   // printf("Para a execução eficiente do programa, aquando da invocação da função garantir que apresenta os argumentos necessários\nOrdem dos argumentos deve ser: nome do ficheiro; -m e o modulo, e qualquer outras informações adicionais\n");
    //printf ("\nPor favor, tenha em consideração que o nome do ficheiro não deverá um nome com mais de 90 caracteres.");
    
        
    }
    return 0;
   
}
