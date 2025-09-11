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

//estrutura que vai conter as informacoes das nossas variaveis e fazer o papel da RAM
struct TpVariavel
{
	char identificador[TF], valor[TF];
	Programa *ponteiro;
	struct TpVariavel *prox;
	int tipo; // 0 = LET 1 = CONST
};
typedef struct TpVariavel Variavel;

//estrutura que vai conter as informacao para o controle de exibiicao do nosso codigo
struct TpControle
{
	int chave, l, c;
	Programa *local;
	struct TpControle *prox;
};
typedef struct TpControle Controle;

//estrutura que vai conter as informacao das functions do nosso programa
struct TpFuncoes
{
	int l;
	Programa *local;
	char function[TF];
	struct TpFuncoes *prox;
};
typedef struct TpFuncoes Funcoes;

//estrutura que vai controlar a manipulacao de bloco for no nosso programa
struct TpControleFor
{
	int start, stop, step, atual;
	char variavel[30];
	struct TpControleFor *prox;
};
typedef struct TpControleFor controleFor;

//estrutura que vai conter nosso console.log
struct TpListaEncadeada
{
	char info[100];
	struct TpListaEncadeada *prox;
};
typedef struct TpListaEncadeada listaEncadeada;

//Estrutura da Lista Generalizada

union info_lista
{
	int valor;
	char operador[3];
	//implementar fun��o tbm!!!
};

struct TpListaGen
{
	char terminal;
	union info_lista no;
	struct TpListaGen *cabeca, *cauda;
};
typedef struct TpListaGen ListaGen;

// -- TAD LISTA GEN ---

void initLG(ListaGen **lista)
{
	*lista = NULL;
}

// -- TAD PILHA CONTROLE --

//funcao que vai inicializar nossa pilha de controle
void initC(Controle **c)
{
	*c = NULL;
}

//funcao que verifica se nossa pilha de controle est� vazia
char isEmptyC(Controle *c)
{
	return c == NULL;
}

//funcao que insere informacao na nossa pilha de controle
void pushC(Controle **c, int chave, Programa *p, int lin, int col)
{
	Controle *nova = (Controle*)malloc(sizeof(Controle));
	nova->chave = chave;
	nova->l = lin;
	nova->c = col;
	nova->local = p;
	nova->prox = *c;
	*c = nova;
}

//funcaoo que vai retirar as informacao da nossa pilha de controle
Controle popC(Controle **c)
{
	Controle *aux, cAtual;
	if(!isEmptyC(*c))
	{
		cAtual.chave = (*c)->chave;
		cAtual.l = (*c)->l;
		cAtual.c = (*c)->c;
		cAtual.local = (*c)->local;
		aux = *c;
		*c = (*c)->prox;
		free(aux);
	}
	else
		cAtual.chave = -1;
		
	return cAtual;
}

//funcao que envia o ultimo elemento da pilha sem modificar ela
void ultimoC(Controle *c, Controle **auxC)
{
	if(!isEmptyC(c))
	{
		while(!isEmptyC(c->prox))
			c = c->prox;
	}
	*auxC = c;
}

void topC(Controle *c, Controle **auxC)
{
	*auxC = c;
}


// -- TAD FILA LISTA ENCADEADA --

//funcao que inicializa a fila de LE
void initLE(listaEncadeada **le)
{
	*le = NULL;
}

//funcao que aloca uma nova caixinha para fila de LE
listaEncadeada *criaCaixaLE(char *info)
{
	listaEncadeada *nova = (listaEncadeada*)malloc(sizeof(listaEncadeada));
	strcpy(nova->info,info);
	nova->prox = NULL;
	return nova;
}

//funcao que insere informacao na fila de LE
void enqueueLE(listaEncadeada **le, char *info)
{
	listaEncadeada *aux = *le, *nova = criaCaixaLE(info);
	
	if(aux == NULL)
		*le = nova;
	else
	{
		while(aux->prox != NULL)
			aux = aux->prox;
			
		aux->prox = nova;
	}
}

//funcao que retira uma informacao da fila de LE
void dequeueLE(listaEncadeada **le, char *info)
{
	listaEncadeada *aux;
	
	if((*le) == NULL)
		strcpy(info, "");
	else
	{
		strcpy(info, (*le)->info);
		aux = *le;
		*le = (*le)->prox;
		free(aux);
	}
}

//funcao que exibe o conteudo da fila LE
void exibirLE(listaEncadeada *le)
{
	listaEncadeada *aux = le;
	
	if(aux == NULL)
		printf("\nLista encadeada de (console.log) esta vazia!");
	else
	{
		printf("Conteudo lista encadeada: ");
		while(aux != NULL)
		{
			printf("%s",aux->info);
			aux = aux->prox;
		}
		printf("\n");
	}
}


// -- TAD FILA LISTA ENCADEADA --

void initF(Funcoes **f)
{
	*f = NULL;
}

char isEmptyF(Funcoes *f)
{
	return f == NULL;
}

void enqueueF(Funcoes **f, char *fun, int l, Programa *local)
{
	Funcoes *nova = (Funcoes*)malloc(sizeof(Funcoes));
	
	strcpy(nova->function, fun);
	nova->l = l;
	nova->local = local;
	nova->prox = NULL;
	
	if(isEmptyF(*f))
		*f = nova;
	else
	{
		Funcoes *aux = *f;
		while(aux->prox != NULL)
			aux = aux->prox;
		aux->prox = nova;
	}
}


// -- TAD PILHA VARIAVEL --

//funcao que inicializa a pilha da variavel
void initPV(Variavel **pilhaVar)
{
	*pilhaVar = NULL;
}

//funcao que verifica se a pilha de variavel e nula ou vazia
char isEmptyPV(Variavel *pilhaVar)
{
	return pilhaVar == NULL;
}

//funcao que insere a variavel dentro da nossa pilha de variavel
void pushPV(Variavel **pilhaVar, Variavel auxVar)
{
	Variavel *nova = (Variavel*)malloc(sizeof(Variavel));
	
	strcpy(nova->valor, auxVar.valor);
	strcpy(nova->identificador, auxVar.identificador);
	nova->ponteiro = auxVar.ponteiro;
	nova->tipo = auxVar.tipo;
		
	nova->prox = *pilhaVar;
	*pilhaVar = nova;
	
	//DEBUG
	/*
	printf("\n\nPUSH realizado");
	printf("\nIdentificador: %s",(*pilhaVar)->identificador);
	printf("\nValor: %s",(*pilhaVar)->valor);
	printf("\nPonteiro: %p",(*pilhaVar)->ponteiro);
	printf("\nTipo: %d", (*pilhaVar)->tipo);
	*/
}

