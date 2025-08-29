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
typedef struct TpVariavel Variavel;

//estrutura que vai conter as informa��es para o controle de exibi��o do nosso codigo
struct TpControle
{
	int chave, l, c;
	Programa *local;
	struct TpControle *prox;
};
typedef struct TpControle Controle;

//estrutura que vai conter as informa��es das functions do nosso programa
struct TpFuncoes
{
	int l;
	Programa *local;
	char function[TF];
	struct TpFuncoes
};
typedef struct TpFuncoes Funcoes;

//estrutura que vai controlar a manipula��o de bloco for no nosso programa
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


//CAIO - POSICIONAR JUNTO COM FUN��ES PARECIDAS
int isTipoVariavel(char *info) 
{
	return !strcmp(info, "LET") || !strcmp(info, "CONST");
}

//CAIO - ESSA FUNC��O SERIA A EXECU��O DO PROGRAMA EM SI, FEITA APENAS A DECLARA��O DE VARIAVEL
void executaPrograma(Programa *programa)
{
	Variavel auxVar;
	Pilha *pv;
	initPV(&pv);
	TpToken *auxToken;
	auxToken = programa->token;
	Programa *auxPrograma;
	auxPrograma = programa;
	
	char auxTipo[7]; //Salvar o tipo de vari�vel quando declarada
	
	while(auxPrograma != NULL)
	{
		while(auxToken != NULL)
		{
			if(isTipoVariavel(auxToken->info)) //verifica se o token � defini��o de vari�vel LET ou CONST
			{
					strcpy(auxTipo, auxToken->info); // Salvar o tipo da v�riavel para tratar da forma adequada
					auxToken = auxToken->prox;
					strcpy(auxVar.identificador, auxToken->info); //Atribui o nome da variavel que SEMPRE estar� na proxima caixa. Ou seja sempre ser�: <<tipo>> nome =
					auxToken = auxToken->prox->prox; //Pula o "=" pq SEMPRE ser� '=' ap�s declara��o de variavel
					if(strcmp(auxTipo,"LET")==0)
					{
						strcpy(auxVar.valor, auxToken->info);
						auxToken = auxToken->prox;
						auxVar.ponteiro = NULL; //ARRUMAR ISSO DPS	
					}
					else //Ent�o � CONST
					{
						strcpy(auxVar.valor, auxToken->info);
						auxToken = auxToken->prox;
						auxVar.ponteiro = auxPrograma; //IMPLEMENTAR LOGICA DE PONTEIRO!!!!!!!!
					}
					pushPV(&pv,auxVar); //Passar a pilha, e a variavel
			}
		}
	} 
	
}

// -- TAD PILHA CONTROLE --

//fun��o que vai inicializar nossa pilha de controle
void initC(Controle **c)
{
	*c = NULL;
}

//fun��o que verifica se nossa pilha de controle est� vazia
char isEmptyC(Controle *c)
{
	return c == NULL
}

//fun��o que insere informa��es na nossa pilha de controle
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

//fun��o que vai retirar as informa��es da nossa pilha de controle
Controle popC(Controle **c)
{
	Cotrole *aux, cAtual;
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

//fun��o que envia o ultimo elemento da pilha sem modificar ela
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

//fun��o que inicializa a fila de LE
void initLE(listaEncadeada **le)
{
	*le = NULL;
}

//fun��o que aloca uma nova caixinha para fila de LE
listaEncadeada *criaCaixaLE(char *info)
{
	listaEncadeada *nova = (listaEncadeada*)malloc(sizeof(listaEncadeada));
	strcpy(nova->info,info);
	nova->prox = NULL;
	return nova;
}

//fun��o que insere informa��o na fila de LE
void enqueueLE(listaEncadeada **le, char *info)
{
	listaEncadeada *aux = *le, *nova = criaCaixaLE(info);
	
	if(aux == NULL)
		*lista = nova;
	else
	{
		while(aux->prox != NULL)
			aux = aux->prox;
			
		aux->prox = nova;
	}
}

//fun��o que retira uma informa��o da fila de LE
void dequeueLE(listaEncadeada **le, char *info)
{}
	listaEncadeada *aux;
	
	if(*le == NULL)
		strcpy(info, "");
	else
	{
		strcpy(info, (*le)->info);
		aux = *le;
		*le = (*le)->prox;
		free(aux);
	}
}

