#include <stdio.h>
#include <conio.h>
#include <conio2.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define TF 100

//#include "TAD-Pilha.h"
//#include "TAD-PilhaLg.h" //TAD da pilha para manipular nossa lista generalizada

//estrutura que vai armazenar nossos tokens
struct TpToken
{
	char info[TF];
	struct TpToken *prox;
};
typedef struct TpToken Token;

//lista que vai apontar para a lista de token de cada linha do programa
struct TpPrograma
{
	Token *token;
	struct TpPrograma *ant, *prox;
};
typedef struct TpPrograma Programa;

//estrutura que vai conter as informa��es das nossas variaveis e fazer o papel da RAM
struct TpVariavel
{
	char identificador[TF], valor[TF];
	Programa *ponteiro;
	struct TpVariavel *prox;
};
typedef struct TpVariavel variavel;

//fun��o que cria a caixa do nosso token
Token *CaixaToken(char *info) 
{
    Token *caixa = (Token*)malloc(sizeof(Token));
    strcpy(caixa->info, info);
    caixa->prox = NULL;
    return caixa;
}

//fun��o que cria a caixa da nossa lista que vai apontar para lista de tokens
Programa *CaixaPrograma() 
{
    Programa *caixa = (Programa*)malloc(sizeof(Programa));
    caixa->prox = NULL;
    caixa->ant = NULL;
    caixa->token = NULL;
    return caixa;
}

//fun��o que vai adicionar um novo token na lista de tokens
void AdicionarToken(Programa *l, char *info) 
{
    Token *caixa = CaixaToken(info);
    if(l->token == NULL)
        l->token = caixa;
	else
	{
        Token *aux = l->token;
        while(aux->prox != NULL)
            aux = aux->prox;
        aux->prox = caixa;
    }
   // printf("Token adicionado: '%s'\n", info);  // Teste para ver se separou os token certos
}

//fun��o que insere uma caixa de programa na lista para que cada caixa aponte para uma linha de tokens
void AdicionarPrograma(Programa **p, Programa *l)
{
    if(*p == NULL)
        *p = l;
    else
	{
        Programa *aux = *p;
        while(aux->prox != NULL)
            aux = aux->prox;
            
        aux->prox = l;
        l->ant = aux;
    }
}

//fun��o que verifica a existencia de um operador ou simbolo (utilizado na hora de tokenrizar)
int operador_simbolo(char caracter)
{
	if(caracter == '(' || caracter == ')' || caracter == '+' || caracter == '-' ||
	    caracter == '*' || caracter == '/' || caracter == '=' || caracter == ',' ||
	    caracter == '{' || caracter == '}' || caracter == ';' || caracter == '<' ||
	    caracter == '>' || caracter == '!' || caracter == '&' || caracter == '|' || 
	    caracter == '.')
	    return 1;
	return 0;
}

//fun��o que verifica a existencia de operadores composto (utilizado na hora de tokenrizar)
int operadoresComposto(char caracter, char caracter2)
{
	if((caracter == '=' && caracter2 == '=') ||
	    (caracter == '!' && caracter2 == '=') ||
	    (caracter == '<' && caracter2 == '=') ||
	    (caracter == '>' && caracter2 == '=') ||
	    (caracter == '+' && (caracter2 == '=' || caracter2 == '+')) ||
	    (caracter == '-' && (caracter2 == '=' || caracter2 == '-')) ||
	    (caracter == '*' && (caracter2 == '=' || caracter2 == '*')) ||
	    (caracter == '/' && caracter2 == '=') ||
	    (caracter == '&' && caracter2 == '&') ||
	    (caracter == '|' && caracter2 == '|'))
	    return 1;
	return 0;
}

//fun��o que verifica a existencia de um identificador (utilizado na hora de tokenzirar)
int identificador(char caracter)
{
	if(caracter != ' ' && caracter != '\t' && caracter != '\n' &&
		caracter != '(' && caracter != ')' && caracter != '+' &&
	    caracter != '-' && caracter != '*' && caracter != '/' &&
	    caracter != '=' && caracter != ',' && caracter != '{' &&
	    caracter != '}' && caracter != ';' && caracter != '<' &&
	    caracter != '>' && caracter != '!' && caracter != '&' &&
	    caracter != '|' && caracter != '.' && caracter != '\0')
	    return 1;
	return 0;
}