//funcao que vai retirar uma variavel da nossa pilha de variavel
void popPV(Variavel **pv, Variavel **pvAux)
{
	if(!isEmptyPV(*pv))
	{
		*pvAux = (Variavel*)malloc(sizeof(Variavel));
		Variavel *aux; //vai ser usado para deletar a caixinha
		
		//fazendo o auxiliar que vem por parametro receber as informacoes
		strcpy((*pvAux)->valor, (*pv)->valor);
		strcpy((*pvAux)->identificador, (*pv)->identificador);
		(*pvAux)->ponteiro = (*pv)->ponteiro;
		(*pvAux)->prox = NULL;
		
		aux = *pv;
		*pv = (*pv)->prox;
		free(aux);
	}
}

//funcao que vai tirar uma variavel da pilha pelo identificador (indepentemente se e topo ou nao)
void popIdentificadorPV(Variavel **pv, char *identificador)
{
	Variavel *atual = *pv, *ant = NULL;
	
	while(atual != NULL)
	{
		if(strcmp(atual->identificador, identificador) == 0)
		{
			if(ant == NULL) //1� caso
				*pv = atual->prox;
			else
				ant->prox = atual->prox;
			
			free(atual);
			atual = NULL; //para encerrar a repeticao
		}
		else
		{
			ant = atual;
			atual = atual->prox;
		}
	}
}

//funcao que vai retornar o topo da nossa pilha de variavel
void topPV(Variavel *pv, Variavel **pvAux)
{
	*pvAux = pv;
}

//funcao que busca uma variavel na pilha de variavel pelo identificador
Variavel *buscarIdentPV(Variavel *pv, char *identificador)
{
	while(pv != NULL && strcmp(pv->identificador, identificador) != 0)
		pv = pv->prox;
		
	if(pv != NULL)
		return pv;
	return NULL; //nao achou
}

//funcao que busca um token dentro da lista
Programa *buscarToken(Programa *programa, char *tokenBusca)
{
	Token *token;
	
	while(programa != NULL)
	{
		token = programa->token;
		while(token != NULL)
		{
			if(strcmp(token->info, tokenBusca) == 0)
				return programa; // lista do programa que tem o token dentro
			token = token->prox;
		}
		programa = programa->prox;
	}
	return NULL; //nao achou o token
}

//funcao que exibe o nome e o local das funcoes do programa
void exibeFuncoes(Funcoes *funcoes)
{
	int cont = 1;
	while(funcoes != NULL)
	{
		printf("\nFuncao %d: %s",cont,funcoes->function);
		printf("\nLocal: %p\n",funcoes->local);
		cont++;
		funcoes = funcoes->prox;
	}
}

//funcao que cria a caixa do nosso token
Token *CaixaToken(char *info) 
{
    Token *caixa = (Token*)malloc(sizeof(Token));
    strcpy(caixa->info, info);
    caixa->prox = NULL;
    return caixa;
}

//funcao que cria a caixa da nossa lista que vai apontar para lista de tokens
Programa *CaixaPrograma() 
{
    Programa *caixa = (Programa*)malloc(sizeof(Programa));
    caixa->prox = NULL;
    caixa->ant = NULL;
    caixa->token = NULL;
    return caixa;
}

//funcao que vai adicionar um novo token na lista de tokens
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

//funcao que insere uma caixa de programa na lista para que cada caixa aponte para uma linha de tokens
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

//funcao que adiciona o token na linha
void AdicionarTokenLinha(Token **token, char *info)
{
	Token *caixa = CaixaToken(info);
	if(*token == NULL)
		*token = caixa;
	else
	{
		Token *aux = *token;
		while(aux->prox != NULL)
			aux = aux->prox;
		aux->prox = caixa;
	}
}

//funcao que verifica se caracter � um operador matem�tico (utilizado na hora verifica��o de express�o matem�tica)
int operadorMatematico(char *caracter)
{
	//printf("\n\nENTROU NA FUNCAO OPERADOR!!!");
	if(strcmp(caracter,"+")==0 || strcmp(caracter,"-")==0 || 
	strcmp(caracter,"*")==0 || strcmp(caracter,"/")==0 ||
	strcmp(caracter,"%") == 0 || strcmp(caracter,"**") == 0)
	{
		//printf("\n\nEH OPERADOR!!!");
		return 1;
	}
		
	//printf("\n\nNAO EH OPERADOR MATEMATICOOOOO!!!");
	return 0;
}

//funcao que verifica a existencia de um operador ou simbolo (utilizado na hora de tokenrizar)
int operador_simbolo(char caracter)
{
	if(caracter == '(' || caracter == ')' || caracter == '+' || caracter == '-' ||
	    caracter == '*' || caracter == '/' || caracter == '=' || caracter == ',' ||
	    caracter == '{' || caracter == '}' || caracter == ';' || caracter == '<' ||
	    caracter == '>' || caracter == '!' || caracter == '&' || caracter == '|')
	    return 1;
	return 0;
}

//funcao que verifica a existencia de operadores composto (utilizado na hora de tokenrizar)
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

//funcao que verifica a existencia de um identificador (utilizado na hora de tokenzirar)
int identificador(char caracter)
{
	if(caracter != ' ' && caracter != '\t' && caracter != '\n' &&
		caracter != '(' && caracter != ')' && caracter != '+' &&
	    caracter != '-' && caracter != '*' && caracter != '/' &&
	    caracter != '=' && caracter != ',' && caracter != '{' &&
	    caracter != '}' && caracter != ';' && caracter != '<' &&
	    caracter != '>' && caracter != '!' && caracter != '&' &&
	    caracter != '|' && caracter != '\0' && caracter != '.')
	    return 1;
	return 0;
}

int numeric(char *caracter)
{
	if(caracter[0] >= 48 && caracter[0] <= 57)
		return 1;
	return 0;
}

//CAIO - POSICIONAR JUNTO COM FUNCOES PARECIDAS
int isTipoVariavel(char *info) 
{
	if(strcmp(info, "const") == 0) //e uma const
		return 1;
	if(strcmp(info, "let") == 0) //e uma let
		return 0;
	return -1; // nao e uma variavel
}


// -------------- FUNCOES QUE TRATA EXPRESS�O MATEMATICA ------------------

struct pilhaLG
{
	ListaGen *caixa;
	struct pilhaLG *prox;
};
typedef struct pilhaLG PilhaLG;

struct no_pilhaValor
{
	float valor;
	struct no_pilhaValor *prox;
};
typedef struct no_pilhaValor NoPilhaValor;

struct pilhavalor
{
	struct no_pilhaValor *topo;	
};
typedef struct pilhavalor PilhaValor;

struct no_pilhaOperador
{
	char operador[3];
	struct no_pilhaOperador *prox;
};
typedef struct no_pilhaOperador NoPilhaOperador;

struct pilhaoperador
{
	struct no_pilhaOperador *topo;
};
typedef struct pilhaoperador PilhaOperador;


void initPLG(PilhaLG **P)
{
	*P = NULL;
}

char Nula(ListaGen *L)
{
	return L == NULL;
}

char isEmptyPilhaOperador(PilhaOperador * P)
{
	return P -> topo == NULL;
}

