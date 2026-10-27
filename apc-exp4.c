#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes Padrões do Sistema
#define ITEM 10       // Quantidade de itens do vetor
#define TAM 50         // Valor padrão para caracter
#define TABELA 100     // Largura da Tabela (terminal)


//=============================================================================================================================
//  FUNÇÕES AUXILIARES DE VISUALIZAÇÃO
//=============================================================================================================================

//Centraliza uma string com uma largura especificada
void centraliza (const char *txt, int largura)
{
    int tamanho = strlen(txt);
    // Caso o texto seja maior ou igual a largura, imprime diretamente
    if (tamanho >= largura)
    {
        printf("%s", txt);
    }
    // Calcula os espaços em branco para a centralização dos texto
    else
    {
        int espaco_esq = (largura - tamanho) / 2;                   // Calcula os espaços em branco à esquerda
        int espaco_dir = largura - tamanho - espaco_esq;            // Calcula os espaços em branco à direita
        printf("%*s%s%*s", espaco_esq, "", txt, espaco_dir, "");
    }
}

// Cria uma linha a partir divisória a repetindo a string "c" "n" vezes
void separador (const char *c,int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s",c);
    }
    printf("\n");
}

// Cria um banner de Título para representação das tabelas
void titulo (char *txt, int n)
{
    separador ("=-", n/2);
    centraliza(txt, n);
    printf("\n");
    separador("=-", n/2);
}

//=============================================================================================================================
// Processamento e Leitura dos dados do arquivo CSV
//=============================================================================================================================

int leitura_dados_csv (const char *nome_arquivo, char linha[ITEM][TAM], char destino[ITEM][TAM], int horario_min[ITEM])
{
    FILE *fp = fopen(nome_arquivo, "r");    // Abre o Arquivo base

    // Verifica a existência do arquivo com nome e local predefinido
    if (fp == NULL)
    {
        printf("[ERRO] Arquivo %s não reconhecido!\n", nome_arquivo);
        printf("Verifique novamente a existência do arquivo e tente novamente!\n");
        return 0;
    }

    // Verifica a existência de dados e descarta a primeira linha 
    char buffer [256];
    if(fgets(buffer, sizeof(buffer), fp) == NULL)
    {
        printf("[ERRO] Arquivo CSV vazio!\nConfirme os dados no arquivo original\n");
        fclose(fp);
        return 0;
    }

    int i = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL && i < ITEM)
    {
        char *token_linha = strtok(buffer, ",\n\r");
        char *token_destino = strtok(NULL, ",\n\r");
        char *token_horario = strtok(NULL, ",\n\r");

        if (token_linha && token_destino && token_horario)
        {
            // Armazena informação de linha
            strncpy(linha[i], token_linha, TAM - 1);
            linha[i][TAM - 1] = '\0';

            // Armazena informação de destino
            strncpy(destino[i], token_destino, TAM - 1);
            destino[i][TAM - 1] = '\0';

            // Converte formato "HH:MM" para minutos totais e armazena em horarios
            int h = 0, m = 0;
            if (sscanf(token_horario, "%d:%d", &h, &m) == 2)
            {
                horario_min[i] = h * 60 + m;
            }
            else
            {
                horario_min[i] = 0;
            }

            i++;
        }
    }

    fclose(fp);
    separador("-", TABELA);
    printf("Leitura concluida: %d Registros Carregados do CSV\n", i);
    separador("-", TABELA);
    return 1;
}

//=============================================================================================================================
// Cópia de vetores e Ordenamento BubbleSort
//=============================================================================================================================

// Faz uma cópia de segurança para ordenamento
void copiar_dados(
    char linha_O[][TAM], char linha_A[][TAM], 
    char destino_O[][TAM], char destino_A[][TAM], 
    int horario_O[], int horario_A[], 
    int tamanho) 
{
    for (int i = 0; i < tamanho; i++)
    {
        strcpy(linha_A[i], linha_O[i]);
        strcpy(destino_A[i], destino_O[i]);
        horario_A[i] = horario_O[i];
    }
}

