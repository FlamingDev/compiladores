#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define MAX_TOKEN_LEN 80
#define MAX_BUF_LEN 256

typedef enum{
	/* Tokens da linguagem */
	NUM,PLUS,MINUS,MULT,DIV,OPEN_PAR,CLOSE_PAR,
	/* Tokens de controle */
	ERROR,END_LINE
}TokenType;

char buffer[MAX_BUF_LEN];
int bufpos = 0;
char tokenStr[MAX_TOKEN_LEN];

char getNextChar(){
	return buffer[bufpos++];
}

void ungetChar(){
	bufpos--;
}

TokenType getToken(){
	char c = getNextChar();	/* Armazena o pr�ximo caractere da entrada*/
	TokenType token; /* Armazena o token */
	int tokenStrIndex = 0;	/* �ndice para guardar o lexema */
	
	/* L�gica para reconhecimento de tokens NUM de tamanho vari�vel. ER: digito(digito)* */
	if (isdigit(c)){
		token = NUM;
		while (isdigit(c)){ 
			tokenStr[tokenStrIndex++] = c;
			c = getNextChar();
		}
		ungetChar();
	}
	else{ /* Reconhecimento de tokens de caractere �nico */
		switch(c){
			case '(':
				token = OPEN_PAR;
				break;
			case ')':
				token = CLOSE_PAR;
				break;
			case '+':
				token = PLUS;
				break;
			case '-':
				token = MINUS;
				break;
			case '*':
				token = MULT;
				break;
			case '/':
				token = DIV;
				break;
			case '\n':
				token = END_LINE;
				break;
			default:
				token = ERROR;
				break;
		}
	}
	return token;
}

char *getTokenName(TokenType t){ /*Fun��o auxiliar para mensagens de erro*/
	switch(t){
		case NUM:
			return "NUM";
		case PLUS:
			return "+";
		case MINUS:
			return "-";
		case MULT:
			return "*";
		case DIV:
			return "/";
		case OPEN_PAR:
			return "(";
		case CLOSE_PAR:
			return ")";
		case END_LINE:
			return "END LINE";
		default:
			return "invalid token";
	}
}

void expr();
void termo();
void fator();
void match(TokenType);
void panic();

/* Gram�tica da linguagem de express�es aritm�ticas:
init -> expr
expr -> termo {soma termo}
termo -> fator {mult fator}
fator -> (expr) | n�mero
soma -> + | -
mult -> * | /	
*/

TokenType lookahead; // guarda o pr�ximo token a ser reconhecido pela gram�tica
void init(){
	lookahead = getToken();
	expr();
}

void expr(){
	termo();
	while (lookahead == PLUS || lookahead == MINUS){
		match(lookahead);
		termo();
	}
}

void termo(){
	fator();
	while (lookahead == MULT || lookahead == DIV){
		match(lookahead);
		fator();
	}
	
}

void fator(){
	if (lookahead == OPEN_PAR){
		match(OPEN_PAR);
		expr();
		match(CLOSE_PAR);
	}
	else if (lookahead == NUM){
		match(NUM);
	}
	else{
		printf("Erro: '%s'. Esperado ( ou NUM\n",getTokenName(lookahead));
		
		/* Modo p�nico: O compilador l� tokens at� que seja encontrado o in�cio
		de uma express�o matem�tica v�lida, que pode iniciar com '(' ou NUM.
		Se encontrou, ent�o tentamos derivar uma express�o novamente */
		
		panic();
		if (lookahead != END_LINE){
			printf("tentando derivar fator novamente...\n");
			fator();
		}
	}
}

void match(TokenType expected){
	if (lookahead == expected){
		lookahead = getToken();
	}
	else{
		if (lookahead != END_LINE || expected == CLOSE_PAR){
			printf("Erro: '%s'. Esperado %s\n",getTokenName(lookahead),getTokenName(expected));
			panic();
		}				
	}
}

void panic(){
	while (lookahead != OPEN_PAR && lookahead != NUM && lookahead != END_LINE){
		lookahead = getToken();
		printf("PANIC...\n");
	}
}

int main(){
	strcpy(buffer,"5*2+1*2+4\n");
	printf("%s\n",buffer);
	init();
	
	if (lookahead != END_LINE){
		printf("token inesperado '%s'",getTokenName(lookahead));
	}
	return 0;
}
