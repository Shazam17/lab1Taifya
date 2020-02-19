//
//  main.cpp
//  lab1Taifya
//
//  Created by Николай Костин on 17.02.2020.
//  Copyright © 2020 Николай Костин. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <sstream>
#include <algorithm>


//MARK: - FORMULA
//MARK: - z = 2y^4 + x – 2 == z = 2y + x – 2

using namespace std;

enum Operator {
    equal,
    plus,
    minus,
    divide,
    multiply,
    constanta,
    pointer,
    error
};

template<typename T>
struct Lexema {
    Operator opCode;
    T value = NULL;
    string varName;
    
    Lexema<T>* leftOperand = nullptr;
    Lexema<T>* rightOperand = nullptr;
};

int getPrecedence(char op) {
    if (op == '+' || op == '-'){
        return 1;
    }
    if(op == '*' || op == '/' ){
        return 2;
    }
    return 0;
}

int charToInt(char val){
    return (int)val - (int)48;
}

Operator getOperator(char val) {
        switch (val) {
        case '+':
           return Operator::plus;
            break;
        case '-':
            return Operator::minus;
            break;
        case '*':
            return Operator::multiply;
            break;
        case '/':
            return Operator::divide;
            break;
        default:
            return Operator::error;
            break;
    }
}

string getStringOfOperator(Operator opCode){
    if (opCode == Operator::plus ) {
        return " + ";
    }
    if (opCode == Operator::minus ) {
        return " - ";
    }
    if (opCode == Operator::multiply ) {
        return " * ";
    }
    if (opCode == Operator::divide ) {
        return " / ";
    }
    return " ";
}

template<typename T>
string printLexemTree(Lexema<T>* rootLexema) {
    if (rootLexema != nullptr){
        if(rootLexema->opCode == Operator::plus || rootLexema->opCode == Operator::minus || rootLexema->opCode == Operator::multiply || rootLexema->opCode == Operator::divide ){
            return printLexemTree(rootLexema->leftOperand) + getStringOfOperator(rootLexema->opCode) + printLexemTree(rootLexema->rightOperand);
        }
        
        if (rootLexema->opCode == Operator::constanta) {
            stringstream ss;
            ss << rootLexema->value;
            return ss.str();
        }
    }
     return " ";
}

template<typename T>
bool isInVector(T symbol, vector<T> v) {
    if (find(v.begin(),v.end(), symbol) != v.end()){
        return true;
    }else {
        return false;
    }
}

//parsing formula to extract lexems
template<typename T>
void parseToLexems(string formula) {
    
    vector<char> varList{ 'x' , 'y' };
    queue<char> input;
    queue<char> output;
    stack<char> opStack;
    
    //copy formula to input queue
    for (auto val : formula) {
        input.push(val);
    }
    
    while(!input.empty()){
        char val = input.front();
        cout << val << endl;
        input.pop();
   
        
        if (val == '+' || val == '-'  || val == '*' || val == '/') {
            if (!opStack.empty()){
                char top = opStack.top();
                if((getPrecedence(top) > getPrecedence(val) || val == '-')) {
                   while(!opStack.empty()){
                       char op = opStack.top();
                       opStack.pop();
                       output.push(op);
                       cout << op << ' ';
                   }
                }
            }
            opStack.push(val);
        }else if (isInVector(val,varList)){
            output.push(val);
        }
    }
    
    //load last operators to the RPN string
    while(!opStack.empty()) {
        char tVal = opStack.top();
        opStack.pop();
        output.push(tVal);
    }
    
    //print output RPN string to the console
    queue<T> tStack = output;
    while(!tStack.empty()){
        char val = tStack.front();
        tStack.pop();
        cout << val << ' ';
    }
    cout << endl;
    
    stack<T> vars;
    //TODO: - make re-asignin temp to root as right leaf , left leaft is some variable
    Lexema<T>* rootLexema = new Lexema<T>();
    
    
    //z equals our expression
//    rootLexema->opCode = Operator::equal;
//    rootLexema->leftOperand = new Lexema<int>();
//    rootLexema->leftOperand->opCode = Operator::constanta;
//    rootLexema->leftOperand->varName = "z";
    
    Lexema<T>* temp = NULL; // = rootLexema;
    
    while(!output.empty()){
        char operand1 = output.front();
        output.pop();
        if ( operand1 == '-' || operand1 == '+' || operand1 == '*' || operand1 == '/'){
            
            //TODO: DEBUG on more than 4 vars
            char operand2 = vars.top();
            vars.pop();
            char operand3;
            if(!vars.empty()){
                operand3 = vars.top();
                vars.pop();
            }
            
            if (temp == NULL) {
                //if there is not root -> create new root
                temp = new Lexema<T>();
                
                //setup type of operation
                temp->opCode = getOperator(operand1);
                
                //setup left operand
                temp->leftOperand = new Lexema<T>();
                temp->leftOperand->opCode = Operator::constanta;
                temp->leftOperand->value = operand2;
                
                //setup right operand
                temp->rightOperand = new Lexema<T>();
                temp->rightOperand->opCode = Operator::constanta;
                temp->rightOperand->value = operand3;
            }else{
                //root exists, so we attach tempLeaf to new root and add right leaf
                
                //temp leaf is left now, so we copy pointer
                Lexema<T>* leftLeaf = temp;
                
                //alocating memory for new root
                temp = new Lexema<T>();
                
                //place old temp to left leaf
                temp->leftOperand = leftLeaf;
                
                //setup type of operation
                temp->opCode = getOperator(operand1);
                
                //setup right operand
                temp->rightOperand = new Lexema<T>();
                temp->rightOperand->opCode = Operator::constanta;
                temp->rightOperand->value = operand2;
                
            }
        }else {
            vars.push(operand1);
        }
    }
    
    //log initial expression
    cout << printLexemTree(temp) << endl;
}

template<typename T>
string lexemsToAsm(vector<Lexema<T>> lexems) {

}




int main(int argc, const char * argv[]) {
    
    //input formula
    string formula;
    cout << "Enter formula: ";
    getline(cin , formula);
    parseToLexems<char>(formula);
    
    
    return 0;
}
