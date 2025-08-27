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

//estrutura que vai conter as informações das nossas variaveis e fazer o papel da RAM
struct TpVariavel
{
	char identificador[TF], valor[TF];
	Programa *ponteiro;
	struct TpVariavel *prox;
};
typedef struct TpVariavel Variavel;

// -- TAD PILHA VARIAVEL --

//função que inicializa a pilha da variavel
void initPV(Variavel **pilhaVar)
{
	*pilhaVar = NULL;
}

//função que verifica se a pilha de variavel é nula ou vazia
char isEmptyPV(Variavel *pilhaVar)
{
	return pilhaVar == NULL;
}

//função que insere a variavel dentro da nossa pilha de variavel
void pushPV(Variavel **pilhaVar, char *valor, char *identificador, Programa *programa)
{
	Variavel *nova = (Variavel*)malloc(sizeof(Variavel));
	strcpy(nova->valor, valor);
	strcpy(nova->identificador, identificador);
	nova->ponteiro = programa;
	
	nova->prox = *pilhaVar;
	*pilhaVar = nova;
}

//função que vai retirar uma variavel da nossa pilha de variavel
void popPV(Variavel **pv, Variavel **pvAux)
{
	if(!isEmptyPV(*pv))
	{
		*pvAux = (Variavel*)malloc(sizeof(Variavel));
		Variavel *aux; //vai ser usado para deletar a caixinha
		
		//fazendo o auxiliar que vem por parametro receber as informações
		strcpy((*pvAux)->valor, (*pv)->valor);
		strcpy((*pvAux)->identificador, (*pv)->identificador);
		(*pvAux)->ponteiro = (*pv)->ponteiro;
		(*pvAux)->prox = NULL;
		
		aux = *pv;
		*pv = (*pv)->prox;
		free(aux);
	}
}

//função que vai tirar uma variavel da pilha pelo identificador (indepentemente se é topo ou não)
void popIdentificadorPV(Variavel **pv, char *identificador)
{
	Variavel *atual = *pv, *ant = NULL;
	
	while(atual != NULL)
	{
		if(strcmp(atual->identificador, identificador) == 0)
		{
			if(ant == NULL) //1º caso
				*pv = atual->prox;
			else
				ant->prox = atual->prox;
			
			free(atual);
			atual = NULL; //para encerrar a repetição
		}
		else
		{
			ant = atual;
			atual = atual->prox;
		}
	}
}

//função que vai retornar o topo da nossa pilha de variavel
void topPV(Variavel *pv, Variavel **pvAux)
{
	*pvAux = pv;
}

//função que busca uma variavel na pilha de variavel pelo identificador
Variavel *buscarIdentPV(Variavel *pv, char *identificador)
{
	while(pv != NULL && strcmp(pv->identificador, identificador) != 0)
		pv = pv->prox;
		
	if(pv != NULL)
		return pv;
	return NULL; //não achou
}

//função que busca um token dentro da lista
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
	return NULL; //não achou o token
}



//função que cria a caixa do nosso token
Token *CaixaToken(char *info) 
{
    Token *caixa = (Token*)malloc(sizeof(Token));
    strcpy(caixa->info, info);
    caixa->prox = NULL;
    return caixa;
}

//função que cria a caixa da nossa lista que vai apontar para lista de tokens
Programa *CaixaPrograma() 
{
    Programa *caixa = (Programa*)malloc(sizeof(Programa));
    caixa->prox = NULL;
    caixa->ant = NULL;
    caixa->token = NULL;
    return caixa;
}

//função que vai adicionar um novo token na lista de tokens
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

//função que insere uma caixa de programa na lista para que cada caixa aponte para uma linha de tokens
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

//função que verifica a existencia de um operador ou simbolo (utilizado na hora de tokenrizar)
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

//função que verifica a existencia de operadores composto (utilizado na hora de tokenrizar)
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

//função que verifica a existencia de um identificador (utilizado na hora de tokenzirar)
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

//função que lê um arquivo que foi passado e armazena os tokens linha a linha
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
	            //pula espaços, tabs e quebras de linha
	            while(linha[i] == ' ' || linha[i] == '\t' || linha[i] == '\n')
	                i++;
	
	            j = 0;
	
	            //verifica se é um operador ou símbolo isolado
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
	                //le identificadores, números, etc.
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

//TENHO QUE TERMINAR ESSA FUNÇÕES PARA EXIBIR LINHA A LINHA DO PROGRAMA
//função que exibe o codigo linha a linha conforme for dando enter
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

////função que exibe o codigo linha a linha conforme for dando enter
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

//função que contem o menu do nosso simulador de execução
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
//FUNÇÕES QUE AINDA FALTA CRIAR
// ------------------//-----------------------//--------------------//------------//---------

//struct TpControle
//{
//	int ide, l, c;
//	Programa *local;
//	struct TpControle *prox;
//};
//typedef struct TpControle Controle;

//struct TpFuncoes
//{
//	int l;
//	Programa *local;
//	char def[TF];
//	struct TpFuncoes
//};
//typedef struct TpFuncoes Funcoes;

//struct TpControleFor
//{
//	int start, stop, step, atual;
//	char variavel[30];
//	struct TpControleFor *prox;
//};
//typedef struct TpControleFor controleFor;