//fun��o que exibe o conteudo da fila LE
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

//void initF(Funcoes **f)

//char isEmptyF(Funcoes *f)


// -- TAD PILHA VARIAVEL --

//fun��o que inicializa a pilha da variavel
void initPV(Variavel **pilhaVar)
{
	*pilhaVar = NULL;
}

//fun��o que verifica se a pilha de variavel � nula ou vazia
char isEmptyPV(Variavel *pilhaVar)
{
	return pilhaVar == NULL;
}

//fun��o que insere a variavel dentro da nossa pilha de variavel
void pushPV(Variavel **pilhaVar, Variavel auxVar)
{
	Variavel *nova = (Variavel*)malloc(sizeof(Variavel));
	strcpy(nova->valor, auxVar.valor);
	strcpy(nova->identificador, auxVar.identificador);
	nova->ponteiro = programa;
	
	nova->prox = *pilhaVar;
	*pilhaVar = nova;
}

//fun��o que vai retirar uma variavel da nossa pilha de variavel
void popPV(Variavel **pv, Variavel **pvAux)
{
	if(!isEmptyPV(*pv))
	{
		*pvAux = (Variavel*)malloc(sizeof(Variavel));
		Variavel *aux; //vai ser usado para deletar a caixinha
		
		//fazendo o auxiliar que vem por parametro receber as informa��es
		strcpy((*pvAux)->valor, (*pv)->valor);
		strcpy((*pvAux)->identificador, (*pv)->identificador);
		(*pvAux)->ponteiro = (*pv)->ponteiro;
		(*pvAux)->prox = NULL;
		
		aux = *pv;
		*pv = (*pv)->prox;
		free(aux);
	}
}

//fun��o que vai tirar uma variavel da pilha pelo identificador (indepentemente se � topo ou n�o)
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
			atual = NULL; //para encerrar a repeti��o
		}
		else
		{
			ant = atual;
			atual = atual->prox;
		}
	}
}

//fun��o que vai retornar o topo da nossa pilha de variavel
void topPV(Variavel *pv, Variavel **pvAux)
{
	*pvAux = pv;
}

//fun��o que busca uma variavel na pilha de variavel pelo identificador
Variavel *buscarIdentPV(Variavel *pv, char *identificador)
{
	while(pv != NULL && strcmp(pv->identificador, identificador) != 0)
		pv = pv->prox;
		
	if(pv != NULL)
		return pv;
	return NULL; //n�o achou
}

//fun��o que busca um token dentro da lista
Programa *buscarToken(Programa *programa, char *tokenBusca)
{
	Token *token;
	
	while(programa != NULL)
	{
		token = programa->token;
		while(token != NULL)
		{
			if(strcmp(token->info, tokenBusca) == 0)
				return programa; // lista do programa auq tem o token dentro
			token = token->prox;
		}
		programa = programa->prox;
	}
	return NULL; //n�o achou o token
}



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

//fun��o que exibe as variaveis e informa��es dentro da memoria ram
void ram(Variavel *pv)
{
	//se caso for colocar moldura depois
	limpaTela(1, 1, 90, 90);
	gotoxy(1,1);
	
	printf("\n ----------- MEMORIA RAM ----------");
	printf("\n| %-*s | %-*s | %-*s |",20,"Identificador",15,"Valor",20,"Ponteiro");
	//printf("\n|Identificador \t|\t Valor \t|\t Ponteiro|");
	while(pv != NULL)
	{
		printf("\n| %-*s | %-*s | %*p |",20,pv->identificador, 15,pv->valor, 20,pv->ponteiro);
		//printf("|%s \t|\t %s\t|\t %p \t|",pv->identificador,pv->valor, pv->ponteiro);
		pv = pv->prox;
	}
}