void pushPLG(PilhaLG **P, ListaGen *aux)
{
	PilhaLG *nova = (PilhaLG*)malloc(sizeof(PilhaLG));
	
	nova->caixa = aux;
	nova->prox = *P;
	*P = nova;
}

ListaGen popLG(PilhaLG **P, ListaGen **aux)
{
	PilhaLG *removido = (PilhaLG*)malloc(sizeof(PilhaLG));
	
	*aux = (*P)->caixa;
	removido = *P;
	*P = (*P)->prox;
	free(removido);
}

ListaGen *novaProf()
{
	ListaGen *nova = (ListaGen*)malloc(sizeof(ListaGen));
	nova->terminal = 'P';
	nova->no.valor = 0;
	nova->cabeca = nova->cauda = NULL;
	return nova;
}

ListaGen *novaV(int valor)
{
	ListaGen *nova = (ListaGen*)malloc(sizeof(ListaGen));
	nova->terminal = 'V';
	nova->no.valor = valor;
	nova->cabeca = nova->cauda = NULL;
	return nova;
}

ListaGen *novaO(char *operador)
{
	ListaGen *nova = (ListaGen*)malloc(sizeof(ListaGen));
	nova->terminal = 'O';
	strcpy(nova->no.operador,operador);
	nova->cabeca = nova->cauda = NULL;
	return nova;
}

void pushValor(PilhaValor **P, float valor)
{
	no_pilhaValor *novoNo = (no_pilhaValor *)malloc(sizeof(no_pilhaValor));
	novoNo -> valor = valor;
	novoNo -> prox = (*P) -> topo;
	(*P) -> topo = novoNo; 
}

void pushOperador(PilhaOperador **P, char *operador)
{
	NoPilhaOperador *novoNo = (NoPilhaOperador *)malloc(sizeof(NoPilhaOperador));
	strcpy(novoNo->operador, operador);
	novoNo->prox = (*P)->topo;
	(*P)->topo = novoNo;
}

void popValor(PilhaValor **P, float * valor)
{
	*valor = (*P) -> topo -> valor;
	NoPilhaValor *noRemovido = (*P) -> topo;
	(*P) -> topo = (*P) -> topo -> prox;
	free(noRemovido);
}

void popOperador(PilhaOperador **P, char *operador)
{
	strcpy(operador, (*P) -> topo -> operador);
	NoPilhaOperador *noRemovido = (*P) -> topo;
	(*P) -> topo = (*P) -> topo -> prox;
	free(noRemovido);
}

//funcao que busca operadores matem�ticos (utilizado para tratar caso seja uma express�o matem�tica)
int procuraOperador(Token* procura)
{
	//printf("\n\n\tEntrou na funcao procura");
	while(procura != NULL && !operadorMatematico(procura->info))
		procura = procura->prox;
	//printf("\n\n\tSaiu do While");
	if(procura != NULL)
		return 1;
	return 0;
}

void destroiLista(ListaGen **lista)
{
	 if((*lista)!=NULL)
	 {
	 		destroiLista(&(*lista)->cabeca);
	 		destroiLista(&(*lista)->cauda);
	 		free(*lista);
	 		*lista = NULL;
	 }
}

void exibeLG(ListaGen *lista)
{
	if(lista != NULL)
	{
		if(lista->terminal == 'V')
			printf("%d",lista->no.valor);
		else if(lista->terminal == 'O')
			printf("%c",lista->no.operador);
		else if(lista->terminal == 'P')
			printf("(");
		exibeLG(lista->cabeca);
		exibeLG(lista->cauda);
	}
}

void constroiLG(ListaGen **lista, Token *token)
{
	char auxO[3], auxF[20];
    int auxV;
	ListaGen *nova, *aux;
	PilhaLG *P;
	initPLG(&P);
	
	//printf("\nEntrou na CONSTROI");
	if((*lista)!=NULL)
		destroiLista(&(*lista)); //RESETA a lista caso j� foi usada para uma expressao anterior
	
	aux = (*lista);
		while(token!=NULL)
		{
			//printf("\nVALOR DO TOKEN: %s",(token)->info);
			if(strcmp(token->info,"(")==0)
			{
				//printf("\nABRE PARENTESE");
				nova = novaProf();
				aux->cauda = nova;
				token = token->prox;
				if(numeric(token->info))
                {
                //	printf("\nVALOR DO TOKEN: %s",(token)->info);
                //	printf("\nNUMERIC DO PARENTESE");
                    auxV = atoi(token->info);
                    nova->cabeca = novaV(auxV);
                }
					
				//if(isVariavel(caracter))
				//if(isFuncao(caracter))
				aux = nova->cabeca;
				pushPLG(&P,nova);
			}
			else if(numeric(token->info))
			{
			//	printf("\nNUMERIC");
                auxV = atoi((token)->info);
                //printf("\n\n %d",auxV);
				nova = novaV(auxV);
				if(aux == NULL)
					(*lista) = aux = nova;
				else
				{
				//	printf("\n\nEntrou no ELSE");
					aux->cauda = nova;
					aux = nova;
				}
			}
			else if(operadorMatematico(token->info))
			{
			//	printf("\n OPERADOR");
				strcpy(auxO,token->info);
				nova = novaO(auxO);
				aux->cauda = nova;
				aux = nova;
			}
            /*
             else if(funcao(token->info)
             {
             }
             else if(variavel(token->info)
             {
             }
             */
			else if(strcmp(token->info,")")==0)
			{
			//	printf("\n FECHA PARENTESE");
				popLG(&P,&aux);
			}
				
			
			token = token->prox;
		}
		exibeLG(*lista);
}

void initPOperador(PilhaOperador **P)
{
	*P = (PilhaOperador*)malloc(sizeof(PilhaOperador));
	(*P) -> topo = NULL;
}

void initPValor(PilhaValor **P)
{
	*P = (PilhaValor*)malloc(sizeof(PilhaValor));
	(*P) -> topo = NULL;
}

int prioridade(char *operador) 
{
    if(strcmp(operador, "+") == 0 || strcmp(operador, "-") == 0) 
        return 1;
    else 
	if(strcmp(operador, "*") == 0 || strcmp(operador, "/") == 0 || strcmp(operador, "%") == 0 || strcmp(operador, "//") == 0)
        return 2;
    else 
	if(strcmp(operador, "**") == 0)
        return 3;
    return 0; // Para operadores n�o reconhecidos
}

// %(resto) int ou float s� precisa do float pq o int usa %
float restoF(float num, float div)
{
	float res;
	
	res = num/div;
	res = div*(int)res;
	res = num-res;
	
	return res;
}

union  INFO
{
	char str[30];
	int i;
	float f;
};

float Expoen(float nume, float Ex)
{
	INFO res;
	res.f = nume;
	
	if(Ex != 0)
	{
		while(Ex > 1)
		{
			res.f =  res.f * nume;
			Ex--;
		}
	}
	else
	{
		res.f = 1;
	}
	
	return res.f;
}

