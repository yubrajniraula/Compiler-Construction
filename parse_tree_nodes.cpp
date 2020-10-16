#include "parse_tree_nodes.h"

string token_to_string(int token) {
    switch(token){
        case TOK_PLUS:
            return " + ";       
        case TOK_MINUS:
            return " - ";
        case TOK_MULTIPLY:
            return " * ";
        case TOK_DIVIDE:
            return " / ";
        case TOK_ASSIGN:
            return " := ";
        case TOK_EQUALTO:
            return " = ";
        case TOK_LESSTHAN:
            return " < ";
        case TOK_GREATERTHAN:
            return " > ";
        case TOK_NOTEQUALTO:
            return " <> ";
        case TOK_MOD:
            return " MOD ";
        case TOK_NOT:
            return " NOT ";
        case TOK_OR:
            return " OR ";
        case TOK_AND:
            return " AND ";
        default:
            return " UNKNOWN TOKEN ";
    }
}

programNode::programNode(string id, blockNode *blk){
    name = new string(id);
    block = blk;
}

programNode::~programNode(){
    cout << "Deleting a programNode" << endl;
    delete name;
    name = nullptr;
    delete block;
    block = nullptr;
}

blockNode::blockNode(compoundNode *cpd){
    this->compoundStatement = cpd;
}

blockNode::~blockNode() {
	cout << "Deleting a blockNode" << endl;
	delete compoundStatement;
	compoundStatement = nullptr;
}

assignmentNode::assignmentNode(string name, expressionNode* expr) {
    this->name = new string(name);
    this->exprs = expr;
}

assignmentNode::~assignmentNode() {
    cout << "Deleting an assignmentNode" << endl;
	delete exprs;
	exprs = nullptr;
}

compoundNode::compoundNode(statementNode *stat) {
    this->mystate = stat;
}

compoundNode::~compoundNode() {
    cout<< "Deleting a compoundNode" << endl;
    delete mystate;
    mystate = nullptr;

    int length = restStatements.size();
	for (int i = 0; i < length; ++i) {
		delete restStatements[i];
		restStatements[i] = nullptr;
	}
}

ifNode::ifNode(expressionNode *expr, statementNode *thenState) {
    this->expression = expr;
    this->thenStatement = thenState;
}

ifNode::ifNode(expressionNode *expr, statementNode *thenState, statementNode *elseState) {
    this->expression = expr;
    this->thenStatement = thenState;
    this->elseStatement = elseState;
}

ifNode::~ifNode(){
    cout << "Deleting an ifNode: " << endl;
	delete expression;
	expression = nullptr;
    delete thenStatement;
    thenStatement = nullptr;
    if (elseStatement != nullptr) {
        delete elseStatement;
        elseStatement = nullptr;
    }
}

whileNode::whileNode(expressionNode* expr, statementNode *loopB) {
    this->expression = expr;
    this->loopBody = loopB;

}

whileNode::~whileNode(){
    delete expression;
    expression = nullptr;
    delete loopBody;
    loopBody = nullptr;
}

readNode::readNode(string name) {
    this->name = new string(name);
}

readNode::~readNode() {
    cout<< "Deleting a readNode"<<endl;
    delete name;
    name = nullptr;
}

writeNode::writeNode(string name, int tokenNum) {
    this->name = new string(name);
    this->tokenNumber = tokenNum;
}

writeNode::~writeNode() {
    cout<<"Deleting a writeNode"<< endl;
    delete name;
    name = nullptr;
}

expressionNode::expressionNode(simpleExpressionNode *pSimp) {
    this->pSimpleExp = pSimp;
}

expressionNode::expressionNode(simpleExpressionNode *pSimp, int opCode) {
    this->pSimpleExp = pSimp;
    this->operand = opCode;
}

expressionNode::~expressionNode(){
    cout<<"Deleting an expressionNode"<< endl;
    delete pSimpleExp;
    pSimpleExp = nullptr;
}

simpleExpressionNode::simpleExpressionNode(termNode *pTerm) {
    this->pTerm = pTerm;
}

simpleExpressionNode::~simpleExpressionNode(){
    cout<<"Deleting a simpleExpressionNode"<< endl;
    delete pTerm;
    pTerm = nullptr;

    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        delete restTerms[i];
        restTerms[i] = nullptr;
    }
}

termNode::termNode(factorNode *pFact) {
    this->pFactor = pFact;
}

