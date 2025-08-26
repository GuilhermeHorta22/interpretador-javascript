#include <stdio.h>
#include <conio.h>
#include <conio2.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define TF 90
#define max_linha 1024

struct TpToken
{
	char token[TF];
	struct TpToken *prox;
};
typedef struct TpToken Token;

struct TpPrograma
{
	Token *token;
	struct TpPrograma *ant, *prox;
};
typedef struct TpPrograma Programa;

struct TpVar
{
	char id;
	int valor;
	int *pont;
};
typedef struct TpVar Variavel;




// Função para criar uma nova lista
Programa  *CaixaLista() {
    Programa *caixa = (Programa*)malloc(sizeof(Programa));
    caixa->prox = NULL;
    caixa->ant = NULL;
    caixa->token = NULL;
    return caixa;
}

// Função para adicionar uma lista a lista principal
void AdicionarLista(Programa **p, Programa *l) {
    if (*p == NULL) {
        *p = l;
    } else {
        Programa *aux = *p;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = l;
        l->ant = aux; 
    }
}

// Função para adicionar um token na lista de tokens
void AdicionarToken(Programa *l, char *info) {
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

void lerArquivo(char* nomeArquivo, Programa** programa) {
    FILE *arquivo;
    char linha[256];
    Programa *novaLinha;
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
            // Pula espaços, tabs e quebras de linha
            while (linha[i] == ' ' || linha[i] == '\t' || linha[i] == '\n') {
                i++;
            }

            j = 0;

            // Verifica se é um operador ou símbolo isolado
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
                // Lê identificadores, números, etc.
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
