/* 
E -> T + E       { E.val = E1.val + T.val }
E -> T           { E.val = T.val }
T -> num         { T.val = num.lexval }
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_CHILDREN 2
// Tokens recognized by the language
enum Token {
	NUM, PLUS, EMPTY
};
// Tokens will be represented with its lexical value. Example: {PLUS, '+'}
struct TokenRecord {
	Token tokenType;
	char lexval;
};
// Expressions might be composed of a operation, a variable or a constant
enum ExpKind{
	OpK, ConstK
};
// Parsing tree node containing all fields that can be required
// Currently, the max children is 2 for operation nodes
// We keep also, a union of the mutually exclusive possible node types and an ExpKind to identify it
struct STNode{
	STNode* children[MAX_CHILDREN] = {NULL};
	ExpKind expk;
	union{
		Token op;
		char val;	
	}attr;
};

char s[] = "1+6+6";
int pos = 0;
int PARSE_ERROR = 0;

int val(char c){
	switch(c){
		case '0':
			return 0;
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
// Useful for debugging
const char* strRepr(Token tk){
	switch(tk){
		case NUM:
			return "NUM";
			break;
		case PLUS:
			return "PLUS";
			break;
		case EMPTY:
			return "EMPTY";
			break;	
	}
}

void match(Token expected, Token obtained){
	if (expected == obtained)
		pos++;
	else{
		PARSE_ERROR = 1;
		printf("Parsing error. Expected %s but got %s", strRepr(expected), strRepr(obtained));
	}
		
}
// Prototype of lexical analyser
TokenRecord getNextToken(){
	char next = s[pos];
	Token tk = EMPTY;
	if (isdigit(next))
		tk = NUM;
	if (next == '+')
		tk = PLUS;
		
	TokenRecord rec = {tk, next};
	return rec;
}
// Recursive descendant parser
STNode* T(){
	STNode* tnode = (STNode*) malloc(sizeof(STNode));	
	tnode->expk = ConstK;
	TokenRecord rec = getNextToken();
	tnode->attr.val = rec.lexval;
	match(NUM, rec.tokenType);	
	return tnode;
	
}

STNode* E(){
	STNode* tnode = T();
	TokenRecord rec =  getNextToken();
	if (rec.lexval == '+'){
		STNode* enode = (STNode*) malloc(sizeof(STNode));
		match(PLUS, rec.tokenType); 
		enode->expk = OpK;
		enode->attr.op = PLUS;	
		enode->children[0] = tnode;
		enode->children[1] = E();
		return enode;
	}
	return tnode;
}
 
int evalTree(STNode* root){
	if (root == NULL){
		return 0;
	}
	if (root->expk == ConstK){
		return val(root->attr.val);
	}
			
	return evalTree(root->children[0]) + evalTree(root->children[1]); 
}

int main() {
	STNode* root = E();
	if (!PARSE_ERROR)
		printf("%d", evalTree(root));
}