termNode::~termNode() {
    cout<<"Deleting a termNode"<< endl;
    delete pFactor;
    pFactor = nullptr;

    int length = restFactorOps.size();
    for (int i = 0; i < length; ++i) {
        delete restFactors[i];
        restFactors[i] = nullptr;
    }
}

intLitNode::intLitNode(int value) {
    int_literal = value;
}

floatLitNode::floatLitNode(float value) {
    float_literal = value;
}

identNode::identNode(string name) {
    id = new string(name);
}

identNode::~identNode(){
    cout<<"Deleting a factorNode"<< endl;
    delete id;
    id = nullptr;
}

nestedExprNode::nestedExprNode(expressionNode* ex) {
	pExpr = ex;
}

nestedFactorNode::nestedFactorNode(factorNode* fa) {
	pFac = fa;
}

ostream& operator<<(ostream& os, factorNode& node){
    os<<"factor( ";
    node.printTo(os);  
    os<<" )";
    return os;
}

ostream& operator<<(ostream& os, termNode& node){
    os<<"term( ";
    os<< *(node.pFactor);
    os<<" )";

    int length = node.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = node.restFactorOps[i];
		if (op == TOK_MULTIPLY)
			os << " * ";
		else if(op == TOK_DIVIDE)
			os << " / ";
        else os << " AND ";
	os << *(node.restFactors[i]);
	}
    return os;
}

ostream& operator<<(ostream& os, simpleExpressionNode& node){
    os<<"simple_expression( ";
    os<< *(node.pTerm);
    os<<" )";

    int length = node.restTermOps.size();
	for (int i = 0; i < length; ++i) {
		int op = node.restTermOps[i];
		if (op == TOK_PLUS)
			os << " + ";
		else if(op == TOK_MINUS)
			os << " - ";
        else os << " 0R ";
		os << *(node.restTerms[i]);
	}
    return os;
}

ostream& operator<<(ostream& os, expressionNode& node){
    // os<<"expression( ";
    os<< *(node.pSimpleExp);

    if (node.operand == TOK_EQUALTO)
		os << " = ";
    else if(node.operand == TOK_LESSTHAN)
        os << " < ";
    else if(node.operand == TOK_GREATERTHAN)
        os << " > ";
    else if(node.operand == TOK_NOTEQUALTO)
        os << " <> ";
    os<<" )";
    return os;
}

ostream& operator<<(ostream& os, compoundNode& node){  
    os<<"Begin Compound Statement"<<endl;
    os << *(node.mystate);

    int length = node.restStatements.size();
	for (int i = 0; i < length; ++i) {
		statementNode* op = node.restStatements[i];
        os<<*op<<endl;
	}
    os <<"End Compound Statement"<< endl;
    return os;
}

ostream& operator<<(ostream& os, statementNode& node){
    node.printTo(os);
    // may need if statements
    // vector
    // stuck here. ya dekhi gayena
    return os;
}

ostream& operator<<(ostream& os, blockNode& node){  
    os << *(node.compoundStatement);
    return os;
}

ostream& operator<<(ostream& os, programNode& node){
    os << "Program Name: " << *(node.name) << endl;
    os << *(node.block) << endl;
    return os;
}

void assignmentNode::printTo(ostream &os) {
    os<<"Assignment " << *name <<" := expression(";
    os<<*exprs;
}

void compoundNode::printTo(ostream &os) {
    os<<"Begin Compound Statement"<<endl;
    os << *mystate;

    int length = restStatements.size();
	for (int i = 0; i < length; ++i) {
		statementNode* op = restStatements[i];
        os<<*op<<endl;
	}
    os <<"End Compound Statement"<< endl;
}

void ifNode::printTo(ostream &os) {
    os<<"If expression( ";
    os<<*expression;
    os<<*thenStatement;
    if (elseStatement != nullptr) os<<*elseStatement;
}

void whileNode::printTo(ostream &os) {
    os<<"While expression( ";
    os<<*expression;
    os << *loopBody;
}

void readNode::printTo(ostream &os) {
    os<<"Read Value " << *name<<endl;
}

void writeNode::printTo(ostream &os) {
    if (tokenNumber == TOK_STRINGLIT) os<<"Write String " << *name;
    else os << "Write Value "<< *name; // else it is a symbol/identifier
}

void intLitNode::printTo(ostream &os) {
    os<< int_literal;
}

void floatLitNode::printTo(ostream &os) {
    os<< float_literal;
}

void identNode::printTo(ostream &os) {
    os<< *id;
}

void nestedExprNode::printTo(ostream &os) {
    os<< *pExpr;
}

void nestedFactorNode::printTo(ostream &os) {
    os<< *pFac;
}

