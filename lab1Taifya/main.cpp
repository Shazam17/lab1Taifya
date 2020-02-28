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
#include <variant>

//MARK: - FORMULA
//MARK: - z = 2y^4 + x – 2 == z = 2y + x – 2

using namespace std;

#define newLine '\n'


string ADD(string var) {
    return "ADD " + var + newLine;
}

string MPY(string var) {
    return "MPY " + var + newLine;
}

string STOREmem(string var) {
    return "STORE $" + var + newLine;
}

string LOADeq(string var) {
    return "LOAD =" + var + newLine;
}

string LOAD(string var) {
    return "LOAD " + var + newLine;
}

string LOADmem(string var) {
    return "LOAD $" + var + newLine;
}

string ADDeq(string var) {
    return "ADD =" + var + newLine;
}

string ADDmem(string var) {
    return "ADD $" + var + newLine;
}

string MPYeq(string var) {
    return "MPY =" + var + newLine;
}

enum Operator {
    equal,
    plus,
    minus,
    divide,
    multiply,
    constanta,
    var,
    pointer,
    error,
    pow
};

//MARK: - Lexema defenition
template<typename T>
struct Lexema {
    int id;
    Operator opCode;
    T value = NULL;
    string varName;
    
    Lexema<T>* leftOperand = nullptr;
    Lexema<T>* rightOperand = nullptr;
};

int getPrecedence(char op) {
    if (op == '+' || op == '-'){
        return 2;
    }
    if(op == '*' || op == '/'){
        return 3;
    }
    if(op == '^' ){
        return 4;
    }
    return 1;
}

int charToInt(char val){
    return (int)val - (int)48;
}

template<typename T>
string ToString(T val){
    stringstream ss;
    ss << val;
    return ss.str();
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
        case '^':
            return Operator::pow;
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
    if (opCode == Operator::pow ) {
        return " ^ ";
    }
    return " ";
}

