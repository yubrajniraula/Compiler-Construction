/******************************************************************* 
Your file header information....

*******************************************************************/

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

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

class programNode{
public:
    string* name = nullptr;
    blockNode* block = nullptr;

    programNode(string id, blockNode* blk);
    ~programNode();

    // you may modify or change these classes....
};

class blockNode{
public:
    compoundNode* compoundStatement = nullptr;

    // add methods
    blockNode(compoundNode* cpd);
    ~blockNode();
};

// Abstract class. Base class for assignmentNode, compoundNode, ifNode, whileNode,
//                    readNode, writeNode
//
// Remove if not using inheritance to model the statement hierarchy
class statementNode{
public:
    // statementNode *pState;
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    // statementNode(statementNode *pState);
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
    void printTo(ostream &os);
    ~assignmentNode();
};


class compoundNode : public statementNode {
public:
    statementNode *mystate = nullptr;

    compoundNode(statementNode* mystate);
    vector <statementNode*> restStatements; // vector to store the rest of the statements of the compound statement
    void printTo(ostream &os);
    ~compoundNode();
};


class ifNode : public statementNode {
public:
    expressionNode* expression = nullptr;
    statementNode* thenStatement = nullptr;
    statementNode* elseStatement = nullptr;

    // add methods
    //ifNode(expressionNode* expr);
    ifNode(expressionNode* expr, statementNode* thenState);
    ifNode(expressionNode* expr, statementNode* thenState, statementNode* elseState);
    //ifNode(statementNode* thenElseState);
    void printTo(ostream &os);
    ~ifNode();
};

class whileNode : public statementNode {
public:
    expressionNode* expression = nullptr;
    statementNode* loopBody = nullptr;
   //whileNode* pHolder = nullptr;

    // add methods  
    whileNode(expressionNode* expr, statementNode* loopB);
    //whileNode(expressionNode* expr);
    //whileNode(whileNode *expr, statementNode* loopB);
    void printTo(ostream &os);
    ~whileNode();
};

class readNode : public statementNode {
public:
    string* name = nullptr;

    // add methods
    readNode(string name);
    void printTo(ostream &os);
    ~readNode(); 
};

// Need more classes
class writeNode : public statementNode {
public:
    string *name = nullptr;
    int tokenNumber = 0;
   // string *strLit = nullptr;

    writeNode(string name, int tokenNum);
    void printTo(ostream &os);
    ~writeNode();
};

class expressionNode {
public:
    simpleExpressionNode *pSimpleExp = nullptr;
    int operand;

    expressionNode(simpleExpressionNode *pSimp);
    expressionNode(simpleExpressionNode *pSimp, int opCode);
    // vector<int> restExpOps; // TOK_EQUALTO or TOK_LESSTHAN or TOK_GREATERTHAN or TOK_NOTEQUALTO
    ~expressionNode();
};

class simpleExpressionNode {
public:
    termNode *pTerm = nullptr;

    simpleExpressionNode(termNode *pTerm);
    vector<int> restTermOps; // TOK_ADD or TOK_SUB or TOK_OR
    vector<termNode*> restTerms;
    ~simpleExpressionNode();
};

class termNode {
public:
    factorNode *pFactor = nullptr;

    termNode(factorNode *pFact);
    vector<int> restFactorOps; // TOK_MULTIPLY or TOK_DIVIDE or TOK_AND
    vector<factorNode*> restFactors;
    ~termNode();
};

// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
class factorNode {
public:
    virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
    virtual ~factorNode(){} // labeling the destructor as virtual allows 
	                       // the subclass destructors to be called
};

class intLitNode : public factorNode {
public:
    int int_literal = 0;

    intLitNode(int value);
    ~intLitNode(){
        cout<<"Deleting a factorNode"<< endl; // nothing else to do as it is not a pointer
    }
    void printTo(ostream &os);
};

class floatLitNode : public factorNode {
public:
    float float_literal = 0.0;

    floatLitNode(float value);
    ~floatLitNode(){
        cout<<"Deleting a factorNode"<< endl;
    }
    void printTo(ostream &os);
};

class identNode : public factorNode {
public:
    string* id = nullptr;

    identNode(string name);
    ~identNode();
    void printTo(ostream &os);
};

class nestedExprNode : public factorNode {
public:
    expressionNode* pExpr = nullptr;

    nestedExprNode(expressionNode* ex);
    void printTo(ostream &os);
    ~nestedExprNode(){}
};

class nestedFactorNode : public factorNode {
public:
    factorNode* pFac = nullptr;

    nestedFactorNode(factorNode* fa);
    void printTo(ostream & os);
    ~nestedFactorNode(){}
};

ostream& operator<<(ostream& os, factorNode& node);
ostream& operator<<(ostream& os, termNode& node);
ostream& operator<<(ostream& os, simpleExpressionNode& node);
ostream& operator<<(ostream& os, expressionNode& node);
ostream& operator<<(ostream& os, compoundNode& node);
ostream& operator<<(ostream& os, statementNode& node);
ostream& operator<<(ostream& os, blockNode& node);
ostream& operator<<(ostream& os, programNode& node);
// Add more prototypes

#endif