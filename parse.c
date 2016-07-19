#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKENS 100
#define MAXTOKENLEN  64

enum type_tag {IDENTIFIER, QUALIFIER, TYPE};

struct token{
  char type;
  char string[MAXTOKENLEN];
};

int top=-1;
struct token stack[MAXTOKENS];
struct token this;

#define pop stack[top--]
#define push(s) stack[++top]=s

/* 識別子の種類の判別 */
enum type_tag
classify_string(void)
{
  char* s = this.string;
  if (!strcmp(s, "const")){
	strcpy(s, "read-only");
	return QUALIFIER;
  }
  if(!strcmp(s, "volatile")) return QUALIFIER;
  if(!strcmp(s, "void")) return TYPE;
  if(!strcmp(s, "char")) return TYPE;
  if(!strcmp(s, "signed")) return TYPE;
  if(!strcmp(s, "unsigned")) return TYPE;
  if(!strcmp(s, "short")) return TYPE;
  if(!strcmp(s, "int")) return TYPE;
  if(!strcmp(s, "long")) return TYPE;
  if(!strcmp(s, "float")) return TYPE;
  if(!strcmp(s, "double")) return TYPE;
  if(!strcmp(s, "struct")) return TYPE;
  if(!strcmp(s, "union")) return TYPE;
  if(!strcmp(s, "enum")) return TYPE;
  return IDENTIFIER;
}

/* thisに次のトークンを読み込む */
void
gettoken(void)
{
  char* p = this.string;

  /* 空白があれば読み飛ばす */
  while((*p = getchar()) == ' ');

  if(isalnum(*p)){
	/* 英数字からはじまる場合は識別子として読み込む */
	while(isalnum(*++p = getchar()));
	ungetc(*p, stdin);
	*p = '\0';
	this.type = classify_string();
	return;
  }

  if(*p=='*'){
	strcpy(this.string, "pointer to");
	this.type = '*';
	return;
  }
  this.string[1] = '\0';
  this.type = *p;
  return;
}


/* パースのすべてを理解できるコード */
read_to_first_identifier()
{
  gettoken();
  while(this.type!=IDENTIFIER){
	push(this);
	gettoken();
  }
  printf("%s is ", this.string);
  gettoken();
}


deal_with_arrays()
{
  while(this.type == '[') {
	printf("array ");
	gettoken(); /* 数値か']' */
	if(isdigit(this.string[0])){
	  printf("0..%d ", atoi(this.string)-1);
	  gettoken(); /* ']'を読む */
	}
	gettoken(); /* ']'の次を読む */
	printf("of ");
  }
}


deal_with_function_args()
{
  while(this.type != ')'){
	gettoken();
  }
  gettoken();
  printf("function returning ");
}


deal_with_pointers()
{
  while(stack[top].type == '*'){
	printf("%s ", pop.string);
  }
}


deal_with_declarator()
{
  /* 識別子の後ろに書かれた、配列と関数を処理する */
  switch(this.type){
  case '[' : deal_with_arrays(); break;
  case '(' : deal_with_function_args();
  }

  deal_with_pointers();

  /* 識別子まで読み進める間にスタックに積まれたトークンを処理する */
  while(top>=0){
	if(stack[top].type == '('){
	  pop;
	  gettoken(); /* ')'の次まで読み進む */
	  deal_with_declarator();
	}else{
	  printf("%s ",pop.string);
	}
  }
}


int
main(int argc, char* argv[])
{
  /* 識別子にたどり着くまでトークンをスタックに積み上げる */
  read_to_first_identifier();
  deal_with_declarator();
  printf("\n");
  return 0;
}

