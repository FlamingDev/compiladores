#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 
E -> E + T       { E.val = E1.val + T.val }
E -> T           { E.val = T.val }
T -> num         { T.val = num.lexval }
*/

struct TNode{
	char val;	
};

struct ENode{
	ENode* en;
	TNode* tn;
	int op;
	int val;	
};



char s[] = "1+5++";
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

TNode* T(){
	TNode* tnode = (TNode*) malloc(sizeof(TNode));	
	char num = getNextToken();
	tnode->val = num;
	match(num);	
	return tnode;
}

ENode* E(){
	ENode* enode = (ENode*) malloc(sizeof(ENode));
	enode->tn = T();
	enode->en = NULL; 
	if (getNextToken() == '+'){
		match('+'); 
		enode->op = '+';
		enode->en = E();
	}
	return enode;
}

int evalTree(ENode* root){
	if (root == NULL){
		return 0;
	}
	if (root->en == NULL){
		return val(root->tn->val);
	}
	root->val = evalTree(root->en) + val(root->tn->val);
	return root->val;
}

int main() {
	ENode* root = E();
	if (pos == strlen(s))
		printf("%d", evalTree(root));
}

