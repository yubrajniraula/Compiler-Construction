/******************************************************************* 
Name: Yubraj Niraula            NetID: yn79
Course: CSE 4713                Assignment: Part 1
Programming Environment: Ubuntu Linxu C++
Purpose of File: Contains the parsing rules for TIPS. It has regular 
expressions listed. 
*******************************************************************/
%option noyywrap
%{
#include "lexer.h"

// global variable to hold current line number being read
int line_number = 1;
int num_newline = 0; // variable to hold number of new lines in string literal

// function to increase variable num_newline
void inc_linenum()
{
	for (int c = 0; c < yyleng; c++){
	if (yytext[c] == '\n') num_newline++; // if any character is a newline in yytext, increase num_newline
	}
}
%}

%%
 
\n	{ line_number++;
	  if (num_newline > 0)
		{
			// add num_newline to line_number to take line numbers inside string literal into account
			line_number += num_newline; 
			num_newline = 0;
		}
	}
 
  /* Keywords */ 

\BEGIN  { return TOK_BEGIN; }
\BREAK  { return TOK_BREAK; }
\CONTINUE { return TOK_CONTINUE; }
\DOWNTO { return TOK_DOWNTO; }
\ELSE   { return TOK_ELSE; }
\END    { return TOK_END; }
\FOR    { return TOK_FOR; }
\IF     { return TOK_IF; }
\LET    { return TOK_LET; }
\PROGRAM { return TOK_PROGRAM; }
\READ   { return TOK_READ; }
\THEN   { return TOK_THEN; }
\TO     { return TOK_TO; }
\VAR    { return TOK_VAR; }
\WHILE  { return TOK_WHILE; }
\WRITE  { return TOK_WRITE; }



 /* Datatype Specifiers */

\INTEGER    { return TOK_INTEGER; }
\REAL       { return TOK_REAL; }


 /* Punctuation */
 \;        { return TOK_SEMICOLON; }
 \:        { return TOK_COLON; }
 \(        { return TOK_OPENPAREN; }
 \)        { return TOK_CLOSEPAREN; }
 \{        { return TOK_OPENBRACE; }
 \}        { return TOK_CLOSEBRACE; }



 /* Operators */
\+      { return TOK_PLUS; }
\-      { return TOK_MINUS; }
\*      { return TOK_MULTIPLY; }
\/      { return TOK_DIVIDE; }
\:=     { return TOK_ASSIGN; }
\=      { return TOK_EQUALTO; }
\<      { return TOK_LESSTHAN; }
\>      { return TOK_GREATERTHAN; }
\<>     { return TOK_NOTEQUALTO; }
\MOD    { return TOK_MOD; }
\NOT    { return TOK_NOT; }
\OR     { return TOK_OR; }
\AND    { return TOK_AND; }


 /* Abstractions */
 [A-Z][A-Z0-9]{0,7}   { return TOK_IDENT; }
 [0-9]+               { return TOK_INTLIT; }
 [0-9]+\.[0-9]+       { return TOK_FLOATLIT; }
 '[^']{0,79}'         { inc_linenum();
		    			return TOK_STRINGLIT;  }

 /* Eat any whitespace */

[ \t\r]+               /* nop */


 /* Found an unknown character */

.         { return TOK_UNKNOWN; }


 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }
