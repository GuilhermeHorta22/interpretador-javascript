struct TpVariavel
{
	char identificador[TF], valor[TF];
	Programa *ponteiro;
};
typedef struct TpVariavel Variavel;

struct TpPilha
{
	Variavel *variavel;
	struct TpPilha *prox;
};
typedef struct TpPilha Pilha;

void executaPrograma(Programa *programa)
{
	Variavel *pv, auxVar;
	TpToken *auxToken;
	auxToken = programa->token;
	Programa *auxProgram;
	auxProgram = programa;
	
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
					if(strcmp(auxTipo,'LET')==0)
					{
						strcpy(auxVar.valor, auxToken.info);
						auxToken = auxToken->prox;
						auxVar.ponteiro = NULL; //ARRUMAR ISSO DPS	
					}
					else //Ent�o � CONST
					{
						auxVar.valor = NULL;
						auxToken = auxToken->prox;
						auxVar.ponteiro = auxToken->info;
					}
					push(&pv,auxVar, tipoVar) //Passar a pilha, a variavel, e o tipo dela pq vamos tratar de diferentes formas se for LET ou COSNT
			}
		}
	} 
	
}
