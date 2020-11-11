/******************************************************************* 
Name: Yubraj Niraula            NetID: yn79
Course: CSE 4713                Assignment: Part 4
Programming Environment: MacOS Visual Studio Code C++
Purpose of File: Driver file for the whole program
*******************************************************************/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

using namespace std;

#include "lexer.h"
#include "parse_tree_nodes.h"
#include "productions.h"

extern "C" {
    // Instantiate global variables
    extern FILE *yyin; // input stream
    extern FILE *yyout; // output stream
    extern int yyleng; // length of current lexeme
    extern char *yytext; // text of current lexeme
    extern int yylex(); // the generated lexical analyzer
    extern int line_number; // current line number of the input
}

// Needed global variables
int nextToken = 0; // token returned from yylex
int level = -1; // current indentation level

// Feel free to use a different data structure for the symbol table (list of
// variables declared in the program) but you will have to adjust the code in
// main() to print out the symbol table after a successful parse

// update symbol table to store identifier with its value
map<string, int> symbolTable; // Symbol Table


//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[]) {
    // Set the input stream
    if (argc > 1) {
        printf("INFO: Using the file %s for input\n", argv[1]);
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            printf("***ERROR: input file not found\n");
            return (1);
        }
    } else {
        printf("INFO: Using stdin for input, use EOF to end input\n");
        printf("      Windows EOF is Ctrl+z, Linux EOF is Ctrl+d\n");
        yyin = stdin;
    }

    // Set the output stream
    yyout = stdout;

    // Create the root of the parse tree
    programNode* root = nullptr;

    // Get the first token
    nextToken = yylex();

    // Fire up the parser!
    try {
        if (!first_of_program()) // Check for PROGRAM
            throw "3: 'PROGRAM' expected";

        // Process <program> production
        root = program();

        if (nextToken != TOK_EOF)
            throw "end of file expected, but there is more here!";
        
    } catch (char const *errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << line_number << ", error type ";
        cout << errmsg << endl;
        return 1;
    }

    // Tell the world about our success!!
    cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;

    // Print out the symbol table
    cout << endl << "User Defined Symbols:" << endl;
    map<string, int>::iterator it;
    for (it = symbolTable.begin(); it != symbolTable.end(); ++it) {
        cout << it->first << endl;
    }

    cout << endl << endl << "*** In order traversal of parse tree ***" << endl;
    cout << *root << endl;

    cout << endl << endl << "*** Interpreting the TIPS program ***" << endl;
    root->interpret();

    cout << endl<< endl<<"*** Delete the parse tree ***" << endl;
    delete root;

    return 0;
}

