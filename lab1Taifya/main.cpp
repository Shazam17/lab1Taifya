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

template<typename T>
Lexema<T>* mergeLexems(stack<Lexema<T>*> &lexems, Operator opCode){
    Lexema<T>* retLexema = new Lexema<T>();
    auto op1 = lexems.top();
    lexems.pop();
    auto op2 = lexems.top();
    lexems.pop();
    retLexema->opCode = opCode;
    retLexema->leftOperand = op1;
    retLexema->rightOperand = op2;
    return retLexema;
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
    stack<Lexema<T>*> lexems;
    while(!output.empty()) {
        //push until operator
        //cout << output.front() << endl;
        if(!output.empty()){
            while(!output.empty() && getPrecedence(output.front()) == 1) {
                cout << "lexem created" << endl;
                Lexema<T>* lexema = new Lexema<T>();
                lexema->value = output.front();
                output.pop();
                if(isInVector(lexema->value, varList)){
                    lexema->opCode = Operator::var;
                }else{
                    lexema->opCode = Operator::constanta;
                }
                lexems.push(lexema);
            }
            
            
            if(lexems.size() > 1) {
                cout << "two lexems branch" << endl;
                char op = output.front();
                output.pop();
                Operator opCode = getOperator(op);
                Lexema<T>* newRoot = mergeLexems(lexems,opCode);
                lexems.push(newRoot);
            }else if(lexems.size() == 1) {
                cout << "one lexems branch" << endl;
                char op = output.front();
                output.pop();
                
                char op1 = output.front();
                output.pop();
                Lexema<T>* rightLexema = new Lexema<T>();
                rightLexema->value = op1;
                if(isInVector(rightLexema->value, varList)){
                    rightLexema->opCode = Operator::var;
                }else{
                    rightLexema->opCode = Operator::constanta;
                }
                lexems.push(rightLexema);
                Operator opCode = getOperator(op);
                Lexema<T>* newRoot = mergeLexems(lexems,opCode);
                lexems.push(newRoot);
            }
        }

    }
    
    Lexema<T>* lexema = lexems.top();
    //log initial expression
    cout << printLexemTree(lexema) << endl;
    return lexema;
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
               string ret1 = lexemsToAsm(rootLexema->leftOperand);
               string ret2 = lexemsToAsm(rootLexema->rightOperand);
               if(ret1.size() == 0 && ret2.size() == 0){
                   string retRes = extractValue(rootLexema->leftOperand) + '|' + extractValue(rootLexema->rightOperand);
                   cout << retRes << endl;
                   string resultAssemblyCode = "";
                   resultAssemblyCode += LOAD(extractValue(rootLexema->rightOperand));
                   resultAssemblyCode += STOREmem(ToString<int>(memCounter));
                   resultAssemblyCode += LOAD(extractValue(rootLexema->leftOperand));
                   
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->rightOperand,memCounter);
                   lastParesed = resultAssemblyCode;
                   return resultAssemblyCode;
               }
               if(ret1.size() == 0 && ret2.size() != 0) {
                   string resultAssemblyCode = lastParesed;
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->leftOperand,memCounter);
                   return resultAssemblyCode ;
               }
               if(ret1.size() != 0 && ret2.size() == 0) {
                   
                   string resultAssemblyCode = "";
                   memCounter++;
                   resultAssemblyCode += LOAD(extractValue(rootLexema->rightOperand));
                   resultAssemblyCode += STOREmem(ToString<int>(memCounter));
                   resultAssemblyCode += lastParesed;
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->rightOperand,memCounter);
                   return resultAssemblyCode;
               }
               
           }

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