//struct TpListaEncadeada
//{
//	char info[100];
//	struct TpListaEncadeada *prox;
//};
//typedef struct TpListaEncadeada listaEncadeada;

//void initC(Controle **controle)

//void initLE(listaEncadeada **LE)

//void initF(Funcoes **f)

//char isEmptyF(Funcoes *f)

//void limpaTela(int lin1, int lin2, int col1, int col2)

//void ram(Variavel *pv)

//void exibirFunction(Funcoes *f)

//void mostrarLinha(Programa *p, int lin)

//função que simula a execução do nosso programa (FALTA FINALIZAR)
void simulaExecucao(Programa **programa, Variavel **pv)
{
	//(FALTA CRIAR) vai ser usado para o controle de toda a estrutura do programa que está sendo compilado
	Controle *se, *rep, *seAux, *repAux, *ifAux, *aux; //FALTA CRIAR -- feito e comentado
	int ideAtual=0, lin=0, col=0, ide, valida=0, l=0, ideFun=0, funL=0, cont=0;
	
	Programa *atual=*programa, *listaPrograma, *auxP, *fun=NULL, *atr = NULL, *numUse=NULL, *print=NULL;
	
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
	int start, stop, step; //valores que vão dentro do for
	controleFor *pFor = NULL; //FALTA CRIAR -- feito e comentado
	
	listaEncadeada *listaPrint; //FALTA CRIAR -- feito e comentado
	initLE(&listaPrint); //FALTA CRIAR -- feito escopo e comentado
	
	limpaTela(1, 1, 90, 90); //FALTA CRIAR -- feito escopo e comentado
	gotoxy(1, 1);
	
	//chamando o menu de opcões do programa
	op = menu();
	
	auxP = atual;
	initF(&funcao); //FALTA CRIAR -- feito escopo e comentado
	
	//------------------- ALERTAAAAAAAA -----------------------------------
	//ESSE BLOCO TODO DO WHILE VAI TER QUE SER FEITO DENTRO DO F7
	//PORQUE AQUI AINDA NÃO VAMOS ESTAR COM O ARQUIVO .JS ABERTO NEM TOKENRIZADO
	//PARA CONSEGUIR MUDAR AS POSIÇÕES
	//vamos andar procurando as funções se caso existir vamos pular elas
	while(auxP != NULL)
	{
		linha = auxP->token;
		
		if(strcmp(linha->info, "function") == 0) //verificando se achou um funcao
		{
			linha = linha->prox;
			while(linha != NULL && strcmp(linha->info, " ") == 0) //acho que vai ser "{" no lugar do espaço
				linha = linha->prox;
				
			enqueue(&funcoes,linha->info,l,auxP);
			l++;
			
			//vamos avançar até o final da função - token->info == }
			while(strcmp(linha->info,"}") != 0)
			{
				auxP = auxP->prox;
				linha = auxP->token;
				l++;
			}
			atual = auxP->prox;
		}
		auxP = auxP->prox;
	}
	auxP = atual;
	numUse = auxP;
	
	//repetição que vai simular a execução do programa
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
				
				break;
			
			case 66: //F8 - executar programa
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
				
				//continua a execução enquanto clicar enter = 13
				while(op == 13 && atual != NULL)
				{
					linha = atual->token;
					ide = 0; //contador para identação do codigo .js
					
					//contando o numero de tab para fazer a identação
					while(strcmp(linha->info,"\t") == 0)
					{
						ide++;
						linha = linha->prox;
					}
					
//					vamos validar se estava em uma repetição se sim vamos retornar para o inicio dela
//					e tambem validar se estava em um if para fazer ideAtual receber ide
//					para continuar a execução
					
					valida = 0;
					if(ide != ideAtual)
					{
						//codigo para avançar nas repetições ou condições
						//se a ide aumentou significa que estamos entrando um novo bloco while ou if
						//se valida =1 vamos pular
						
						auxP = atual;
						l = lin;
						
						//verifica as linhas até encontrar a mesma identação ou um bloco que termine
						while(auxP != NULL && ide > ideAtual)
						{
							linha = auxP->token;
							ide = 0; //reinicia a contagem de identação
							
							//TENHO QUE CONTINUAR DAQUIIIIIII!!!!!!!!!!!!!!!!!
						}
					}
				}
				
				break;
				
			case 67: // F9 - memoria ram
				//temos que criar ainda a pilha que vai simular a memoria ram em execução
				ram(*pv); //FALTA CRIAR -- feito escopo e comentado
				//ja temos pv criado porque é a pilha de variavel, mas ainda falta mais coisa
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
		
		//chamando o menu de opcões do programa
		op = menu();
	}
}

int main()
{
	//lista que vai apontar para a lista de tokens
    Programa *l = NULL; //vou passa l como null, porque vai ler o arquivo dentro da simulação
    
    //lista do tipo pilha que vai guardar todas as informações das nossas variaveis
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
				
				//daqui para baixo chama a função de abrir o arquivo
				//e com esse arquivo vamos tokenrizar para simular a execução
				
				break;
			
			case 66: //F8
				//aqui eu vou chamar a execução do programa linha a linha
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