//MARK: - Log Lexem tree
template<typename T>
string printLexemTree(Lexema<T>* rootLexema) {
    if (rootLexema != nullptr){
        if(rootLexema->opCode == Operator::plus || rootLexema->opCode == Operator::minus || rootLexema->opCode == Operator::multiply || rootLexema->opCode == Operator::divide || rootLexema->opCode == Operator::pow){
            return printLexemTree(rootLexema->leftOperand) + getStringOfOperator(rootLexema->opCode) + printLexemTree(rootLexema->rightOperand);
        }
        
        if (rootLexema->opCode == Operator::constanta) {
            stringstream ss;
            ss << rootLexema->value << "|id(const):" << rootLexema->id;
            return ss.str();
        }
        if (rootLexema->opCode == Operator::var) {
            stringstream ss;
            ss << rootLexema->value << "|id(var):" << rootLexema->id;
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

//MARK: - parseToLexems
//parsing formula to extract lexems
template<typename T>
Lexema<T>* parseToLexems(string formula) {
    
    vector<char> varList{ 'x' , 'y' };
    vector<char> numbers{'1','2','3','4','5','6','7','8','9','0'};
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
   
        
        if (val == '+' || val == '-'  || val == '*' || val == '/' || val == '^') {
            
            //FULL REWORKED
            //if (!opStack.empty()){
                //char top =
                //MARK: - changed > to >= TODO: - NEED TEST!!!
                   while(!opStack.empty() &&( getPrecedence(opStack.top()) >= getPrecedence(val) || val == '-' )) {
                       char op = opStack.top();
                       opStack.pop();
                       output.push(op);
                       cout << op << ' ';
                   }
                
            //}
            opStack.push(val);
        }else if (isInVector(val,varList) || isInVector(val, numbers)){
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
    int idCounter = 0;
    bool flag = true;
    while(!output.empty()){
        char operand1 = output.front();
        output.pop();
        if ( operand1 == '-' || operand1 == '+' || operand1 == '*' || operand1 == '/' || operand1 == '^'){
            
            
            //TODO: DEBUG on more than 4 vars
            char operand2 = vars.top();
            vars.pop();
            
            if(!vars.empty() && temp != NULL){
                char operand3 = vars.top();
                vars.pop();
                
                Lexema<T>* newRoot = new Lexema<T>;
                newRoot->leftOperand = temp;
                char op = output.front();
                output.pop();
                newRoot->opCode = getOperator(op);
                
                Lexema<T>* rightLeaf = new Lexema<T>();
                Lexema<T>* rightOpLexema = new Lexema<T>();
                Lexema<T>* leftOpLexem = new Lexema<T>();
                
                
                rightOpLexema->value = operand2;
                if(isInVector<char>(operand2, varList)){
                    rightOpLexema->opCode = Operator::var;
                }else {
                    rightOpLexema->opCode = Operator::constanta;
                }
                leftOpLexem->value = operand3;
                if(isInVector<char>(operand3, varList)){
                    leftOpLexem->opCode = Operator::var;
                }else {
                    leftOpLexem->opCode = Operator::constanta;
                }
                rightLeaf->leftOperand = leftOpLexem;
                rightLeaf->rightOperand = rightOpLexema;
                
                newRoot->rightOperand = rightLeaf;
                newRoot->rightOperand->opCode = getOperator(operand1);
                 
                }
            
            }else{
                if (temp == NULL) {
                    //if there is not root -> create new root
                    char operand3;
                    if(!vars.empty() ){
                        operand3 = vars.top();
                        vars.pop();
                    }
                    temp = new Lexema<T>();
                    
                    //setup type of operation
                    temp->opCode = getOperator(operand1);
                    
                    //setup left operand
                    temp->leftOperand = new Lexema<T>();
                    if(isInVector<char>(operand2, varList)){
                        temp->leftOperand->opCode = Operator::var;
                    }else {
                        temp->leftOperand->opCode = Operator::constanta;
                    }
                    
                    temp->leftOperand->value = operand2;
                    temp->leftOperand->id = idCounter;
                    idCounter++;
                    
                    //setup right operand
                    temp->rightOperand = new Lexema<T>();
                    if(isInVector<char>(operand3, varList)){
                        temp->rightOperand->opCode = Operator::var;
                    }else {
                        temp->rightOperand->opCode = Operator::constanta;
                    }
                    
                    temp->rightOperand->value = operand3;
                    temp->rightOperand->id = idCounter;
                    idCounter++;
                    flag = vars.empty();
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
                    if(isInVector(operand2, varList)){
                        temp->rightOperand->opCode = Operator::var;
                    }else {
                        temp->rightOperand->opCode = Operator::constanta;
                    }
                    temp->rightOperand->id = idCounter;
                    temp->rightOperand->value = operand2;
                    idCounter++;
                }
            }
            
            
        }else {
            vars.push(operand1);
        }
    }
    
    //log initial expression
    cout << printLexemTree(temp) << endl;
    return temp;
}

template<typename T>
string extractValue(Lexema<T>* lexema) {
    stringstream ss;
    ss << lexema->value;
    return ss.str();
}

template<typename T>
string extractId(Lexema<T>* lexema) {
    stringstream ss;
    ss << '$'<< lexema->id;
    return ss.str();
}

template<typename T>
string getAssemblyOperator(Operator opCode , Lexema<T>* lexema,int id){
    switch (opCode) {
        case Operator::plus:
            return ADD('$' + ToString(id));
            break;
        case Operator::multiply:
            return MPY('$' + ToString(id));
            break;
        case Operator::pow:
            stringstream ss;
            ss << lexema->value;
            int power;
            ss >> power;
            ss.clear();
            for(int i = 0; i < power; i++){
                ss << MPY('$' + ToString(id));
            }
            return ss.str();
            break;
       
    }
    return "";
}

//convert binary lexem tree to abstract assembly language
template<typename T>
string lexemsToAsm(Lexema<T>* rootLexema) {
    static int memCounter = 0;
    static string lastParesed = "";
    
    if (rootLexema != nullptr){
           if(rootLexema->opCode == Operator::plus || rootLexema->opCode == Operator::minus || rootLexema->opCode == Operator::multiply || rootLexema->opCode == Operator::divide || rootLexema->opCode == Operator::pow ){
//                printLexemTree(rootLexema->leftOperand) + getStringOfOperator(rootLexema->opCode) + printLexemTree(rootLexema->rightOperand);
               
               string ret1 = lexemsToAsm(rootLexema->leftOperand);
               string ret2 = lexemsToAsm(rootLexema->rightOperand);
               if(ret1.size() == 0 && ret2.size() == 0){
                   string retRes = extractValue(rootLexema->leftOperand) + '|' + extractValue(rootLexema->rightOperand);
                   cout << retRes << endl;
                   string resultAssemblyCode = "";
                   resultAssemblyCode += LOAD(extractValue(rootLexema->rightOperand));
                   //resultAssemblyCode += STOREmem(extractId(rootLexema->rightOperand));
                   resultAssemblyCode += STOREmem(ToString<int>(memCounter));
                   resultAssemblyCode += LOAD(extractValue(rootLexema->leftOperand));
                   
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->rightOperand,memCounter);
                   lastParesed = resultAssemblyCode;
                   return resultAssemblyCode;
               }
               if(ret1.size() == 0 && ret2.size() != 0) {
                   string resultAssemblyCode = lastParesed;
//                   resultAssemblyCode += LOAD(extractValue(rootLexema->leftOperand));
//                   resultAssemblyCode += STOREmem(extractId(rootLexema->leftOperand));
//                   resultAssemblyCode += LOADmem(extractId(rootLexema->rightOperand));
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->leftOperand,memCounter);
                   return resultAssemblyCode ;
               }
               if(ret1.size() != 0 && ret2.size() == 0) {
                   
                   //string resultAssemblyCode = ret1;
                   string resultAssemblyCode = "";
                   memCounter++;
                   resultAssemblyCode += LOAD(extractValue(rootLexema->rightOperand));
                   //resultAssemblyCode += STOREmem(extractId(rootLexema->rightOperand));
                   resultAssemblyCode += STOREmem(ToString<int>(memCounter));
                   
                   resultAssemblyCode += lastParesed;
                    
                   //resultAssemblyCode += LOADmem(extractId(rootLexema->leftOperand));
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->rightOperand,memCounter);
                   return resultAssemblyCode;
               }
               
           }
//
//           if (rootLexema->opCode == Operator::constanta) {
//               stringstream ss;
//               ss << rootLexema->value << "|id:" << rootLexema->id;
//               return ss.str();
//           }
       }
        return "";
}




int main(int argc, const char * argv[]) {
    
    //input formula
    string formula;
    cout << "Enter formula: ";
    getline(cin , formula);
    Lexema<char>* rootLexema = parseToLexems<char>(formula);
    cout << lexemsToAsm<char>(rootLexema) << endl;
    
    return 0;
}
