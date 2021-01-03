#ifndef MODULOF_H
#define MODULOF_H

/*
    Módulo F - parte do trabalho shafa grupo 16, UC de Comunicação de Dados MIEI 2020-2021
    Período de desenvolvimento de trabalho: 2 de dezembro de 2020 a 3 de janeiro de 2021
    Autores (por ordel alfabética):
    Francisco Toldy   a93226
    Mariana Rodrigues a93306;

    Este módulo inclui funções como "toRLE": compressão RLE; "gravarFrequências": grava as frequências no file .freq; "calculaFreqs": calcula a frequẽncia de cada byte; "inicializaFile": criação dos files que serão output do programa;
 "funcaoFREQ": função auxiliar para obter os ficheiros .freq; "printTerminal": escrita das informações no terminal; "moduloF": principal função do modulo.
*/

/*
    Esta função tem como objetivo calcular a taxa de compressão. Recebe o numero de bytes do bloco original e o 
    numero de bytes do bloco pós compressão RLE
    antes  -> tamanho do bloco original
    depois -> tamanho do bloco pós compressão
*/
float compressao (unsigned long antes, unsigned long depois);


/*
    Função que avalia se a compressão RLE compensa, retorna o valor 1 caso a taxa de compressão seja mais que 0.05 ou 5% (valor definido pelos docentes)
    bytes_lidos -> numero de bytes do bloco em questão
    tamArrRLE   -> tamanho do bloco depois da compressão
    result      -> variável que é devolvida com valor 0 ou 1 consoante o valor da compressão
*/
float fazerRLE ( unsigned long bytes_lidos, unsigned long tamArrRLE);


/*
    Função tem como propósito fazer a compressão RLE de um bloco de bytes de tamanho tamBlock e guardar o bloco covertido no array arr.
    A função analisa o bloco byte a byte, incrementando o numero de repetições a cada vez que o byte atual é igual ao seguinte.
    Quando o byte atual for diferente do seguinte ou igual a zero ou diferente e o numero de repetições (r) for maior que 3, o array guarda 3 bytes:
    o primeiro byte guarda o valor 0
    o segundo o byte que se está a repetir
    o terceiro guarda o número de repetições.
    No segundo cenário (quando o byte atual é diferente do seguinte mas o numero de repetições nao é superior a 3), o ciclo for guarda no array r repetições do byte atual
    
    bloco[]  -> bloco original que será comprimido
    tamBlock -> tamanho do bloco original (que será condição do ciclo for principal)
    *arr     -> array onde serão guardados os valores da compressão RLE
    r        -> contador de repetições (inicializado a 1)
*/
unsigned long toRLE(unsigned char bloco[],unsigned long tamBlock,unsigned char *arr);


/*
    Função que recebe um array de frequencias de um bloco, o tamanho do mesmo bloco e, com um ciclo for, grava no ficheiro as frequencias desse bloco
    e o tamanho do bloco na notação indicada pelos docentes no enunciado.

    o primeiro fprintf server para guardar o tamanho do bloco em questão na notação @tamanho@
    o ciclo for seguinte guarda as frequências dos 255 valores possíveis. 
        caso o valor seja 255 (último) e a frequência seja igual a anterior, nada é feito
        caso o a frequência seja igual à anterior, o byte em questão toma o valor ";"
        caso o valor seja o último, é guardado o valor na posição 255 do array de frequencias
        caso nenhum dos cenários anteriores se verifique, é guardado o valor correspondente do array de frequências freq.

    *fp      -> ficheiro onde será guardado
    tamBloco -> tamanho do bloco a ser guardado
    *freq    -> array das frequências do bloco
*/
void gravarFrequencias (FILE *fp, unsigned long tamBloco, unsigned long *freq);

/*
    Função que calcula a frequência de cada caracter

    *bufferBloco -> bloco que vai ser submetido ao calculo de frequencias
    *freq        -> array onde serão guardadas as frequências
    tamBloco     -> tamanho do bloco original
*/
void calculaFreqs (unsigned char *bufferBloco, unsigned long *freq, unsigned long tamBloco);



/*
    Função que reune instruções e invocações de funções auxiliares relativas às frequencias e ao ficheiro gerado.
    *fileF  -> ficheiro que será alterado
    tamanho -> tamanho do bloco
    *bloco  -> array com os bytes do bloco a ser processado
*/
void funcaoFREQ (FILE *fileF, unsigned long tamanho, unsigned char *bloco);



