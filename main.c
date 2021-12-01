/* Syntax and Lexical Analyzer
  Grammar based off of Java rules
  Similar to Java spec doc:
  https://docs.oracle.com/javase/specs/jls/se8/html/index.html
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);
void expr(void);
void term(void);
void factor(void);
void error(void);
void boolexpr(void);
void program(void);
void block(void);
void block_statements(void);
void statement(void);
void switchstmt(void);
void foreachstmt(void);
void forstmt(void);
void dostmt(void);
void whilestmt(void);
void ifstmt(void);
void assignment(void);
void returnstmt(void);


/* Character classes */
#define LETTER 0 //[a-zA-Z]
#define DIGIT 1 //[0-9]
#define SINGLEQUOTE 2 //[\']
#define DOUBLEQUOTE 3 //[\"]
#define DECIMAL 4 //[\.]
#define KEYWORDS 5 // Reserved words in C
#define UNKNOWN 99 //[^a-zA-Z0-9]

/* Token codes */
#define FLOAT_LIT 9
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define DECIMAL_POINT 27
#define DOLLAR_SIGN 28
#define EXCLAMATION_POINT 29
#define TILDE 30
#define NUMBER_SIGN 31
#define PERCENTAGE_SIGN 32
#define CARET_SIGN 33
#define AMPERSAND_SIGN 34
#define UNDERSCORE_SIGN 35
#define COMMA_SIGN 36
#define VERTICAL_BAR 37
#define FORWARD_SLASH_SIGN 38
#define BACKSLASH_SIGN 39
#define APOSTROPHE_SIGN 40
#define OPENING_ANGLE_BRACKET 41
#define CLOSING_ANGLE_BRACKET 42
#define QUESTION_MARK 43
#define LEFT_BRACE 44
#define RIGHT_BRACE 45
#define LEFT_BRACKET 46
#define RIGHT_BRACKET 47
#define COLON 48
#define QUOTATION_MARK 49
#define SEMICOLON_SIGN 50
#define AT_SYMBOL 51

/* Reserve word codes */
#define INT_CODE 52
#define FLOAT_CODE 53
#define FOR_CODE 54
#define IF_CODE 55
#define ELSE_CODE 56
#define WHILE_CODE 57
#define DO_CODE 58
#define SWITCH_CODE 59
#define FOREACH_CODE 60
#define RETURN_CODE 61
#define VOID_CODE 62
#define MAIN_CODE 63
#define TRUE 64
#define FALSE 65
#define CASE_CODE 66
#define DEFAULT_CODE 67
#define BREAK_CODE 68


/******************************************************/
/* main driver */
int main(void) {
/* Open the input data file and process its contents */
  if ((in_fp = fopen("front.in", "r")) == NULL)
    printf("ERROR - cannot open front.in \n");
  else {
    getChar();
    do {
      lex();
      program();
    } while (nextToken != EOF);
  }
  return 0;
}


/******************************************************/
/* lookup - a function to look up operators and
 parentheses and return the token */
int lookup(char ch) {
  switch (ch) {
    case '(':
      addChar();
      nextToken = LEFT_PAREN;
      break;
    case '$':
      addChar();
      nextToken = DOLLAR_SIGN;
      break;
    case '#':
      addChar();
      nextToken = NUMBER_SIGN;
      break;
    case '^':
      addChar();
      nextToken = CARET_SIGN;
      break; 
    case '&':
      addChar();
      nextToken = AMPERSAND_SIGN;
      break;
    case '_':
      addChar();
      nextToken = UNDERSCORE_SIGN;
      break;
    case ',':
      addChar();
      nextToken = COMMA_SIGN;
      break;
    case '|':
      addChar();
      nextToken = VERTICAL_BAR;
      break;
    case '`':
      addChar();
      nextToken = APOSTROPHE_SIGN;
      break;
    case '<':
      addChar();
      nextToken = OPENING_ANGLE_BRACKET;
      break;
    case '>':
      addChar();
      nextToken = CLOSING_ANGLE_BRACKET;
      break;
    case '?':
      addChar();
      nextToken = QUESTION_MARK;
      break;
    case '{':
      addChar();
      nextToken = LEFT_BRACE;
      break;
    case '}':
      addChar();
      nextToken = RIGHT_BRACE;
      break;
    case '[':
      addChar();
      nextToken = LEFT_BRACKET;
      break;
    case ']':
      addChar();
      nextToken = RIGHT_BRACKET;
      break;
    case ':':
      addChar();
      nextToken = COLON;
      break;
    case '"':
      addChar();
      nextToken = QUOTATION_MARK;
      break;
    case ';':
      addChar();
      nextToken = SEMICOLON_SIGN;
      break;
    case '@':
      addChar();
      nextToken = AT_SYMBOL;
      break;
    case '%':
      addChar();
      nextToken = PERCENTAGE_SIGN;
      break;
    case '~':
      addChar();
      nextToken = TILDE;
      break;
    case ')':
      addChar();
      nextToken = RIGHT_PAREN;
      break;
    case '+':
      addChar();
      nextToken = ADD_OP;
      break;
    case '-':
      addChar();
      nextToken = SUB_OP;
      break;
    case '*':
      addChar();
      nextToken = MULT_OP;
      break;
    case '/':
      addChar();
      nextToken = DIV_OP;
      break;
    case '=':
      addChar();
      nextToken = ASSIGN_OP;
      break;
    case '.':
      addChar();
      nextToken = DECIMAL_POINT;
      break;
    default:
      addChar();
      nextToken = EOF;
      break;
  }
  return nextToken;
}