float calculaEquacao(ListaGen *caixa) 
{
    PilhaOperador *POperador;
    PilhaValor *PValor;
    
    initPOperador(&POperador); 
    initPValor(&PValor);
    
    char op[3];
    float resultado, subResultado, val1, val2;
    
    while(!Nula(caixa)) {
        if(caixa->terminal == 'V') 
            pushValor(&PValor, caixa->no.valor);
        else if(caixa->terminal == 'P')
        {
        	float subResultado = calculaEquacao(caixa->cabeca);
    		pushValor(&PValor, subResultado);
        }
        else if(caixa->terminal == 'O') {
            
			while(!isEmptyPilhaOperador(POperador) && prioridade(caixa->no.operador) <= prioridade(POperador->topo->operador)) 
			{	
                popOperador(&POperador, op);
                popValor(&PValor, &val1);
                popValor(&PValor, &val2);
                
                if (strcmp(op, "+") == 0) {
				    pushValor(&PValor, val2 + val1);
				} else if (strcmp(op, "-") == 0) {
				    pushValor(&PValor, val2 - val1);
				} else if (strcmp(op, "*") == 0) {
				    pushValor(&PValor, val2 * val1);
				} else if (strcmp(op, "/") == 0) {
				    pushValor(&PValor, val2 / val1);
				} else if (strcmp(op, "%") == 0) {
					pushValor(&PValor, restoF(val1, val2));
				} else if (strcmp(op, "//") == 0) {
					pushValor(&PValor, (int)val2/(int)val1);
				} else if (strcmp(op, "**") == 0) {
				    pushValor(&PValor, Expoen(val2, val1));
				}
            }
            pushOperador(&POperador, caixa->no.operador);
        }
        caixa = caixa->cauda;
    }
    
    while(!isEmptyPilhaOperador(POperador)) {
        popOperador(&POperador, op);
        popValor(&PValor, &val1);
        popValor(&PValor, &val2);
     
        if (strcmp(op, "+") == 0) {
		    pushValor(&PValor, val2 + val1);
		} else if (strcmp(op, "-") == 0) {
		    pushValor(&PValor, val2 - val1);
		} else if (strcmp(op, "*") == 0) {
		    pushValor(&PValor, val2 * val1);
		} else if (strcmp(op, "/") == 0) {
		    pushValor(&PValor, val2 / val1);
		} else if (strcmp(op, "%") == 0) {
			pushValor(&PValor, restoF(val1, val2));
		} else if (strcmp(op, "//") == 0) {
			pushValor(&PValor, (int)val2/(int)val1);
		} else if (strcmp(op, "**") == 0) {
		    pushValor(&PValor, Expoen(val2, val1));
		}	
    }
    popValor(&PValor, &resultado);
    return resultado;
}


// -------------- FUNCOES QUE TRATA O CONSOLE.LOG ------------------

//funcao que verifica se e uma string
int ehString(char *token)
{
	int length = strlen(token);
	
	//verifica se uma string vazia com aspas duplas ou simples
	if(strcmp(token,"\"") == 0  || strcmp(token,"'") == 0)
		return 1;
	
	//verifica se comeca e termina com aspas duplas
	if(length >=2 && token[0] == '\"' && token[length-1] == '\"')
		return 1;
		
	//verifica se comeca e termina com aspas simples
	if(length >=2 && token[0] == '\'' && token[length-1] == '\'')
		return 1;
		
	return 0;
}

//funcao que pula os espacos que tiver
void pulaEspacos(Token **linha)
{
	while(*linha != NULL && (strcmp((*linha)->info," ") == 0 || strcmp((*linha)->info,"\t") == 0))
		*linha = (*linha)->prox;
	printf("\n     pulou espaco (pularEspacos)");
}

//funcao que verifica qual o tipo da expressao presente no console.log
char tipoExpressao(Token *atual) 
{
    int encontrouOperadorMatematico = 0;
    int concatenacaoString = 0;

    Token *anterior = NULL;

	printf("\n22-  antes do while principal(tipoExpressao)");
    while(atual != NULL) 
    {
        if(strcmp(atual->info, " ") != 0)
            anterior = atual;

		printf("\n23-  dentro do while principal(tipoExpressao)");
        //operador '+' indica concatena��o de strings ou opera��o matem�tica
        if(strcmp(atual->info,"+") == 0 && anterior != NULL && atual->prox != NULL) 
        {
        	printf("\n24-  dentro do if se o token foi um '+'(tipoExpressao)");

            Token *proximo = atual->prox;

            if(ehString(anterior->info) && ehString(proximo->info)) 
                concatenacaoString = 1;
            else
                encontrouOperadorMatematico = 1;
            printf("\n25-  apos descobrir soma ou concatenacao(tipoExpressao)");
        }
        if(strcmp(atual->info,",") == 0)
		{
			printf("\n24-  se caso for uma ","(tipoExpressao)");

			//apenas marca fim do argumento, nada a fazer aqui
		} 
        else 
		if(operadorMatematico(atual->info))
		{
			printf("\n24- se caso for um operador matematico(tipoExpressao)");
			encontrouOperadorMatematico = 1;
		}
            

        atual = atual->prox;
    }

	printf("\n26-  antes dos returno se eh M ou S(tipoExpressao)");
    if(concatenacaoString == 1) 
        return 'S'; //string ou concatena��o
    if(encontrouOperadorMatematico == 1)
        return 'M'; //matematica
    return 'S'; //simples (string, numero, variavel, etc.)
}

//funcao que resolve qual o tipo da express�o presente no meu console.log
Token *resolConLog(Programa *programa, Variavel *pv, Funcoes *funcoes)
{
	char tipoExp, bol, caracter[10];
	Token *novo, *novo2, *linha;
	linha = programa->token;
	Variavel *pvAux;
	initPV(&pvAux);
	
	printf("\n19- estou antes de chamar a funcao tipoExpressao (resol con log)");
	
	tipoExp = tipoExpressao(linha);
	printf("\n20- estou apos chamar a funcao tipoExpressao (resol con log)");
	if(tipoExp == 'M')
	{
		strcpy(caracter,"R");
		novo = CaixaToken(caracter);
		
		strcpy(caracter,"=");
		novo2 = CaixaToken(caracter);
		novo2->prox = linha;
		
		novo->prox = novo2;
		programa->token = novo;
		
		//atribuicao(programa,&pv,funcao);
		
		popPV(&pv,&pvAux);
		strcpy(novo->info,pvAux->valor);
		printf("\n21- estou dentro do if tipoExp == M (resol con log)");
	}
	else
	{
		printf("\21- estou dentro do else tipoExp == S (resol con log)");
		novo = linha;
	}
	return novo;
}