/*
    Função que define o sufixo do nome do ficheiro gerado e tambem o prefixo @R ou @N dentro do ficheiro, consoante a opção escolhida indicada pelo int
    modulo.
    modulo = 0 -> ficheiro .RLE , o nome do ficheiro é alterado para ter sufixo .rle
    modulo = 1 -> ficherio .RLE.FREQ , o nome do ficheiro é alterado para ter sufixo .rle.freq, além disso, é guardado no ficheiro o prefixo @R e o número de blocos
    modulo = 2 -> ficheiro .FREQ, o nome do ficheiro é alterado para ter sufixo .freq, além disso, é guardado no ficheiro o prefixo @N e o número de blocos

    *nomefile -> nome do ficheiro que vai ser concatenado com o sufixo adequado
    modulo    -> variável que indica as alterações que deverão ser feitas ao ficheiro
    *nomefinal-> array onde será guardado o nome concatenado do ficheiro
    nblock    -> número de blocos do ficheiro
*/
void inicializaFile (char *nomefile, int modulo, char *nomeFinal , long long nblock);


/*Função que calcula o tamanho de um ficheiro. Função definida sobretudo para calcular o tamanho de um comprimido RLE.
    *file -> ficheiro cujo tamanho se pretende determinar
    *tamBLock -> Tamanho de um bloco 
*/
unsigned long long  tamfile (char *file, unsigned long *tamBlock);

/*
    Função que reune instruções e invocações de funções auxiliares relativas às frequencias e ao ficheiro gerado.
    *fileF  -> ficheiro que será alterado
    tamanho -> tamanho do bloco
    *bloco  -> array com os bytes do bloco a ser processado
*/
void funcaoFREQ (FILE *fileF, unsigned long tamanho, unsigned char *bloco);

/*
    Função que imprime as informações necessárias no terminal, independentemente do tipo de compressão escolhido. além de escrever todos os dados necessários,
    também é nela que é invocada a função localtime para definir a data de execução do módulo. Os primeiros printf's são comuns a ambos os cenários, depois
    há uma condição if que verifica se houve compressão RLE, definindo aí se é necessário imprimir as linhas relativas à mesma.
*/
void printfTerminal (unsigned long *arrayTamRLE, long long nblock, unsigned long tamBlock, unsigned long size_of_last_block, char *fileFREQ, char *fileRLE, int obrigatorio,  double time_spent, unsigned long long sizefile );


/*
    A função principal do módulo F que tem as seguintes funcionalidades:
    -> inicia o relógio de contagem do tempo de execução
    -> abre o ficheiro e calcula o numero de blocos (guardado na variavel nblock) e o tamanho do ficheiro (guardado em sizefile)
    -> verifica se o último bloco (size_of_last_block) é menor que 1KB, caso essa situação se verifique, o ultimo bloco é adicionado ao penúltimo.
    -> aloca memória para um bufferBloco que, consoante o resultado do ponto anterior, terá o tamanh máximo do tamanho do bloco (selecionado pelo utilizador)
    ou o tamanho do último bloco
    -> se o número de blocos for 1, a variável bytes_lidos terá o valor do tamanho total do ficheiro, se o numero de blocos for maior que 1, então a variável
    terá o valor do tamanho do bloco. é neste passo também que o bloco lido é guardado no bufferBloco
    -> o apontador auxRLE aponta para memória alocada com tamanho bytes_lidos * 3
    -> é calculado o tamanho do bloco comprimido (valor que é guardado em tamRLE) ao invocar a função auxiliar toRLE
    -> se o utilizador forçar a compressão RLE ou está ser selecionada devido ao valor devolvido pela auxiliar fazerRLE, então é executado um grupo de auxiliares
            -> é chamada a auxiliar inicializaFile na opção 0 e depois é invocada de novo na opção 1, sendo o ficheiro aberto sempre que necessário
            -> a função fwrite guarda no ficheiro o array resultante da compressão (apontado por auxRLE) de tamanho tamRLE
            -> o tamanho do bloco comprimido é guardado no array arrayTamRLE na posição 0 (visto se tratar do primeiro bloco)
            -> é executada a função funcaoFREQ para calcular e guardar as frequencias deste bloco
            -> um ciclo for repete o processo para os blocos seguintes, tendo em atenção se o bloco a ser avaliado é o último e agindo em conformidade
    -> Se a compressão RLE nao for feita, a função inicializaFile é invocada na opção 2, o ficheiro é aberto e a função auxiliar funcaoFREQ é executada
    -> com um ciclo for semelhante ao relativo à compressão RLE, o processo anterior é aplicado a todos os outros blocos, sempre guardando o bloco lido no buffer adequado
    -> é libertado a memória e calculado o tempo de execução e invocada a função auxiliar printTerminal
*/
int moduloF (char *fileInicial, unsigned long tamBlock, int *obrigatorioRLE);

#endif //MODULOF_H