/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} else
	printf("Error - lexeme is too long \n");
}


/******************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar(void) {
  if ((nextChar = getc(in_fp)) != EOF) {
    if (isalpha(nextChar))
    charClass = LETTER;
    else if (isdigit(nextChar))
    charClass = DIGIT;
    else
    charClass = UNKNOWN;
  } else
  charClass = EOF;
}


/******************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
}


/******************************************************/
/* lex - a simple lexical analyzer for arithmetic
 expressions */
int lex(void) {
  lexLen = 0;
  getNonBlank();
  switch (charClass) {
    /* Identifiers */
    case LETTER:
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT) {
        addChar();
        getChar();
      }
      char keywords[17][10] = {"do","else","float","for","foreach","if","int","main","return","switch","void","while","true","false","case","default","break"};
      if(strcmp(keywords[3], lexeme) == 0){
        nextToken = FOR_CODE;
        break;
      }
      if(strcmp(keywords[5], lexeme) == 0){
        nextToken = IF_CODE;
        break;
      }
      if(strcmp(keywords[1], lexeme) == 0){
        nextToken = ELSE_CODE;
        break;
      }
      if(strcmp(keywords[11], lexeme) == 0){
        nextToken = WHILE_CODE;
        break;
      }
      if(strcmp(keywords[0], lexeme) == 0){
        nextToken = DO_CODE;
        break;
      }
      if(strcmp(keywords[6], lexeme) == 0){
        nextToken = INT_CODE;
        break;
      }
      if(strcmp(keywords[2], lexeme) == 0){
        nextToken = FLOAT_CODE;
        break;
      }
      if(strcmp(keywords[9], lexeme) == 0){
        nextToken = SWITCH_CODE;
        break;
      }
      if(strcmp(keywords[4], lexeme) == 0){
        nextToken = FOREACH_CODE;
        break;
      }
      if(strcmp(keywords[8], lexeme) == 0){
        nextToken = RETURN_CODE;
        break;
      }
      if(strcmp(keywords[10], lexeme) == 0){
        nextToken = VOID_CODE;
        break;
      }
      if(strcmp(keywords[7], lexeme) == 0){
        nextToken = MAIN_CODE;
        break;
      }
      if(strcmp(keywords[12], lexeme) == 0){
        nextToken = TRUE;
        break;
      }
      if(strcmp(keywords[13], lexeme) == 0){
        nextToken = FALSE;
        break;
      }
      if(strcmp(keywords[14], lexeme) == 0){
        nextToken = CASE_CODE;
        break;
      }
      if(strcmp(keywords[15], lexeme) == 0){
        nextToken = DEFAULT_CODE;
        break;
      }
      if(strcmp(keywords[16], lexeme) == 0){
        nextToken = BREAK_CODE;
        break;
      }
      nextToken = IDENT;
      break;
    /* Integer & Float literals */
    case DIGIT:
      addChar();
      getChar();
      while (charClass == DIGIT) {
        addChar();
        getChar();
      }
      if (charClass == UNKNOWN && nextChar == '.'){
        addChar();
        getChar();
        while (charClass == DIGIT) {
        addChar();
        getChar();
        }
        nextToken = FLOAT_LIT;
      } else {
      nextToken = INT_LIT;
      }
      break;
    case DECIMAL:
          addChar();
          getChar();
          if (charClass == DIGIT){
            addChar();
            getChar();
            while (charClass == DIGIT) {
              addChar();
              getChar();
            }
            nextToken = FLOAT_LIT;
          } else {
              nextToken = DECIMAL_POINT;
          }
      break;
    /* Parentheses and operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
      break;
    /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = '\0';
      break;
  } /* End of switch */
  printf("Next token is: %d, Next lexeme is %s\n",
  nextToken, lexeme);
  return nextToken;
} /* End of function lex */