//funcao que separa as expressoes dentro de um console.log
Programa *separaExpressoes(Programa *ant, Variavel **pv, Funcoes *funcoes)
{
    Programa *atual = ant;

    if(atual == NULL || atual->token == NULL)
        return atual;

    Token *linha = atual->token, *atr = NULL, *novoT = NULL, *c = NULL, *auxT = NULL, *token = NULL;
    Programa *novaP = NULL, *cabeca = NULL;

    char div[10], aspa, buffer[200];
    int length;

    //pulando console.log
    if(linha != NULL && linha->prox != NULL && strcmp(linha->info, "console") == 0 &&strcmp(linha->prox->info, ".log") == 0)
        linha = linha->prox->prox;

    //pula (
    if(linha != NULL && strcmp(linha->info, "(") == 0)
        linha = linha->prox;

    //pula espacos
    if(linha != NULL)
        pulaEspacos(&linha);

	printf("\n4- Estou antes do if de pulas aspas (SEPARA EXPRESSOES)");
    //pula ' ou "
    if(linha != NULL && (strcmp(linha->info, "'") == 0 || strcmp(linha->info, "\"") == 0))
    {
        linha = linha->prox;
        if(linha != NULL)
            pulaEspacos(&linha);

        while(linha != NULL && strcmp(linha->info, "'") != 0 && strcmp(linha->info, "\"") != 0)
            linha = linha->prox;

        if(linha != NULL)
            linha = linha->prox;
        printf("\n5- Estou saindo do if de pulas aspas (SEPARA EXPRESSOES)");
    }

    if(linha != NULL)
        pulaEspacos(&linha);

    // Pulando at� encontrar ,
    while(linha != NULL && strcmp(linha->info, ",") != 0)
        linha = linha->prox;

    if(linha != NULL)
        strncpy(div, linha->info, sizeof(div) - 1), div[sizeof(div) - 1] = '\0';

    atr = linha;

    if(linha != NULL)
        linha = linha->prox;
        
	printf("\n6- Estou antes do while principal (SEPARA EXPRESSOES)");
    //loop principal
    while(linha != NULL && linha->prox != NULL && strcmp(linha->prox->info, ")") != 0)
    {
        if(linha != NULL)
            pulaEspacos(&linha);
		
		printf("\n7- Estou dentro do while e antes do if que compara aspas (SEPARA EXPRESSOES)");
        if(strcmp(linha->info, "'") != 0 && strcmp(linha->info, "\"") != 0)
        {
            if(strcmp(linha->info, ",") == 0)
            {
                if(linha != NULL)
                    pulaEspacos(&linha);

                novaP = CaixaPrograma();
                novaP->token = c;
                cabeca = novaP;
                c = NULL;
				
				printf("\n8- Estou antes da chamada da funcao resolConLog (SEPARA EXPRESSOES)");
                token = resolConLog(cabeca, *pv, funcoes);
                if(token != NULL && atr != NULL)
                {
                    atr->prox = token;
                    token->prox = linha;
                }
                printf("\n9- Estou apos a chamada da funcao resolConLog (SEPARA EXPRESSOES)");

                if(linha != NULL)
                    strcpy(div, linha->info);

                atr = linha;
            }
            else
            {
            	printf("\n10- Estou no else de que se for diferente de virgula (SEPARA EXPRESSOES)");
				AdicionarTokenLinha(&c,linha->info);
            }
        }
        else // strings
		{
		    printf("\n11- Estou no else de pulas aspas (SEPARA EXPRESSOES)");
		    buffer[0] = '\0'; //zera o buffer
		
		    //guarda o tipo de aspa
		    aspa = linha->info[0];
		    linha = linha->prox;
		
		    //junta tudo ate achar a aspa final
		    while(linha != NULL && !(linha->info[0] == aspa && linha->info[1] == '\0'))
		    {
		        if(strlen(buffer) > 0)
		            strcat(buffer, " "); //adiciona espaco entre tokens
		        strcat(buffer, linha->info);
		        linha = linha->prox;
		    }
		
		    //pula a aspa final
		    if(linha != NULL)
		        linha = linha->prox;
		
		    //cria um unico token com a string completa
		    AdicionarTokenLinha(&c, buffer);
		
		    atr = linha;
		}

        if(linha != NULL)
            linha = linha->prox;
    }
    
    printf("\n16- Antes do ultimo if (SEPARA EXPRESSOES)");

    //ultimo token antes do )
    if(linha != NULL && strcmp(linha->info, ")") == 0)
    {
    	printf("\n17- Dentro do ultimo if antes do ')' (SEPARA EXPRESSOES)");
        novaP = CaixaPrograma();
        novaP->token = c;

        cabeca = novaP;
        token = resolConLog(cabeca, *pv, funcoes);
        if(token != NULL && atr != NULL)
        {
            atr->prox = token;
            token->prox = linha;
        }
    }

	printf("\n18- antes do return (SEPARA EXPRESSOES)");
    return atual;
}

//funcao que trata o console.log
void tratarConLog(Programa *programa, Variavel *pv, char *mensagemPronta)
{
    strcpy(mensagemPronta, "");

    Token *atual = programa->token;
    Variavel *pvAux = NULL;
    char nomeVar[100];

    int flag = 1, length; // flag para controlar o loop

    //pula tabs ou espa�os iniciais
    while(atual != NULL && strcmp(atual->info, "\t") == 0)
        atual = atual->prox;

    //verifica se e console.log
    if(atual != NULL && atual->prox != NULL && strcmp(atual->info, "console") == 0 && strcmp(atual->prox->info, ".log") == 0)
    {
        atual = atual->prox->prox; // pular console e .log

        //pular '('
        if(atual != NULL && strcmp(atual->info, "(") == 0)
            atual = atual->prox;

        //loop principal
        while(flag && atual != NULL)
        {
            //pular espacos e tabs
            while(atual != NULL && (strcmp(atual->info, " ") == 0 || strcmp(atual->info, "\t") == 0))
                atual = atual->prox;

            //checa se chegou no final dos argumentos
            if(atual != NULL && strcmp(atual->info, ")") == 0)
                flag = 0;

            if(atual != NULL && flag)
            {
                // >>> ignorar virgula ou +
                if(strcmp(atual->info, ",") == 0 || strcmp(atual->info, "+") == 0)
                {
                    atual = atual->prox; // apenas pula
                }
                else
                {
                    // string literal
                    if(strcmp(atual->info, "\"") == 0 || strcmp(atual->info, "'") == 0)
                    {
                        atual = atual->prox;
                    }
                    else
                    {
                        //numero ou literal direto
                        if(isdigit(atual->info[0]) || atual->info[0] == '-' || atual->info[0] == '.')
                        {
                            strcat(mensagemPronta, atual->info);
                        }
                        else //variavel ou expressao
                        {
                            strcpy(nomeVar, atual->info);
                            pvAux = buscarIdentPV(pv, nomeVar);
                            if(pvAux != NULL)
                                strcat(mensagemPronta, pvAux->valor);
                            else
                                strcat(mensagemPronta, nomeVar);
                        }
                        atual = atual->prox;
                    }

                    // adiciona espa�o se o pr�ximo token n�o for ')'
                    if(atual != NULL && strcmp(atual->info, ")") != 0)
                        strcat(mensagemPronta, " ");
                }
            }
        }
        length = strlen(mensagemPronta);
		if(length > 0 && mensagemPronta[length-1] != '"' && mensagemPronta[length-1] != '\'')
		{
		    mensagemPronta[length] = '"';
		    mensagemPronta[length+1] = '\0';
		}
    }
}