// Ordenamento BubbleSort sincronizado aos vetores correspondentes
// Crescente (ordem = 1) - Decrescente (ordem = 0)
void bubble_sort (char linha[ITEM][TAM], char destino[ITEM][TAM], int horario[ITEM], int tamanho, int ordem)
{
    for (int i = 0; i < tamanho - 1; i++) 
    { 
        for (int j = 0; j < tamanho - i - 1; j++) 
        { 
            int precisa_trocar = 0; 
            if (ordem) 
            { 
                if (horario[j] > horario[j + 1]) precisa_trocar = 1; 
            } 
            else 
            { 
                if (horario[j] < horario[j + 1]) precisa_trocar = 1; 
            } 
            
            if (precisa_trocar) 
            { 
                // 1. Troca no vetor de horários (inteiros) 
                int temp_h = horario[j]; 
                horario[j] = horario[j + 1]; 
                horario[j + 1] = temp_h; 
                
                // 2. Troca no vetor de linhas (strings) 
                char temp_l[TAM]; 
                strcpy(temp_l, linha[j]); 
                strcpy(linha[j], linha[j + 1]); 
                strcpy(linha[j + 1], temp_l); 
                
                // 3. Troca no vetor de destinos (strings) 
                char temp_d[TAM]; 
                strcpy(temp_d, destino[j]); 
                strcpy(destino[j], destino[j + 1]); 
                strcpy(destino[j + 1], temp_d); 
            } 
        } 
    }
}

//=============================================================================================================================
// Exibição e Formatação dos dados no terminal
//=============================================================================================================================

void formata_horario (int min_total, char *horario, int tamanho)
{
    int h = min_total / 60;
    int m = min_total % 60;
    snprintf(horario, tamanho,"%02d:%02d", h, m);
}


void exibir_tabela (
    char *txt, 
    char linha[][TAM], 
    char destino[][TAM], 
    int horario[], 
    int tamanho) 
{
    titulo(txt, TABELA);

    centraliza("ORDEM", 6);
    printf("|");
    centraliza("LINHA", 12);
    printf("| %-32s | %-15s\n", "DESTINO", "HORARIO");
    separador("-", TABELA);
    for (int i = 0; i < tamanho; i++)
    {
        char str_hora[TAM];
        formata_horario(horario[i], str_hora, sizeof(str_hora));
        printf(" %4d |", i + 1);
        centraliza(linha[i], 12);
        printf("| %-32s | %-15s\n", destino[i], str_hora);
    }
    
    separador("=", TABELA);
    printf("\n");
    
}

//=============================================================================================================================
// FUNÇÃO PRINCIPAL
//=============================================================================================================================
int main (void)
{
    // Vetores de leitura
    char linha[ITEM][TAM];
    char destino[ITEM][TAM];
    int horario[ITEM];

    // Vetores de ordenamento cescente
    char linha_crescente[ITEM][TAM];
    char destino_crescente[ITEM][TAM];
    int horario_crescente[ITEM];

    // Vetores de ordenamento decescente
    char linha_decrescente[ITEM][TAM];
    char destino_decrescente[ITEM][TAM];
    int horario_decrescente[ITEM];

    // 1. Leitura do arquivo CSV
    if (!leitura_dados_csv("dados.csv", linha, destino, horario))
    {
        return 1;
    }

    // 2. Cópia de Segurança para ordenação
    copiar_dados(linha, linha_crescente, destino, destino_crescente, horario, horario_crescente, ITEM);

    copiar_dados(linha, linha_decrescente, destino, destino_decrescente, horario, horario_decrescente, ITEM);

    // 3. BubbleSort - Aplicação Crescente e Decrescente
    bubble_sort(linha_crescente, destino_crescente, horario_crescente, ITEM, 1);        // Crescente
    bubble_sort(linha_decrescente,  destino_decrescente, horario_decrescente, ITEM, 0); // Decrescente

    // 4. Exibição dos resultados no terminla
    exibir_tabela("DADOS ORIGINAIS", linha, destino, horario, ITEM); 
    exibir_tabela("ORDEM CRESCENTE", linha_crescente, destino_crescente, horario_crescente, ITEM);
    exibir_tabela("ORDEM DECRESCENTE", linha_decrescente, destino_decrescente, horario_decrescente, ITEM);
    
    titulo("PROCESSAMENTO FINALIZADO COM SUCESSO!!", TABELA);
    
    return 0;
}