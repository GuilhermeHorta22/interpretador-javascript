struct TpVariavel
{
	char identificador[TF], valor[TF];
	Programa *ponteiro;
};
typedef struct TpVariavel Variavel;

struct TpPilha
{
	Variavel variavel;
	struct TpPilha *prox;
};
typedef struct TpPilha Pilha;

int isTipoVariavel(char *info)
{
	return !strcmp(info, "LET") || !strcmp(info, "CONST");
}

//função que inicializa a pilha da variavel
void initPV(Variavel **pilhaVar)
{
	*pilhaVar = NULL;
}

//função que insere a variavel dentro da nossa pilha de variavel
void pushPV(Pilha **pilhaVar, Variavel auxVar)
{
	Pilha *nova = (Pilha*)malloc(sizeof(Pilha));
	strcpy(nova->variavel.valor, auxVar.valor);
	strcpy(nova->variavel.identificador, auxVar.identificador);
	nova->variavel.ponteiro = auxVar.ponteiro;
	
	nova->prox = *pilhaVar;
	*pilhaVar = nova;
}

void executaPrograma(Programa *programa)
{
	Variavel auxVar;
	Pilha *pv;
	initPV(&pv);
	TpToken *auxToken;
	auxToken = programa->token;
	Programa *auxPrograma;
	auxPrograma = programa;
	
	char auxTipo[7]; //Salvar o tipo de variável quando declarada
	
	while(auxPrograma != NULL)
	{
		while(auxToken != NULL)
		{
			if(isTipoVariavel(auxToken->info)) //verifica se o token é definição de variável LET ou CONST
			{
					strcpy(auxTipo, auxToken->info); // Salvar o tipo da váriavel para tratar da forma adequada
					auxToken = auxToken->prox;
					strcpy(auxVar.identificador, auxToken->info); //Atribui o nome da variavel que SEMPRE estará na proxima caixa. Ou seja sempre será: <<tipo>> nome =
					auxToken = auxToken->prox->prox; //Pula o "=" pq SEMPRE será '=' após declaração de variavel
					if(strcmp(auxTipo,"LET")==0)
					{
						strcpy(auxVar.valor, auxToken->info);
						auxToken = auxToken->prox;
						auxVar.ponteiro = NULL; //ARRUMAR ISSO DPS	
					}
					else //Então é CONST
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