/* expr
 Parses strings in the language generated by the rule:
 <expr> -> <term> {(+ | -) <term>}
 */
void expr(void) {
  //printf("Enter <expr>\n");
  term();
  while (nextToken == ADD_OP || nextToken == SUB_OP) {
    lex();
    term();
  }
  //printf("Exit <expr>\n");
}


/* term
 Parses strings in the language generated by the rule:
 <term> -> <factor> {(* | /) <factor>}
 */
void term(void) {
  //printf("Enter <term>\n");
  factor();
  while (nextToken == MULT_OP || nextToken == DIV_OP) {
    lex();
    factor();
  }
  //printf("Exit <term>\n");
}


/* factor
 Parses strings in the language generated by the rule:
 <factor> -> id | int_constant | ( <expr> )
 */
void factor(void) {
  //printf("Enter <factor>\n");
  if (nextToken == IDENT || nextToken == INT_LIT || nextToken == FLOAT_LIT)
    lex();
  else { if (nextToken == LEFT_PAREN) {
    lex();
    expr();
    if (nextToken == RIGHT_PAREN)
      lex();
    else
      error();
    }
    else
    error();
  }
  //printf("Exit <factor>\n");
}

/* program
  <program> -> VOID MAIN '(' ')' <block>
*/
void program(void) {
  printf("Enter <program>\n");
  if (nextToken == VOID_CODE) {
    lex();
    if (nextToken == MAIN_CODE) {
      lex();
      if (nextToken == LEFT_PAREN) {
        lex();
        if (nextToken == RIGHT_PAREN){
          lex();
          block();
        } else {
          error();
        }
      } else {
        error();
      }
    }else{
      error();
    }
  }
  printf("Exit <program>\n");
}

/* block
 <block> -> { <block_statements> }
 */
void block(void) {
  printf("Enter <block>\n");
  if (nextToken != LEFT_BRACE)
    error();
  else {
    lex();
    block_statements();
    if (nextToken == RIGHT_BRACE || nextToken == EOF)
      printf("Exit <block>\n");
  }
}

void block_statements(void){
  do {
    statement();
  } while(nextToken != EOF);
}

void statement(void) {
  switch(nextToken) {
    case SWITCH_CODE:
      switchstmt();
      break;
    case FOREACH_CODE:
      foreachstmt();
      break;
    case FOR_CODE:
      forstmt();
      break;
    case DO_CODE:
      dostmt();
      break;
    case WHILE_CODE:
      whilestmt();
      break;
    case IF_CODE:
      ifstmt();
      break;
    case INT_CODE:
      assignment();
      break;
    case FLOAT_CODE:
      assignment();
      break;
    case RETURN_CODE:
      returnstmt();
      break;
    case IDENT:
      expr();
      break;
    default:
      lex();
  }
}

/* switchblock
<switchblock> -> '{' [case int: <expr>]* | default: <expr> '}'
*/
void switchblock(void) {
  printf("Enter <switchblock>\n");
  if (nextToken != LEFT_BRACE)
    error();
  else {
    do {
      lex();
      if (nextToken == DEFAULT_CODE)
        break;
      if (nextToken != CASE_CODE)
        error();
      else {
        lex();
        if (nextToken != INT_LIT)
          error();
        else {
          lex();
          if (nextToken != COLON)
            error();
          else {
            lex();
            expr();
            lex();
            if (nextToken != BREAK_CODE)
              error();
            else {
              lex();
              if (nextToken != SEMICOLON_SIGN)
                error();
            }
          }
        }
      }
    } while (nextToken != DEFAULT_CODE);
    if (nextToken == DEFAULT_CODE) {
      lex();
      if (nextToken != COLON)
        error();
      else {
        lex();
        expr();
        lex();
        if (nextToken != BREAK_CODE)
          error();
        else {
          lex();
          if (nextToken != SEMICOLON_SIGN)
            error();
          else
            lex();
        }
      }
    }
  }
  printf("Exit <switchblock>\n");
}

/* switchstmt
<switchstmt> -> switch (id) { switchblock }
*/
void switchstmt(void) {
  printf("Enter <switchstmt>\n");
  if (nextToken != SWITCH_CODE)
    error();
  else {
    lex();
    if (nextToken != LEFT_PAREN)
      error();
    else {
      lex();
      if (nextToken != IDENT)
        error();
      else {
        lex();
        if (nextToken != RIGHT_PAREN)
          error();
        else {
          lex();
          switchblock();
        }
      }
    }
  }
  printf("Exit <switchstmt>\n");
}