//fun��o que contem o menu do nosso simulador de execu��o
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
//FUN��ES QUE AINDA FALTA CRIAR
// ------------------//-----------------------//--------------------//------------//---------

//void limpaTela(int lin1, int lin2, int col1, int col2)



//void exibirFunction(Funcoes *f)

//void mostrarLinha(Programa *p, int lin)

//void posicionaCursor(Programa *p, int lin, int pos)



//fun��o que simula a execu��o do nosso programa (FALTA FINALIZAR)
void simulaExecucao(Programa **programa, Variavel **pv)
{
	//(FALTA CRIAR) vai ser usado para o controle de toda a estrutura do programa que est� sendo compilado
	Controle *se, *rep, *seAux, *repAux, *ifAux, *aux; //FALTA CRIAR -- feito e comentado
	int chaveAtual=0, lin=0, col=0, chave=0, flag=0, l=0, chaveFun=0, funL=0, cont=0;
	
	Programa *atual=*programa, *listaPrograma, *auxP, *fun=NULL, *atr = NULL, *numUse=NULL, *print=NULL, *auxAtual;
	
	//ponteiro que vamos usar para ler os tokens e andar no codigo .js
	Variavel *pAux;
	Token *linha, *nova, *linha2;
	Funcoes *funcoes, *funAux; //FALTA CRIAR -- feito e comentado
	
	initC(&se); //FALTA CRIAR -- feito escopo e comentado
	initC(&rep); //FALTA CRIAR -- feito escopo e comentado
	
	char op, seVar='0', rep='0', nomeArquivo[50];
	
	//vai ser utilizado para o controle do for presente no programa .js
	Variavel *var;
	char *variavel;
	char valorStr[30]; //receber a string que foi passada
	int start, stop, step; //valores que v�o dentro do for
	controleFor *pFor = NULL; //FALTA CRIAR -- feito e comentado
	
	listaEncadeada *listaPrint; //FALTA CRIAR -- feito e comentado
	initLE(&listaPrint); //FALTA CRIAR -- feito escopo e comentado
	
	limpaTela(1, 1, 90, 90); //FALTA CRIAR -- feito escopo e comentado
	gotoxy(1, 1);
	
	//chamando o menu de opc�es do programa
	op = menu();
	
	auxP = atual;
	initF(&funcao); //FALTA CRIAR -- feito escopo e comentado
	
	//repeti��o que vai simular a execu��o do programa
	while(op != 27)
	{
		switch(op)
		{
			case 65: //F7 - Abrir arquivo .js
				//essa parte aqui era do main
				printf("\nNome do arquivo: ");
				fflush(stdin);
				gets(nomeArquivo);
				
				lerArquivo(nomeArquivo, &*programa);
				
				while(auxP != NULL)
				{
					linha = auxP->token; //recebe a primeira linha
					
					if(strcmp(linha->info, "function") == 0) //verificando se achou uma funcao
					{
						//vamos inserir a function
						enqueue(&funcoes,linha->info,l,auxP);
						l++;
						
						auxP = auxP->prox;
						if(auxP != NULL && auxP->token != NULL && strcmp(auxP->token->info, "{") == 0)
						{
							auxP = auxP->prox;
							chave++;
						}
							
						if(auxP != NULL)
							linha = auxP->token;
						
						while(auxP != NULL && auxP->token != NULL && chave > 0)
				        {
				        	if(strcmp(linha->info,"{") == 0)
				        		chave++;
				        	else
				        	if(strcmp(linha->info,"}") == 0)
				        		chave--;
				        	
				            auxP = auxP->prox;
				            if(auxP != NULL)
				            {
				                linha = auxP->token;
				                l++;
				            }
				        }
				
				        if(auxP != NULL)
				        	atual = auxP->prox;
					}
					else //andar se caso n�o achou um function
            			auxP = auxP->prox;
				}
				auxP = atual;
				numUse = auxP;
				
				break;
			
			case 66: //F8 - executar programa !!!! TENHO QUE CONTINUAR ANALISANDO A LOGICA AQUI !!!!!!
				limpaTela(1, 1, 90, 90);
				gotoxy(1,1);
				
				if(!isEmptyF(funcoes)) //FALTA CRIAR -- feito escopo e comentado
					exibirFunction(funcoes); //FALTA CRIAR -- feito escopo e comentado
				if(numUse == atual)
				{
					mostrarLinha(numUse,l); //FALTA CRIAR -- feito escopo e comentado
					lin = l;
					col = 1;
				}
				else
					mostrarLinha(numUse,l);
					
				gotoxy(col,lin); //move o cursor para proxima linha
				op = getch(); //ou talvez tenha que ser op = menu();
				gotoxy(col,lin);
				
				//continua a execu��o enquanto clicar enter = 13
				while(op == 13 && atual != NULL)
				{
					linha = atual->token;
					chave = 0; //nosso cotador que vai indicar dentro de quantos bloco estamos
					
					if(strcmp(linha->info, "{") == 0) //inicio de um bloco de execu��o
					{
					    // apenas avan�a uma linha, n�o precisa de contador
					    atual = atual->prox;
					    chave++;
					}
//					vamos validar se estava em uma repeti��o se sim vamos retornar para o inicio dela
//					e tambem validar se estava em um if para fazer chaveAtual receber chave
//					para continuar a execu��o
					
					flag = 0;
					if(chave != chaveAtual)
					{
						if(chave > chaveAtual)
						{
							//codigo para avan�ar nas repeti��es ou condi��es
							//se as chave aumentou significa que estamos entrando um novo bloco while ou if
							//se valida =1 vamos pular
							
							auxP = atual;
							l = lin;
							
							while(auxP != NULL && chave > chaveAtual)
							{
							    linha = auxP->token;
							    chave = 0;
							
							    if(strcmp(linha->info,"{") == 0) //se eu achei uma chave eu dou ++
							        chave++;
							
							    if(chave > chaveAtual) 
								{
							        l++;
							        auxP = auxP->prox;
							    }
							}
							
							//atualizando a linha atual e o contador de linhas
							if(auxP != NULL)
							{
								linha = auxP->token;
								atual = auxP;
								lin = l
							}
							else
							{
								flag = 1;
								atual = auxP
							}
						}
						if(chave < chaveAtual)
						{
							if(!isEmpty(rep))
							{
								topC(rep,&repAux);
								auxP = repAux->local;
								if(repAux->chave >= chave)
								{
									atual = repAux->local;
									linha = atual->token;
									chave = 0;
									if(strcmp(linha->info,"}") == 0)
									{
										chave++;
										atual = atual->prox;
									}
										
									linha = atual;
									chaveAtual = repAux->chave;
									lin = repAux->l;
									col = repAux->c;
									repAux = popC(&rep);
								}
							}
							if(!isEmptyC(se))
							{
								chaveAtual = chave;
								ifAux = pop(&se);
								seVar = 1;
							}
						}
					}
				}
				
				break;
				
			case 67: // F9 - memoria ram
				//temos que criar ainda a pilha que vai simular a memoria ram em execu��o
				ram(*pv); //FALTA CRIAR -- feito escopo e comentado
				//ja temos pv criado porque � a pilha de variavel, mas ainda falta mais coisa
				op = getch();
				
				break;
				
			case 68: //F10 - exibir os prints
				listaEncadeada *auxLE = listaPrint;
				limpaTela(1, 1, 90, 90);
				gotoxy(1,1);
				printf("\n *** Prints ***\n");
				while(auxLE != NULL)
				{
					printf("\n%s",auxLE->info);
					auxLE = auxLE->prox;
				}
				op = getch();
				
				break;
			
			case 27:
				printf("\nPrograma encerrado!");
		}
		
		//chamando o menu de opc�es do programa
		op = menu();
	}
}

int main()
{
	//lista que vai apontar para a lista de tokens
    Programa *l = NULL; //vou passa l como null, porque vai ler o arquivo dentro da simula��o
    
    //lista do tipo pilha que vai guardar todas as informa��es das nossas variaveis
    Variavel *pilhaVar = NULL;
    
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
				//simulaExecucao(l, &pilhaVar);
				
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
