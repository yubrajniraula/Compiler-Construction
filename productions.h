/******************************************************************* 
Name: Yubraj Niraula            NetID: yn79
Course: CSE 4713                Assignment: Part 3
Programming Environment: MacOS Visual Studio Code C++
Purpose of File: Recursive descent parser to parse any program inputted. 
    Now this returns a pointer to each node
*******************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H


extern int nextToken;
extern int level;
extern map<string, int> symbolTable;

extern "C"
{
	// Instantiate global variables used by flex
	extern char* yytext;       // text of current lexeme
	extern int   yylex();      // the generated lexical analyzer
}

// function declarations
programNode *program();
blockNode *block();
compoundNode *compound_statement();
statementNode *statement();
assignmentNode* assignment_statement();
ifNode* if_statement();
whileNode* while_statement();
readNode* read_statement();
writeNode* write_statement();
expressionNode* expression();
simpleExpressionNode* simple_expression();
termNode* term();
factorNode* factor();

string spaces() {
	string str(level * 4, ' ');
	return str;
}

void output_lexeme(){
    cout << spaces() << "-->found " << yytext << endl;
}

int lex() {
  nextToken = yylex();
  if (nextToken == TOK_EOF){
      yytext[0] = 'E';
      yytext[1] = 'O';
      yytext[2] = 'F';
      yytext[3] = 0;
  }
  return nextToken;
}

// start checking from here
programNode *program() { 
    programNode *pProgram; // create a pointer to programNode
    ++level;
    cout << spaces() << "enter <program>" << endl;
    output_lexeme();
    lex();
    string x = yytext; // store the name of the program in x
    // Parse input to see if the input matches the EBNF rule for <program>
    if (nextToken == TOK_IDENT){
        output_lexeme();
        lex();
        if (nextToken == TOK_SEMICOLON){
            output_lexeme();
            pProgram = new programNode(x, block()); // pass program name and pointer to block to programNode

        }
        else throw "14: ';' expected";
    }
    else throw "2: identifier expected";

    cout << spaces() << "exit <program>" << endl;
    --level;
    return pProgram;
}


blockNode* block(){
    blockNode *pBlock;
    bool begin_check = false;
    ++level;
    cout << spaces() << "enter <block>" << endl;
    lex();
    if (nextToken == TOK_VAR){
        output_lexeme();
        lex();
        do{ // check for identifier at least once and keep looping unless begin_check is false
            if (nextToken == TOK_IDENT){
                output_lexeme();
                if (symbolTable.count(yytext)) throw "101: identifier declared twice"; //might have to use find instead of count
                symbolTable.insert(pair<string, int>(yytext, 0)); // update the symbol table
                lex();
                if(nextToken == TOK_COLON){
                    output_lexeme();
                    lex();
                    if(nextToken == TOK_INTEGER || nextToken == TOK_REAL){
                        output_lexeme();
                        lex();
                        if(nextToken == TOK_SEMICOLON){
                            output_lexeme();
                            lex();
                            cout<<'\n'; // check why this is here???????????????????????????????
                            if(nextToken == TOK_IDENT){
                                begin_check = false;
                            }
                            else if (nextToken == TOK_BEGIN){
                                begin_check = true;
                                pBlock = new blockNode(compound_statement()); // call compound_statement if next token is BEGIN
                            }
                            else throw "17: 'BEGIN' expected";
                        }
                        else throw "14: ';' expected";
                    }
                    else throw "10: error in type";
                }
                else throw "5: ':' expected";
            }
            else throw "2: identifier expected";
        } while (begin_check == false);   
    }
    else if (nextToken == TOK_BEGIN){
        pBlock = new blockNode(compound_statement());
    }
    else throw "18: error in declaration part OR 17: 'BEGIN' expected";

    cout << spaces() << "exit <block>" << endl;
    --level;
    return pBlock;
}

compoundNode *compound_statement(){
    compoundNode *pComp;
    ++level;
    cout << spaces() << "enter <compound_statement>" << endl;
    output_lexeme();
    lex();
    // check for statement at least once
    pComp = new compoundNode(statement()); 
    // if semicolon comes after statement, keep looping until END comes
    while (nextToken == TOK_SEMICOLON){
        output_lexeme();
        lex();
        pComp->restStatements.push_back(statement()); // if more than one statements in compound statement, push into a vector
    } 
    if (nextToken == TOK_END){
        output_lexeme();
        lex();
    } 
    else throw "13: 'END' expected";
    cout << spaces() << "exit <compound_statement>" << endl;  
    --level;  
    return pComp;
}

statementNode *statement(){
    statementNode *pState;
    ++level;
    cout << spaces() << "enter <statement>" << endl;
    if (nextToken == TOK_IDENT){
        pState = assignment_statement(); // just receive the pointer from assignment_statement() and save it in statementNode pointer pState
    }
    else if (nextToken == TOK_BEGIN){
        pState = compound_statement(); 
    }
    else if (nextToken == TOK_IF){
        pState = if_statement();
    }
    else if(nextToken == TOK_WHILE){
        pState = while_statement();
    }
    else if (nextToken == TOK_READ){
        pState = read_statement();
    }
    else if(nextToken == TOK_WRITE){
        pState = write_statement();
    }
    else throw "900: illegal type of statement"; 
    cout << spaces() << "exit <statement>" << endl;
    --level;
    return pState;
}

assignmentNode* assignment_statement(){
    assignmentNode *pAss;
    ++level;
    cout << spaces() << "enter <assignment>" << endl;
    output_lexeme();
    string y = yytext; // store the name of the identifier in a string y

    // check in the symbol table if the identifier is declared before; if not, throw error
    if (!symbolTable.count(yytext)) throw "104: identifier not declared";
    lex();
    if (nextToken == TOK_ASSIGN){
        output_lexeme();
        lex();
        expressionNode *exp = expression();
        // cout<<"********"<<exp->pSimpleExp1<<endl;
        pAss = new assignmentNode(y, exp);
        //symbolTable[y] = exp->interpret();
    }
    else throw "51: ':=' expected";
    cout << spaces() << "exit <assignment>" << endl;
    --level;
    return pAss;
}

ifNode* if_statement(){
    ifNode *pIf;
    ++level;
    cout << spaces() << "enter <if statement>" << endl;
    output_lexeme();
    lex();
    expressionNode *pEx = expression();
    if(nextToken == TOK_THEN){
        output_lexeme();
        lex();
        statementNode *pSt = statement(); // create a separate pointer to store pointer returned from statement()
        // pass pEx and pSt separately as if may not have else clause
        // if no else found, return this pointer pIf
        pIf = new ifNode(pEx, pSt); 
        if(nextToken == TOK_ELSE){ // else may not be required, if no ELSE found, get out of statement
            output_lexeme();
            lex();
            pIf = new ifNode(pEx, pSt, statement()); // if Else found, return this pointer pIf
        }
    }
    else throw "52: 'THEN' expected";
    cout << spaces() << "exit <if statement>" << endl;
    --level;
    return pIf;
}

whileNode* while_statement(){
    whileNode *pWhile;
    ++level;
    cout << spaces() << "enter <while statement>" << endl;
    output_lexeme();
    lex();
    expressionNode *pEx = expression(); // while statement contains expression then statement
    pWhile = new whileNode(pEx, statement());
    cout << spaces() << "exit <while statement>" << endl;
    --level;
    return pWhile;
}

readNode* read_statement(){
    readNode *pRead;
    ++level;
    cout << spaces() << "enter <read>" << endl;
    output_lexeme();
    lex();
    if (nextToken == TOK_OPENPAREN){
        output_lexeme();
        lex();
        if(nextToken == TOK_IDENT){
            output_lexeme();
            if (!symbolTable.count(yytext)) throw "104: identifier not declared";
            pRead = new readNode(yytext);
            lex();
            if(nextToken == TOK_CLOSEPAREN){
                output_lexeme();
                lex();
            }
            else throw "4: ')' expected";
        }
        else throw "2: identifier expected";
    }
    else throw "9: '(' expected";
    cout << spaces() << "exit <read>" << endl;
    --level;
    return pRead;
}

writeNode* write_statement(){
    writeNode *pWrite;
    string y;
    ++level;
    cout << spaces() << "enter <write>" << endl;
    output_lexeme();
    lex();
    if(nextToken == TOK_OPENPAREN){
        output_lexeme();
        lex();
        if(nextToken == TOK_IDENT){
            output_lexeme();
            if (!symbolTable.count(yytext)) throw "104: identifier not declared";
            pWrite = new writeNode(yytext, nextToken);
            lex();
        }
        else if(nextToken == TOK_STRINGLIT){
            output_lexeme();
            pWrite = new writeNode(yytext, nextToken);
            lex();
        }
        else throw "134: illegal type of operand(s)";
        if(nextToken == TOK_CLOSEPAREN){
            output_lexeme();
            lex();
        }
        else throw "4: ')' expected";
    }   
    else throw "9: '(' expected";
    cout << spaces() << "exit <write>" << endl;
    --level;
    return pWrite;
}

expressionNode* expression(){
    expressionNode *pExpr;
    ++level;
    cout << spaces() << "enter <expression>" << endl;
    if(nextToken != TOK_INTLIT && nextToken != TOK_FLOATLIT && nextToken != TOK_IDENT && nextToken != TOK_OPENPAREN && nextToken != TOK_NOT && nextToken != TOK_MINUS){
       throw "144: illegal type of expression";
    }
    simpleExpressionNode *pSimp = simple_expression(); // do separately as an expression may only contain a single simple expression
    pExpr = new expressionNode(pSimp); // if only one expression, return this pointer
    if (nextToken == TOK_EQUALTO || nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_NOTEQUALTO){
        output_lexeme();
        int y = nextToken;
        lex();
        pExpr = new expressionNode(pSimp, y, simple_expression()); // if two expressions, return this pointer
        // vector not required as expression can contain at max two simple expressions
    }
    cout << spaces() << "exit <expression>" << endl;
    --level;
    return pExpr;
}

simpleExpressionNode* simple_expression(){
    simpleExpressionNode *pSimple;
    ++level;
    cout << spaces() << "enter <simple expression>" << endl;
    if(nextToken != TOK_INTLIT && nextToken != TOK_FLOATLIT && nextToken != TOK_IDENT && nextToken != TOK_OPENPAREN && nextToken != TOK_NOT && nextToken != TOK_MINUS){
       throw "901: illegal type of simple expression";
    }
    pSimple = new simpleExpressionNode(term()); 
    while (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR){
        output_lexeme();
        // vector required as a simple expression may have more than one terms
        pSimple->restTermOps.push_back(nextToken); // vector to store those operands, pass their token number
        lex();
        pSimple->restTerms.push_back(term()); // vector to store the term itself, pass the whole term
    }
    cout << spaces() << "exit <simple expression>" << endl;
    --level;
    return pSimple;
}

termNode* term(){
    termNode *pTerm;
    ++level;
    cout << spaces() << "enter <term>" << endl;
    if(nextToken != TOK_INTLIT && nextToken != TOK_FLOATLIT && nextToken != TOK_IDENT && nextToken != TOK_OPENPAREN && nextToken != TOK_NOT && nextToken != TOK_MINUS){
       throw "902: illegal type of term";
    }
    pTerm = new termNode(factor());
    while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND){
        output_lexeme();
        pTerm->restFactorOps.push_back(nextToken);
        lex();
        pTerm->restFactors.push_back(factor());
    }
    cout << spaces() << "exit <term>" << endl;
    --level;
    return pTerm;
}

factorNode* factor(){
    factorNode *pFact;
    ++level;
    cout << spaces() << "enter <factor>" << endl;
    if(nextToken == TOK_INTLIT){
        pFact = new intLitNode(stoi(yytext)); // yytext has interger as a string, convert it into integer
        output_lexeme();
        lex();
    }
    else if(nextToken == TOK_FLOATLIT){
        pFact = new floatLitNode(stof(yytext));
        output_lexeme();
        lex();
    }
    else if(nextToken == TOK_IDENT){
        output_lexeme();
        if (!symbolTable.count(yytext)) throw "104: identifier not declared";
        pFact = new identNode(yytext);
        lex();
    }
    else if(nextToken == TOK_OPENPAREN){
        output_lexeme();
        lex();
        pFact = new nestedExprNode(expression()); 
        if (nextToken == TOK_CLOSEPAREN){
            output_lexeme();
            lex();
        }
        else throw "4: ')' expected";
    }
    else if(nextToken == TOK_NOT){
        int y = nextToken;
        output_lexeme();
        lex();
        pFact = new nestedFactorNode(y, factor()); // pass token NOT as well
    }
    else if(nextToken == TOK_MINUS){
        int y = nextToken;
        output_lexeme();
        lex();
        pFact = new nestedFactorNode(y, factor()); // pass token MINUS as well
    }
    else throw "903: illegal type of factor";
    cout << spaces() << "exit <factor>" << endl;
    --level;
    return pFact;
}



bool first_of_program(void) {
    return nextToken == TOK_PROGRAM;
}

#endif