//funcao que busca function pelo nome e retorna o endereco dela
Programa *buscaFuncoes(Funcoes *funcoes, char *token)
{
	while(funcoes != NULL && strcmp(funcoes->function,token) != 0)
		funcoes = funcoes->prox;
		
	if(funcoes != NULL && strcmp(funcoes->function,token) == 0)
		return funcoes->local;
	else
		return NULL;
}

char isVariavel(char *aux, Variavel *P)
{
	while(P != NULL && strcmp(aux,P->identificador)!=0)
		P = P->prox;
	if(P != NULL)
		return 1;
	return 0;
}

//algoritmo que executa uma funcao
//int executaFuncao(Funcoes *funcoes, Programa *programa, Token *token, Variavel *pv)
//{
//	int chave=0;
//	Variavel *auxVariavel;
//	
//	
//	if(token != NULL && strcmp(token->info,"{") == 0)
//	{
//		chave++;
//		programa = program->prox;
//	}
//	
//	if(chave > 0) //chave maior que 0 indica que ainda est� na function
//	{
//		token = programa->token;
//		if(strcmp(token->info,"{") == 0)
//			chave++;
//		else
//		if(strcmp(token->info,"}") == 0)
//			chave--;
//		else
//			executaPrograma(programa, &pv, funcoes);
//							
//		programa = programa->prox;
//	}
//}

//CAIO - ESSA FUNCAO SERIA A EXECUCAO DO PROGRAMA EM SI, FEITA APENAS A DECLARACAO DE VARIAVEL
void executaPrograma(Programa *programa, Variavel **pv, Funcoes *funcoes)
{
	Variavel auxVar, *auxPilha;
	//Variavel *pv;
	//initPV(&pv);
	Token *auxToken, *auxProcura, *linhaAux, *atualToken;
	Programa *auxPrograma, *pontConLog, *auxLocalFun, *atualProgm; //pontConLog = endere�o de onde tem um console.log
	auxPrograma = programa;
	
	//lista encadeada que vai guardar as informa��es dos console.log
	listaEncadeada *listaConLog;
	initLE(&listaConLog);
	
	//Lista Generalizada para calcular express�es matem�ticas
	ListaGen *listaCalcula;
	initLG(&listaCalcula);
	
	//Salvar o tipo de variavel quando declarada
	char auxTipo[7], mensagemPronta[200];
	
	int chave=0, flagFun = 0; //contator de chaver para saber quando sair de uma fun��o
	
	while(auxPrograma != NULL)
	{
		auxToken = auxPrograma->token;
		while(auxToken != NULL)
		{
			//funcao que busca uma funcao e retorna o local dela
			auxLocalFun = buscaFuncoes(funcoes, auxToken->info);
			//printf("\nLocal da function OLA: %p",auxLocalFun); // PARA TESTE
			
			//ESTA COM ERRO NA COMPARACAO
			if(isTipoVariavel(auxToken->info) == 1 || isTipoVariavel(auxToken->info) == 0) //verifica se o token � DECLARA��O de variavel LET ou CONST
			{
				strcpy(auxTipo, auxToken->info); // Salvar o tipo da variavel para posteriormente validar e tratar de forma adequada cada tipo
				auxToken = auxToken->prox;
				strcpy(auxVar.identificador, auxToken->info); //Atribui o nome da variavel que SEMPRE estara na proxima caixa. Ou seja sempre ser�: <<tipo>> nome =
				auxToken = auxToken->prox->prox; //Pula o "=" pq SEMPRE ser� '=' apos nome de variavel
				auxProcura = auxToken;
					
				//se tiver um operador(+ - * /) apartir do '=' significa que � uma EXPRESS�O/CONTA.
				if(procuraOperador(auxProcura))
				{	
					constroiLG(&listaCalcula, auxToken);//preciso construir a listagen a partir do token
					// Converte o float para string usando sprintf()
    				sprintf(auxVar.valor, "%2.f", calculaEquacao(listaCalcula));
				}
				//else if(procuraFuncao(auxProcura))
				//{
						
					//}
					//caso n�o seja fun��o ou conta;
				else
				{
					strcpy(auxVar.valor, auxToken->info);
					auxToken = auxToken->prox;
				}
					
				auxVar.ponteiro = auxPrograma; //IMPLEMENTAR LOGICA DE PONTEIRO!!!!!!!!
						
				if(strcmp(auxTipo,"let")==0)
					auxVar.tipo = 0;
				else //Entao e CONST
					auxVar.tipo = 1;
	
				pushPV(pv,auxVar); //Passar a pilha, e a variavel
				//}
			}
		 	else
			if(auxToken->prox != NULL && linhaAux->prox && strcmp(auxToken->info,"console") == 0 && strcmp(auxToken->prox->info,".log") == 0) 
			{
				pontConLog = separaExpressoes(auxPrograma, &*pv, funcoes);
				
				linhaAux = pontConLog->token;
				while(linhaAux != NULL && strcmp(linhaAux->info,"\t") == 0 )
					linhaAux = linhaAux->prox;
					
				if(linhaAux != NULL && linhaAux->prox != NULL)
				{
					tratarConLog(pontConLog, *pv, mensagemPronta);
					enqueueLE(&listaConLog, mensagemPronta);
					
					printf("\n\nMensagem console.log: %s",mensagemPronta);
					getch();
				}
				pontConLog = NULL;	
			} //n�o pode ter else aqui!!!
			if(auxLocalFun != NULL || flagFun == 1) //achou a funcao
			{
				flagFun = 1;//indica que eu estou na fun��o
				//guardando a caixinha de programa e de token que teve a chamada da funcao
				atualProgm = auxPrograma;
				atualToken = auxToken;
				
				auxPrograma = auxLocalFun; // posicionando onde esta a function
				//printf("\nLocal da function OLA: %p",auxPrograma); //PARA TESTE
				auxPrograma = auxPrograma->prox; //esta na chave
				auxToken = auxPrograma->token;
				
				if(auxToken != NULL && strcmp(auxToken->info,"{") == 0)
				{
					chave++;
					auxPrograma = auxPrograma->prox;
				}
				
				if(chave > 0) //chave maior que 0 indica que ainda est� na function
				{
					auxToken = auxPrograma->token;
					if(strcmp(auxToken->info,"{") == 0)
						chave++;
					else
					if(strcmp(auxToken->info,"}") == 0)
						chave--;
										
					//auxPrograma = auxPrograma->prox; //nao pode fazer isso
				}
				
				if(chave == 0) //terminou a funcao entao volta para o chamado
				{
					flagFun=0;
					auxPrograma = atualProgm;
					auxToken
					 = atualToken;
				}
			}
			else if(isVariavel(auxToken->info), *pv) //Busca na pilha para verificar se o token � uma variavel
			{
				auxPilha = buscaVariavel(auxToken->info, *pv);
				
				if(numeric(auxPilha->valor[0])) //Variavel � numero, "Int".
				{
						auxToken = auxToken->prox->prox; //Pula o '='
						
					//	if(procuraFuncao())// Procura fun��o nos proximos tokens
					//	{
							
					//	}
					/*	else*/ if(procuraOperador(auxToken))//Procura operador matematico para ver se � express�o matematica
						{
							constroiLG(&listaCalcula, auxToken);//preciso construir a listagen a partir do token
							// Converte o float para string usando sprintf()
		    				sprintf(auxPilha->valor, "%2.f", calculaEquacao(listaCalcula));
						//	alteraValor(auxVariavel, *pv); //Atribuir novo valor a variavel
						}
						else if(numeric(auxToken->info[0]))//SE N�o � fun��o e nem express�o ent�o � apenas uma mudan�a de valor
						{
							auxPilha->valor = atoi(auxToken);
							//alteraValor(auxVariavel, *pv); //Atribuir novo valor a variavel //NAO SEI SE PRECISA
						}
						
				}
		//		else if(array(auxVariavel))
		//		{
		//			auxToken = auxToken->prox;
		//			if(strcmp(auxToken->info,". ))
		//		}
		//		else //Se n�o � Int e nem Array, ent�o � string
		//		{
		//			
		//		}
		//		auxToken = auxToken->prox;
				
			}
			auxToken = auxToken->prox;
		}
		auxPrograma = auxPrograma->prox;
	} 
}

//funcao que le um arquivo que foi passado e armazena os tokens linha a linha
void lerArquivo(char *nomeArquivo, Programa **programa)
{
    FILE *ptr;
    char linha[256];
    Programa *novaLinha;
    char token[256];
    int i, j, contConchete=0;

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
	            //pula espacos, tabs e quebras de linha
	            while(linha[i] == ' ' || linha[i] == '\t' || linha[i] == '\n')
	                i++;
	
	            j = 0;
	
	            //verifica se e um operador ou simbolo isolado
	            if(linha[i] == '.' && !(linha[i+1] >= '0' && linha[i+1] <= '9'))
				{
				    j = 0;
				    token[j++] = linha[i++]; // guarda o ponto
				
				    // guarda at� encontrar '(' ou algum delimitador
				    while(linha[i] != '(' && identificador(linha[i]) == 1)
				        token[j++] = linha[i++];
				
				    token[j] = '\0';
				    AdicionarToken(novaLinha, token);
				}
				else
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
				if(linha[i] == '[')//Verifica se � um Array
				{
					contConchete++;
					token[j++] = linha[i++];
					while(contConchete != 0)
					{
						if(linha[i] == '[')
							contConchete++;
						if(linha[i] == ']')
							contConchete--;
						token[j++] = linha[i++];
					}
					token[j] = '\0';
					AdicionarToken(novaLinha, token);
				}
				else
				{
	                //le identificadores, numeros, etc.
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

//funcao que exibe o programa
void ExibirPrograma(Programa *programa) 
{
    Programa *linhaAtual = programa;
    Token *tokenAtual = linhaAtual->token;
    int numLinha = 1, contToken=1;

    while(linhaAtual != NULL)
	{
        while(tokenAtual != NULL)
		{
            printf("Token: %d - info: %s \t ",contToken, tokenAtual->info);
            tokenAtual = tokenAtual->prox;
            contToken++;
        }
        printf("\n");
        //printf("\n");
        linhaAtual = linhaAtual->prox;
        tokenAtual = linhaAtual->token;
        numLinha++;
    }
}

//funcao que exibe as variaveis e informacoes dentro da memoria ram
void ram(Variavel *pv)
{
	//se caso for colocar moldura depois
	//limpaTela(1, 1, 90, 90);
	system("cls");
	//gotoxy(1,1);
	
	printf("\n\t\t----------- MEMORIA RAM ----------\n");
	printf("\n| %-*s | %-*s | %-*s |",20,"Identificador",15,"Valor",20,"Ponteiro");
	//printf("\n|Identificador \t|\t Valor \t|\t Ponteiro|");
	while(pv != NULL)
	{
		printf("\n| %-*s | %-*s | %*p |",20,pv->identificador, 15,pv->valor, 20,pv->ponteiro);
		//printf("|%s \t|\t %s\t|\t %p \t|",pv->identificador,pv->valor, pv->ponteiro);
		pv = pv->prox;
	}
}

//funcao que contem o menu do nosso simulador de execucao
char menu()
{
	printf("\n\n[F7] - Abrir \t"); //F7 = 65
	printf("[F8] - Executar \t"); //F8 = 66
	printf("[F9] - Memoria RAM \t"); //F9 = 67
	printf("[F10] - Tela\t"); //F10 = 68
	printf("\n[ESC] - Sair"); // ESC = 27
	return getch();
}

// ------------------//-----------------------//--------------------//------------//---------
//FUNCOES QUE AINDA FALTA CRIAR
// ------------------//-----------------------//--------------------//------------//---------

//void limpaTela(int lin1, int lin2, int col1, int col2)

//TENHO QUE TERMINAR ESSA FUN��ES PARA EXIBIR LINHA A LINHA DO PROGRAMA
//fun��o que exibe o codigo linha a linha conforme for dando enter
//void exibirExecucao(Programa *programa)
//{
//	Token *linha;
//	
//	while(pProg != NULL)
//	{
//		linha = programa->token;
//		while(linha != NULL)
//		{
//			printf("%s",linha->token);
//			linha=linha->prox;
//		}
//		printf("\n");
//		pProg = pProg->prox;
//	}
//}

////fun��o que exibe o codigo linha a linha conforme for dando enter
//void mostrarLinha(Programa *programa)
//{
//	Token *token = programa->token;
//	
//	while(token != NULL)
//	{
//		printf("%s",token->token);
//		token = token->prox;
//	}
//	
//	printf("\n");
//	Mostrar_Excucao(atual->prox);
//}

//void exibirFunction(Funcoes *f)

//void mostrarLinha(Programa *p, int lin)

//void posicionaCursor(Programa *p, int lin, int pos)



//funcao que simula a execucao do nosso programa (FALTA FINALIZAR)
void simulaExecucao(Programa **programa, Variavel **pv)
{
	//vai ser usado para o controle de toda a estrutura do programa que esta sendo compilado
	Controle *se, *Rep, *seAux, *repAux, *ifAux, *aux; //FALTA CRIAR -- feito e comentado
	int chaveAtual=0, lin=0, col=0, chave=0, flag=0, l=0, chaveFun=0, funL=0, cont=0;
	
	Programa *atual= NULL, *auxP = NULL;
	//Programa *listaPrograma, *fun=NULL, *atr = NULL, *numUse=NULL, *print=NULL, *auxAtual;
	
	//ponteiro que vamos usar para ler os tokens e andar no codigo .js
	Variavel *pvAux = *pv;
	Token *linha, *nova, *linha2;
	Funcoes *funcoes, *funAux;
	
	initC(&se); 
	initC(&Rep);
	
	char seVar='0', rep='0', op, nomeArquivo[50];
	
	//vai ser utilizado para o controle do for presente no programa .js
	Variavel *var;
	char *variavel;
	char valorStr[30]; //receber a string que foi passada
	int start, stop, step; //valores que vao dentro do for
	controleFor *pFor = NULL;
	
	listaEncadeada *listaPrint, *auxLE; 
	initLE(&listaPrint); 
	
	//limpaTela(1, 1, 90, 90); //FALTA CRIAR -- feito escopo e comentado
	//gotoxy(1, 1);
	
	//chamando o menu de opcoes do programa
	op = menu();
	
	//inicia Fila de fun��es
	initF(&funcoes);
	
	//repeticao que vai simular a execucao do programa
	while(op != 27)
	{
		switch(op)
		{
			case 65: //F7 - Abrir arquivo .js
				printf("\nNome do arquivo: ");
				fflush(stdin);
				gets(nomeArquivo);
				
				lerArquivo(nomeArquivo, &*programa);
				
				auxP = *programa;
				//ExibirPrograma(auxP); //PARA TESTE
				op = getch();
				//printf("\nLocal da function OLA: %p",auxP); //PARA TESTE
				getch();

				chave = 1;
				while(auxP != NULL && chave > 0)
				{
					chave = 0;
					linha = auxP->token;
					if(linha != NULL && strcmp(linha->info,"function") == 0) //achou uma fun��o
					{
						linha = linha->prox; //esta no nome da function
						enqueueF(&funcoes, linha->info, l, auxP);
						l++;
						
						auxP = auxP->prox;
						linha = auxP->token;
						
						if(linha != NULL && strcmp(linha->info,"{") == 0)
						{
							chave++;
							auxP = auxP->prox;
						}
						
						while(chave > 0) //chave maior que 0 indica que ainda est� na function
						{
							linha = auxP->token;
							if(strcmp(linha->info,"{") == 0)
								chave++;
							else
							if(strcmp(linha->info,"}") == 0)
								chave--;
							
							auxP = auxP->prox;
						}
						
						linha = auxP->token;
						if(linha != NULL && strcmp(linha->info,"function") == 0)
							chave = 1;
					}
				}
				atual = auxP;
				
				//exibeFuncoes(funcoes); //PARA TESTE

				break;
			
			case 66: //F8 - executar programa !!!! TENHO QUE CONTINUAR ANALISANDO A LOGICA AQUI !!!!!!
				//nao sei se estaria correto como estou passando
				executaPrograma(atual, &pvAux, funcoes);
				op = getch();
//				limpaTela(1, 1, 90, 90);
//				gotoxy(1,1);
//				
//				if(!isEmptyF(funcoes)) //FALTA CRIAR -- feito escopo e comentado
//					exibirFunction(funcoes); //FALTA CRIAR -- feito escopo e comentado
//				if(numUse == atual)
//				{
//					mostrarLinha(numUse,l); //FALTA CRIAR -- feito escopo e comentado
//					lin = l;
//					col = 1;
//				}
//				else
//					mostrarLinha(numUse,l);
//					
//				gotoxy(col,lin); //move o cursor para proxima linha
//				op = getch(); //ou talvez tenha que ser op = menu();
//				gotoxy(col,lin);
//				
//				//continua a execu��o enquanto clicar enter = 13
//				while(op == 13 && atual != NULL)
//				{
//					linha = atual->token;
//					chave = 0; //nosso cotador que vai indicar dentro de quantos bloco estamos
//					
//					if(strcmp(linha->info, "{") == 0) //inicio de um bloco de execu��o
//					{
//					    // apenas avan�a uma linha, n�o precisa de contador
//					    atual = atual->prox;
//					    chave++;
//					}
////					vamos validar se estava em uma repeti��o se sim vamos retornar para o inicio dela
////					e tambem validar se estava em um if para fazer chaveAtual receber chave
////					para continuar a execu��o
//					
//					flag = 0;
//					if(chave != chaveAtual)
//					{
//						if(chave > chaveAtual)
//						{
//							//codigo para avan�ar nas repeti��es ou condi��es
//							//se as chave aumentou significa que estamos entrando um novo bloco while ou if
//							//se valida =1 vamos pular
//							
//							auxP = atual;
//							l = lin;
//							
//							while(auxP != NULL && chave > chaveAtual)
//							{
//							    linha = auxP->token;
//							    chave = 0;
//							
//							    if(strcmp(linha->info,"{") == 0) //se eu achei uma chave eu dou ++
//							        chave++;
//							
//							    if(chave > chaveAtual) 
//								{
//							        l++;
//							        auxP = auxP->prox;
//							    }
//							}
//							
//							//atualizando a linha atual e o contador de linhas
//							if(auxP != NULL)
//							{
//								linha = auxP->token;
//								atual = auxP;
//								lin = l
//							}
//							else
//							{
//								flag = 1;
//								atual = auxP
//							}
//						}
//						if(chave < chaveAtual)
//						{
//							if(!isEmpty(rep))
//							{
//								topC(rep,&repAux);
//								auxP = repAux->local;
//								if(repAux->chave >= chave)
//								{
//									atual = repAux->local;
//									linha = atual->token;
//									chave = 0;
//									if(strcmp(linha->info,"}") == 0)
//									{
//										chave++;
//										atual = atual->prox;
//									}
//										
//									linha = atual;
//									chaveAtual = repAux->chave;
//									lin = repAux->l;
//									col = repAux->c;
//									repAux = popC(&rep);
//								}
//							}
//							if(!isEmptyC(se))
//							{
//								chaveAtual = chave;
//								ifAux = pop(&se);
//								seVar = 1;
//							}
//						}
//					}
//				}
				
				break;
				
			case 67: // F9 - memoria ram
			//exibe o estado da pilha de variavel simulando a memoria ram
				ram(pvAux);
				op = getch();
				
				break;
				
			case 68: //F10 - exibir os prints
				auxLE = listaPrint;
				//limpaTela(1, 1, 90, 90);
				//gotoxy(1,1);
//				printf("\n *** Prints ***\n");
//				while(auxLE != NULL)
//				{
//					printf("\n%s",auxLE->info);
//					auxLE = auxLE->prox;
//				}

				ExibirPrograma(*programa);
				op = getch();
				
				break;
			
			case 27:
				printf("\nPrograma encerrado!");
				
				break;
		}
		
		//chamando o menu de opcoes do programa
		op = menu();
	}
}

int main()
{
	//lista que vai apontar para a lista de tokens
    Programa *l = NULL; //vou passa l como null, porque vai ler o arquivo dentro da simulacao
    
    //lista do tipo pilha que vai guardar todas as informacoes das nossas variaveis
    Variavel *pilhaVar = NULL;

	//Chamando a funcao que vai executar todo nosso programa
	simulaExecucao(&l, &pilhaVar);
    
    return 0;
}