/* foreachstmt
<foreachstmt> -> foreach (type id ':' id) <statement>
*/
void foreachstmt(void) {
  printf("Enter <foreachstmt>\n");
  if (nextToken != FOREACH_CODE)
    error();
  else {
    lex();
    if (nextToken != LEFT_PAREN)
      error();
    else{
      lex();
      if (nextToken != IDENT)
        error();
      else {
        lex();
        if (nextToken != IDENT)
          error();
        else {
          lex();
          if (nextToken != COLON)
            error();
          else {
            lex();
            if (nextToken != IDENT)
              error();
            else {
              lex();
              if (nextToken != RIGHT_PAREN)
                error();
              else {
                lex();
                statement();
              }
            }
          }
        }
      }
    }
  }
  printf("Exit <foreachstmt>\n");
}

/* forupdate
<forupdate> -> id ++|--
*/
void forupdate(void){
  if (nextToken != IDENT)
    error();
  else {
    lex();
    if(nextToken == ADD_OP) {
      lex();
      if(nextToken == ADD_OP)
        lex();
    } else if(nextToken == SUB_OP) {
      lex();
      if(nextToken == SUB_OP)
        lex();
    } else
      error();
  }
}

/* forstmt
<forstmt> -> for (<assignment>; <boolexpr>; <forupdate>) <statement>
*/
void forstmt(void) {
  printf("Enter <forstmt>\n");
  if (nextToken != FOR_CODE)
    error();
  else{
    lex();
    if (nextToken != LEFT_PAREN)
      error();
    else {
      lex();
      assignment();
      if (nextToken != SEMICOLON_SIGN)
        error();
      else {
        lex();
        boolexpr();
        if (nextToken != SEMICOLON_SIGN)
          error();
        else {
          lex();
          forupdate();
          if (nextToken != RIGHT_PAREN)
            error();
          else {
            lex();
            statement();
          }
        }
      }
    }
  }
  printf("Exit <forstmt>\n");
}

/* dostmt
<dostmt> -> do <statement>
*/
void dostmt(void) {
  printf("Enter <dostmt>\n");
  if (nextToken != DO_CODE)
    error();
  else {
    lex();
    statement();
  }
  printf("Exit <dostmt>\n");
}

/* whilestmt
<whilestmt> -> while (<boolexpr>)
*/
void whilestmt(void) {
  printf("Enter <whilestmt>\n");
  if (nextToken != WHILE_CODE)
    error();
  else{
    lex();
    if (nextToken != LEFT_PAREN)
      error();
    else {
      lex();
      boolexpr();
      if (nextToken != RIGHT_PAREN)
        error();
      else {
        lex();
      }
    }
  }
  printf("Exit <whilestmt>\n");
}

/* ifstmt
 <ifstmt> -> if (<boolexpr>) <statement>
 [else <statement>]
 */
void ifstmt(void) {
  printf("Enter <ifstmt>\n");
  if (nextToken != IF_CODE)
    error();
  else {
    lex();
    if (nextToken != LEFT_PAREN)
      error();
    else {
      lex();
      boolexpr();
      if (nextToken != RIGHT_PAREN)
        error();
      else {
        lex();
        statement();
        if (nextToken == ELSE_CODE) {
          lex();
          statement();
        }
      }
    }
  }
  printf("Exit <ifstmt>\n");
}

/* assignment
 <assignment> -> int|float '=' <expr>
 */
void assignment(void) {
  if (nextToken == INT_CODE || nextToken == FLOAT_CODE){
    lex();
    if (nextToken != IDENT)
      error();
    else {
      lex();
      if (nextToken != ASSIGN_OP)
        error();
      else {
        lex();
        expr();
      }
    }
  } else
    error();
}

/* returnstmt
 <returnstmt> -> return <expr>
 */
void returnstmt(void) {
  printf("Enter <returnstmt>\n");
  if (nextToken != RETURN_CODE)
    error();
  else {
    lex();
    expr();
  }
  printf("Exit <returnstmt>\n");
}

/* boolexpr
<boolexpr> -> true | false | [id|int '<'|'>' id|int]
*/
void boolexpr(void) {
  if (nextToken == TRUE || nextToken == FALSE)
    lex();
  if (nextToken == INT_LIT || nextToken == IDENT){
    lex();
    if (nextToken == OPENING_ANGLE_BRACKET || nextToken == CLOSING_ANGLE_BRACKET)
      lex();
      if (nextToken == INT_LIT || nextToken == IDENT)
        lex();
  }
}

void error(void){
  exit(1);
}
