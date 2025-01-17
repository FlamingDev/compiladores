#include <stdio.h>
#define TRUE 1
#define FALSE 0
#define ERROR -1
#define INITIAL 1
#define OPEN_SLASH 2
#define OPEN_ASTERISK 3
#define CLOSING_ASTERISK 4
#define CLOSING_SLASH 5



/* 
Estados do AFD na forma f(estado corrente, símbolo lido) = estado alvo

f(INITIAL,'/') = OPEN_SLASH
f(OPEN_SLASH,'*') = OPEN_ASTERISK
f(OPEN_ASTERISK,outro) = OPEN_ASTERISK
f(OPEN_ASTERISK,'*') = CLOSING_ASTERISK
f(OPEN_ASTERISK,'*') = CLOSING_ASTERISK
f(CLOSING_ASTERISK,outro) = OPEN_ASTERISK
f(CLOSING_ASTERISK,'/') = CLOSING_SLASH	

*/
char buf[256];
int bufpos = 0;

char getNextChar(){
	return buf[bufpos++];
}
/*oi*/ // aceita
/***oi*/ // aceita
/* /oi*/ // rejeita 

int acceptFirstComment() {
    int state = INITIAL;
    char tokenStr[256];
    int strpos = 0;
    char c = getNextChar();
    printf("computation:\n\n");
	while (c != '\0'){
		switch (state){
			case INITIAL:
				printf("initial state\n");
				switch (c){
					case '/':
						c = getNextChar();    
						state = OPEN_SLASH;
						break;
					default:
						state = ERROR;
						break;
				}
				break;
			case OPEN_SLASH:
				printf("open slash state\n");
				switch (c){
					case '*':
						c = getNextChar();
						state = OPEN_ASTERISK;
						break;
					default:
						state = ERROR;
						break;
				}
				break; 
			case OPEN_ASTERISK:
				printf("open asterisk state (reading comments)\n");
				switch (c){
					case '*':
						c = getNextChar();
						state = CLOSING_ASTERISK;
						break; 
					default:
						tokenStr[strpos++] = c; 
						c = getNextChar();
						break;
				}
				break;
			case CLOSING_ASTERISK:
				printf("closing asterisk state\n");
				switch (c){
					case '/':
						c = getNextChar();
						state = CLOSING_SLASH;
						break;
					case '*':
						tokenStr[strpos++] = c;
						c = getNextChar();
						break;
					default:
						state = OPEN_ASTERISK;
						break;
				}
				break;
			case ERROR:
				printf("Error state\n");
				switch (c){
					case '/':
						state = OPEN_SLASH;
						c = getNextChar();
						break;
					default:
						c = getNextChar();
						break;
				}
				break;
			default:
				c = getNextChar();
				break;
		}
		
	}
	if (state == CLOSING_SLASH){
		printf("Comentario aceito: %s",tokenStr);
	}
	else printf("Comentario invalido. Estado corrente: %d", state);
}

int main(){
    printf("Digite a entrada:\n");
    scanf("%s",&buf);
    acceptFirstComment();
    return 0;
}