//fun��o que l� um arquivo que foi passado e armazena os tokens linha a linha
void lerArquivo(char *nomeArquivo, Programa **programa)
{
    FILE *ptr;
    char linha[256];
    Programa *novaLinha;
    char token[256];
    int i, j;

    ptr = fopen(nomeArquivo, "r");
    if(ptr == NULL) 
        printf("Erro ao abrir o arquivo\n");
    else
    {
    	while(fgets(linha, sizeof(linha), ptr)) 
		{
	        novaLinha = CaixaPrograma();
	        i = 0;
	
	        while(linha[i] != '\0') 
			{
	            //pula espa�os, tabs e quebras de linha
	            while(linha[i] == ' ' || linha[i] == '\t' || linha[i] == '\n')
	                i++;
	
	            j = 0;
	
	            //verifica se � um operador ou s�mbolo isolado
	            if(operador_simbolo(linha[i]) == 1) 
	            {
	                //verifica operadores compostos (==, !=, <=, >=, +=, -=, *=, /=, **, ++, --, &&, ||)
	                if(operadoresComposto(linha[i], linha[i+1]) == 1) 
	                {
	                    token[0] = linha[i];
	                    token[1] = linha[i+1];
	                    token[2] = '\0';
	                    i += 2;
	                }
					else
					{
	                    token[0] = linha[i++];
	                    token[1] = '\0';
	                }
	
	                AdicionarToken(novaLinha, token);
	            }
				else
				{
	                //le identificadores, n�meros, etc.
	                while(identificador(linha[i]) == 1) 
	                    token[j++] = linha[i++];
	                if(j > 0)
					{
	                    token[j] = '\0';
	                    AdicionarToken(novaLinha, token);
	                }
	            }
	        }
	        AdicionarPrograma(programa, novaLinha);
	    }
    	fclose(ptr);
    }
}

void ExibirPrograma(Programa *programa) {
    Programa *linhaAtual = programa;
    int numLinha = 1;

    while(linhaAtual != NULL)
	{
        printf("\nLinha %d: ", numLinha);
        Token *tokenAtual = linhaAtual->token;
        while(tokenAtual != NULL)
		{
            printf("%s ", tokenAtual->info);
            tokenAtual = tokenAtual->prox;
        }
        //printf("\n");
        linhaAtual = linhaAtual->prox;
        numLinha++;
    }
}

//TENHO QUE TERMINAR ESSA FUN��ES PARA EXIBIR LINHA A LINHA DO PROGRAMA
//fun��o que exibe o codigo linha a linha conforme for dando enter
void exibirExecucao(Programa *programa)
{
	Token *linha;
	
	while(pProg != NULL)
	{
		linha = programa->token;
		while(linha != NULL)
		{
			printf("%s",linha->token);
			linha=linha->prox;
		}
		printf("\n");
		pProg = pProg->prox;
	}
}

//fun��o que exibe o codigo linha a linha conforme for dando enter
void mostrarLinha(Programa *programa)
{
	Token *token = programa->token;
	
	while(token != NULL)
	{
		printf("%s",token->token);
		token = token->prox;
	}
	
	printf("\n");
	Mostrar_Excucao(atual->prox);
}

//fun��o que contem o menu do nosso simulador de execu��o
char menu()
{
	printf("\n\n[F7] - Abrir \t"); //F7 = 65
	printf("[F8] - Executar \t"); //F8 = 66
	printf("[F9] - Memoria RAM \t"); //F9 = 67
	printf("[F10] - Tela\t"); //F10 = 68
	return getch();
}

int main()
{
	//lista que vai apontar para a lista de tokens
    Programa *l = NULL; 
    
    char op, nomeArquivo[50];
	int result;
	
	do
	{
		op = menu();
		switch(op)
		{
			case 65: //F7
				printf("\nNome do arquivo: ");
				fflush(stdin);
				gets(nomeArquivo);
				
				lerArquivo(nomeArquivo, &l);
				
				//daqui para baixo chama a fun��o de abrir o arquivo
				//e com esse arquivo vamos tokenrizar para simular a execu��o
				
				break;
			
			case 66: //F8
				//aqui eu vou chamar a execu��o do programa linha a linha
				
				break;
				
			case 67: //F9
				//aqui eu vou mostrar a nossa memoria ram que vai ser nossa pilha de variavel
				printf("\n*** MEMORIA RAM ***\n");
				
				
				break;
			
			case 68: //F10
				//mostrar a tela exibindo os console.log
				ExibirPrograma(l);
				
				
				break;
		}
	}while(op != 27); //ESC = 27
    
    return 0;
}
