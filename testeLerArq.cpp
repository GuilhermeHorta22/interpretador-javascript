#include <stdio.h>
#include <conio.h>
#include <conio2.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//#include "TAD-Interpretador.h"

// STRUCTS
struct token {
    char info[60];
    struct token *prox;
};
typedef struct token Token;

struct Lista {
    struct Lista *prox, *ant;
    Token *token;
};
typedef struct Lista lista;

//FUNÇÕES CRIAR CAIXA

// Função para criar um novo token
Token *CaixaToken(char *info) {
    Token *caixa = (Token*)malloc(sizeof(Token));
    strcpy(caixa->info, info);
    caixa->prox = NULL;
    return caixa;
}

// Função para criar uma nova lista
lista *CaixaLista() {
    lista *caixa = (lista*)malloc(sizeof(lista));
    caixa->prox = NULL;
    caixa->ant = NULL;
    caixa->token = NULL;
    return caixa;
}

// Função para verificar se é delimitador
/*
int ehDelimitador(char c) {
    char delimitadores[] =
        " ", "\t", "\n", "(", ")", "{", "}", ";", "=", "+", "-", ">", "<", "*", "/", ".", ",", "==", ">=", "<=";
    for (int i = 0; delimitadores[i] != '\0'; i++) {
        if (c == delimitadores[i]) {
            return 1; // é delimitador
        }
    }
    return 0; // não é
}
*/

// FunÁ„o para adicionar um token na lista de tokens
void AdicionarToken(lista *l, char *info) {
    Token *caixa = CaixaToken(info);
    if (l->token == NULL) {
        l->token = caixa;
    } else {
        Token *aux = l->token;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = caixa;
    }
   // printf("Token adicionado: '%s'\n", info);  // Teste para ver se separou os token certos
}

// FunÁ„o para adicionar uma lista a lista principal
void AdicionarLista(lista **p, lista *l) {
    if (*p == NULL) {
        *p = l;
    } else {
        lista *aux = *p;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = l;
        l->ant = aux;
    }
}

void lerArquivo(char* nomeArquivo, lista** programa) {
    FILE *arquivo;
    char linha[256];
    lista *novaLinha;
    char token[256];
    int i, j;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        novaLinha = CaixaLista();
        i = 0;

        while (linha[i] != '\0') {
            // Pula espa�os, tabs e quebras de linha
            while (linha[i] == ' ' || linha[i] == '\t' || linha[i] == '\n') {
                i++;
            }

            j = 0;

            // Verifica se � um operador ou s�mbolo isolado
            if (linha[i] == '(' || linha[i] == ')' || linha[i] == '+' || linha[i] == '-' ||
                linha[i] == '*' || linha[i] == '/' || linha[i] == '=' || linha[i] == ',' ||
                linha[i] == '{' || linha[i] == '}' || linha[i] == ';' || linha[i] == '<' ||
                linha[i] == '>' || linha[i] == '!' || linha[i] == '&' || linha[i] == '|' || 
                linha[i] == '.') 
            {
                // Verifica operadores compostos (==, !=, <=, >=, +=, -=, *=, /=, **, ++, --, &&, ||)
                if ((linha[i] == '=' && linha[i+1] == '=') ||
                    (linha[i] == '!' && linha[i+1] == '=') ||
                    (linha[i] == '<' && linha[i+1] == '=') ||
                    (linha[i] == '>' && linha[i+1] == '=') ||
                    (linha[i] == '+' && (linha[i+1] == '=' || linha[i+1] == '+')) ||
                    (linha[i] == '-' && (linha[i+1] == '=' || linha[i+1] == '-')) ||
                    (linha[i] == '*' && (linha[i+1] == '=' || linha[i+1] == '*')) ||
                    (linha[i] == '/' && linha[i+1] == '=') ||
                    (linha[i] == '&' && linha[i+1] == '&') ||
                    (linha[i] == '|' && linha[i+1] == '|')) 
                {
                    token[0] = linha[i];
                    token[1] = linha[i+1];
                    token[2] = '\0';
                    i += 2;
                } else {
                    token[0] = linha[i++];
                    token[1] = '\0';
                }

                AdicionarToken(novaLinha, token);
            } else {
                // L� identificadores, n�meros, etc.
                while (linha[i] != ' ' && linha[i] != '\t' && linha[i] != '\n' &&
                       linha[i] != '(' && linha[i] != ')' && linha[i] != '+' &&
                       linha[i] != '-' && linha[i] != '*' && linha[i] != '/' &&
                       linha[i] != '=' && linha[i] != ',' && linha[i] != '{' &&
                       linha[i] != '}' && linha[i] != ';' && linha[i] != '<' &&
                       linha[i] != '>' && linha[i] != '!' && linha[i] != '&' &&
                       linha[i] != '|' && linha[i] != '.' && linha[i] != '\0') 
                {
                    token[j++] = linha[i++];
                }
                if (j > 0) {
                    token[j] = '\0';
                    AdicionarToken(novaLinha, token);
                }
            }
        }
        AdicionarLista(programa, novaLinha);
    }

    fclose(arquivo);
}

void ExibirPrograma(lista *programa) {
    lista *linhaAtual = programa;
    int numLinha = 1;

    while (linhaAtual != NULL) {
        printf("Linha %d: ", numLinha);
        Token *tokenAtual = linhaAtual->token;
        while (tokenAtual != NULL) {
            printf("%s ", tokenAtual->info);
            tokenAtual = tokenAtual->prox;
        }
        printf("\n");
        linhaAtual = linhaAtual->prox;
        numLinha++;
    }
}

/*
int main() {
   lista *l = NULL;
   Node *PilhaTokens = NULL;
   Node *PilhaResul= NULL;
   SimboloVariavel *variaveis = NULL;
   float resul;
   char opc = 0, arquivo[50];
   char equa[] = "3 + 2 * 2";
   
   
   while (opc != 27) {
       opc = Menu();

       switch(opc) {
           case 65: // F7
               clrscr();
               printf("Nome do arquivo:\n");
               fflush(stdin);
               gets(arquivo);
               lerArquivo(arquivo, &l);
               ExibirPrograma(l);
               break;

           case 66: // F8
               clrscr();
               PushVariaveis(&PilhaTokens, l);
               exibirPilha(PilhaTokens);
               break;

           case 67: // F9
               clrscr();
               opc = menu2();
               switch(opc) {
                   case 1://returns
                       EncontraReturn(&l);
                       break;
                   case 2://chamadas funÁ„o
                      // ProcessarPrograma(l, &pilha);
                       break;
                   case 3:
                       resul = Resolve(equa);
                       printf("\nResultado: %f\n", resul);
                       break;
                   case 4:
                       // ifs
                       break;
                   case 5:
                       resul = Resolve(equa);
                       printf("\nResultado: %f\n", resul);
                       break;
                   case 6:
                       // while e for
                       break;
                   case 7://prints
                           ArmazenarVariaveisParaPrint(&variaveis, l);
                           ExibirPrints(&l, variaveis);
                       break;
                   case 8:
                       printf("Opcao invalida! Tente novamente.\n");
                       break;
               }
               break;

           case 27:
               printf("Saindo do programa...\n");
               break;

           case 0:
           case -32:
               printf("Opcao invalida! Tente novamente.\n");
               break;
       }
   }

   return 0;
}
*/

int main()
{
    lista *l = NULL;
    char arquivo[50];
    printf("Nome do arquivo:\n");
    fflush(stdin);
    gets(arquivo);
    lerArquivo(arquivo, &l);
    ExibirPrograma(l);
    
    return 0;
}
