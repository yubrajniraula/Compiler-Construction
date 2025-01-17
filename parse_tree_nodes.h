/******************************************************************* 
Name: Yubraj Niraula            NetID: yn79
Course: CSE 4713                Assignment: Part 4
Programming Environment: MacOS Visual Studio Code C++
Purpose of File: Header file for parse_tree_cpp
*******************************************************************/

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "lexer.h"

using namespace std;

extern map<string, int> symbolTable;

// forward declarations of classes
class programNode;
class blockNode;
class statementNode;
class assignmentNode;
class compoundNode;
class ifNode;
class whileNode;
class readNode;
class writeNode;
class expressionNode;
class simpleExpressionNode;
class termNode;
class factorNode;
class intLitNode;
class floatLitNode;
class identNode;
class nestedExprNode;
class nestedFactorNode;

class programNode{
public:
    string* name = nullptr;
    blockNode* block = nullptr;

    programNode(string id, blockNode* blk);
    int interpret();
    ~programNode();
};

class blockNode{
public:
    compoundNode* compoundStatement = nullptr;

    blockNode(compoundNode* cpd);
    int interpret();
    ~blockNode();
};

// Abstract class. Base class for assignmentNode, compoundNode, ifNode, whileNode,
//                    readNode, writeNode
//
// Remove if not using inheritance to model the statement hierarchy
class statementNode{
public:
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual int interpret() = 0;
    virtual ~statementNode(){} // labeling the destructor as virtual allows 
	                          // the subclass destructors to be called
};

//
// You do not have to use inheritance....
//
class assignmentNode : public statementNode{
public:
    string* name = nullptr;
    expressionNode* exprs = nullptr;

    assignmentNode(string name, expressionNode* expr);
    int interpret();
    void printTo(ostream &os);
    ~assignmentNode();
};


class compoundNode : public statementNode {
public:
    statementNode *mystate = nullptr;

    compoundNode(statementNode* mystate);
    vector <statementNode*> restStatements; // vector to store the rest of the statements of the compound statement
    int interpret();
    void printTo(ostream &os);
    ~compoundNode();
};


class ifNode : public statementNode {
public:
    expressionNode* expression = nullptr;
    statementNode* thenStatement = nullptr;
    statementNode* elseStatement = nullptr;

    ifNode(expressionNode* expr, statementNode* thenState);
    ifNode(expressionNode* expr, statementNode* thenState, statementNode* elseState);
    int interpret();
    void printTo(ostream &os);
    ~ifNode();
};

class whileNode : public statementNode {
public:
    expressionNode* expression = nullptr;
    statementNode* loopBody = nullptr;

    whileNode(expressionNode* expr, statementNode* loopB);
    int interpret();
    void printTo(ostream &os);
    ~whileNode();
};

class readNode : public statementNode {
public:
    string* name = nullptr;

    readNode(string name);
    int interpret();
    void printTo(ostream &os);
    ~readNode(); 
};

// Need more classes
class writeNode : public statementNode {
public:
    string *name = nullptr;
    int tokenNumber = 0;

    writeNode(string name, int tokenNum);
    int interpret();
    void printTo(ostream &os);
    ~writeNode();
};

class expressionNode {
public:
    simpleExpressionNode *pSimpleExp1 = nullptr;
    simpleExpressionNode *pSimpleExp2 = nullptr;
    int operand;

    // an expression may contain either a single simple expression
    expressionNode(simpleExpressionNode *pSimp1);
    // or a simple expression followed by an operand and another simple expression
    expressionNode(simpleExpressionNode *pSimp1, int opCode, simpleExpressionNode *pSimp2);
    // interpret method to interpret expression
    int interpret();
    ~expressionNode();
};

class simpleExpressionNode {
public:
    termNode *pTerm = nullptr;

    simpleExpressionNode(termNode *pTerm);
    // vector to store the operands
    vector<int> restTermOps; // TOK_ADD or TOK_SUB or TOK_OR
    vector<termNode*> restTerms; // vector to store other terms
    int interpret();
    ~simpleExpressionNode();
};

class termNode {
public:
    factorNode *pFactor = nullptr;

    termNode(factorNode *pFact);
    vector<int> restFactorOps; // TOK_MULTIPLY or TOK_DIVIDE or TOK_AND
    vector<factorNode*> restFactors;
    int interpret();
    ~termNode();
};

// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
class factorNode {
public:
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual int interpret() = 0; // pure virtual method interpret
    virtual ~factorNode(){} // labeling the destructor as virtual allows 
	                       // the subclass destructors to be called
};

class intLitNode : public factorNode {
public:
    int int_literal = 0;

    intLitNode(int value);
    int interpret();
    ~intLitNode(){
        cout<<"Deleting a factorNode"<< endl; // nothing else to do as it is not a pointer
    }
    void printTo(ostream &os);
};

class floatLitNode : public factorNode {
public:
    float float_literal = 0.0;

    floatLitNode(float value);
    int interpret();
    ~floatLitNode(){
        cout<<"Deleting a factorNode"<< endl;
    }
    void printTo(ostream &os);
};

class identNode : public factorNode {
public:
    string* id = nullptr;

    identNode(string name);
    int interpret();
    ~identNode();
    void printTo(ostream &os);
};

class nestedExprNode : public factorNode {
public:
    expressionNode* pExpr = nullptr;

    nestedExprNode(expressionNode* ex);
    void printTo(ostream &os);
    int interpret();
    ~nestedExprNode();
};

class nestedFactorNode : public factorNode {
public:
    int op = 0;
    factorNode* pFac = nullptr;

    nestedFactorNode(int op, factorNode* fa);
    void printTo(ostream & os);
    int interpret();
    ~nestedFactorNode();
};

ostream& operator<<(ostream& os, factorNode& node);
ostream& operator<<(ostream& os, termNode& node);
ostream& operator<<(ostream& os, simpleExpressionNode& node);
ostream& operator<<(ostream& os, expressionNode& node);
ostream& operator<<(ostream& os, statementNode& node);
ostream& operator<<(ostream& os, blockNode& node);
ostream& operator<<(ostream& os, programNode& node);
// Add more prototypes

#endif