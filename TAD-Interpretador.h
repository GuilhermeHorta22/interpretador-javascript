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
	struct Token token;
	struct TpPrograma *ant, *prox;
};
typedef struct TpPrograma Programa;
