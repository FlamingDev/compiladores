#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 
E -> E + T       { E.val = E1.val + T.val }
E -> T           { E.val = T.val }
T -> num         { T.val = num.lexval }
*/



char s[] = "1+5+6+7+9";
int pos = 0;

int val(char c){
	switch(c){
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		default:
			return 29249;
	}
}

void match(char token){
	if (val(token) < 10 or token == '+')
		pos++;
}

char getNextToken(){
	return s[pos];
}

int T(){
	char op = getNextToken();
	match(op);
	return val(op); // T -> num
}

int E(){
	int op1 = T(); // E -> T
	while (getNextToken() == '+'){
		match('+');
		int op2 = T();
		op1 += op2;
	}
	return op1;
}

int main() {
	int result = E();
    printf("%d", pos == strlen(s)? result: -1);
}

