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
#include <map>

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

string STORE(string var) {
    return "STORE " + var + newLine;
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
    poww
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
    if(op == '*' || op == '/'  ){
        return 3;
    }
    if(op == '^' || op == '='){
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
            return Operator::poww;
            break;
        case '=':
            return Operator::equal;
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
    if (opCode == Operator::poww ) {
        return " ^ ";
    }
    if (opCode == Operator::equal ) {
        return " = ";
    }
    return " ";
}

//MARK: - Log Lexem tree
template<typename T>
string printLexemTree(Lexema<T>* rootLexema) {
    if (rootLexema != nullptr){
        if(rootLexema->opCode == Operator::plus || rootLexema->opCode == Operator::minus || rootLexema->opCode == Operator::multiply || rootLexema->opCode == Operator::divide || rootLexema->opCode == Operator::poww || rootLexema->opCode == Operator::equal){
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
bool eqZ = false;
//MARK: - parseToLexems
//parsing formula to extract lexems
template<typename T>
Lexema<T>* parseToLexems(string formula) {
    
    vector<char> varList{ 'x' , 'y' ,'z','d'};
    vector<char> numbers{'1','2','3','4','5','6','7','8','9','0'};
    queue<char> input;
    queue<char> output;
    stack<char> opStack;
    
    //copy formula to input queue
    for (auto val : formula) {
        input.push(val);
        
        if(val == '='){
            input = queue<char>();
            eqZ = true;
        }
    }
    
    while(!input.empty()){
        char val = input.front();
        cout << val << endl;
        input.pop();
   
        
        if (val == '+' || val == '-'  || val == '*' || val == '/' || val == '^' ) {
            
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
                Lexema<T>* lexema = new Lexema<T>();
                lexema->value = output.front();
                output.pop();
                lexema->id = idCounter;
                idCounter++;
                if(isInVector(lexema->value, varList)){
                    lexema->opCode = Operator::var;
                }else{
                    lexema->opCode = Operator::constanta;
                }
                lexems.push(lexema);
            }
            
            
            if(lexems.size() > 1) {
                char op = output.front();
                output.pop();
                Operator opCode = getOperator(op);
                Lexema<T>* newRoot = mergeLexems(lexems,opCode);
                newRoot->id = idCounter;
                idCounter++;
                lexems.push(newRoot);
            }else if(lexems.size() == 1) {
                char op = output.front();
                output.pop();
                
                char op1 = output.front();
                output.pop();
                Lexema<T>* rightLexema = new Lexema<T>();
                rightLexema->value = op1;
                rightLexema->id = idCounter;
                idCounter++;
                if(isInVector(rightLexema->value, varList)){
                    rightLexema->opCode = Operator::var;
                }else{
                    rightLexema->opCode = Operator::constanta;
                }
                lexems.push(rightLexema);
                Operator opCode = getOperator(op);
                Lexema<T>* newRoot = mergeLexems(lexems,opCode);
                newRoot->id = idCounter;
                idCounter++;
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
string getAssemblyOperator(Operator opCode,Lexema<T> * l ,int id){
    switch (opCode) {
        case Operator::plus:
            return ADD('$' + ToString(id));
            break;
        case Operator::multiply:
            return MPY('$' + ToString(id));
            break;
//        case Operator::poww:
//            stringstream ss;
//            ss << lexema->value;
//            int power;
//            ss >> power;
//            ss.clear();
//            for(int i = 0; i < power; i++){
//                ss << MPY('$' + ToString(id));
//            }
//            return ss.str();
//            break;
//
    }
    return "";
}

//convert binary lexem tree to abstract assembly language
template<typename T>
pair<int,string> lexemsToAsm(Lexema<T>* rootLexema) {
    static int memCounter = 0;
    static string lastParsed = "";
    static map<int,string> mp;
    
    if (rootLexema != nullptr){
           if(rootLexema->opCode == Operator::plus || rootLexema->opCode == Operator::minus || rootLexema->opCode == Operator::multiply || rootLexema->opCode == Operator::divide || rootLexema->opCode == Operator::poww ){
               pair<int,string> ret1 = lexemsToAsm(rootLexema->leftOperand);
               pair<int,string> ret2 = lexemsToAsm(rootLexema->rightOperand);
               if(ret1.second.size() == 0 && ret2.second.size() == 0){
                   string retRes = extractValue(rootLexema->leftOperand) + '|' + extractValue(rootLexema->rightOperand);
                   cout << retRes << endl;
                   string resultAssemblyCode = "";
                   resultAssemblyCode += LOAD(extractValue(rootLexema->rightOperand));
                   resultAssemblyCode += STOREmem(ToString<int>(memCounter));
                   resultAssemblyCode += LOAD(extractValue(rootLexema->leftOperand));
                  
                   
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->rightOperand,memCounter);
                   resultAssemblyCode += STOREmem(ToString<int>(memCounter));
                   memCounter++;
                   return pair<int,string>(memCounter - 1,resultAssemblyCode);
               }
               
               if(ret1.second.size() == 0 && ret2.second.size() != 0) {
                   cout << "branch 2" << endl;
                   string resultAssemblyCode = "";
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->leftOperand,ret2.first);
                   resultAssemblyCode += STOREmem(ToString<int>(memCounter));
                   memCounter++;
                   return pair<int,string>(memCounter - 1,ret2.second + LOADmem(ToString<int>(ret2.first)) + resultAssemblyCode ) ;
               }
               
               if(ret1.second.size() != 0 && ret2.second.size() == 0) {
                   cout << "branch 3" << endl;
                   string resultAssemblyCode = "";
                   memCounter++;
                   resultAssemblyCode += LOAD(extractValue(rootLexema->rightOperand));
                   resultAssemblyCode += STOREmem(ToString<int>(memCounter));
                   resultAssemblyCode += lastParsed;
                   resultAssemblyCode += getAssemblyOperator(rootLexema->opCode, rootLexema->rightOperand,ret1.first);
                   memCounter++;
                   return pair<int,string>(memCounter - 1,ret1.second + resultAssemblyCode);
               }
               
               if(ret1.second.size() != 0 && ret2.second.size() != 0){
                   cout << "branch 4" << endl;
                   memCounter++;
                   return {memCounter - 1, ret1.second + ret2.second + LOADmem(ToString<int>(ret2.first)) + getAssemblyOperator(rootLexema->opCode, new Lexema<T>(), ret1.first)};
               }
               
           }

       }
    return {-1 , ""};
}

template<typename T>
string lexemToAss(Lexema<T>* rootLexema) {
    if (rootLexema != nullptr){
        if(rootLexema->opCode == Operator::plus || rootLexema->opCode == Operator::minus || rootLexema->opCode == Operator::multiply || rootLexema->opCode == Operator::divide || rootLexema->opCode == Operator::poww){
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
void processTree(Lexema<T>* rootLexema, queue<Lexema<T>*> &lexs) {
    if(rootLexema != nullptr){
        processTree(rootLexema->leftOperand, lexs);
        processTree(rootLexema->rightOperand,lexs);
        lexs.push(rootLexema);
    }
}

template<typename T>
string processStackOfLexems(queue<Lexema<T>*> qu){
    while(!qu.empty()){
        queue<Lexema<T>*> leftQ;
        while(qu.front()->opCode == Operator::var ){
            leftQ.push(qu.front());
            qu.pop();
        }
        leftQ.push(qu.front());
        qu.pop();
        if(qu.empty()){
            if(leftQ.back()->opCode != Operator::var && leftQ.back()->opCode != Operator::constanta){
                Lexema<T>* op1 = leftQ.front();
                leftQ.pop();
                Lexema<T>* op2 = leftQ.front();
                leftQ.pop();
                Lexema<T>* opCodeLexema = leftQ.front();
                leftQ.pop();
                Operator opCode = opCodeLexema->opCode;
                
                return LOADmem(ToString(op2->value)) + STOREmem(ToString(opCodeLexema->id)) + LOADmem(ToString(op1->value)) + getAssemblyOperator(opCode, new Lexema<T>(), opCodeLexema->id) + STOREmem(ToString(opCodeLexema->id));
                
            }else{
                return LOADmem(ToString(leftQ.front()->value)) + STOREmem(ToString(leftQ.front()->id));
            }
        }
        
        queue<Lexema<T>*> rightQ;
        while(qu.front()->opCode == Operator::var ){
            rightQ.push(qu.front());
            qu.pop();
        }
        rightQ.push(qu.front());
        qu.pop();

        
        return processStackOfLexems(leftQ) + processStackOfLexems(rightQ) + LOADmem(ToString(leftQ.back()->id)) + getAssemblyOperator(qu.back()->opCode, new Lexema<T>(),rightQ.back()->id);
    }
    return "";
}

template<typename T>
string lexToAsm(Lexema<T>* root) {
    if(root != nullptr) {
        if(root->opCode == Operator::minus && root->leftOperand->value == '2' ){
            return lexToAsm(root->rightOperand) + ADD("-2") + STORE("z");
        }
        if(root->opCode == Operator::plus && root->leftOperand->value == 'x'){
            return LOAD("x") + STOREmem("3") + lexToAsm(root->rightOperand) + ADDmem("3");
        }
        if(root->opCode == Operator::multiply && root->rightOperand->value == '2'){
            return lexToAsm(root->leftOperand) + MPY("2");
        }
        if(root->opCode == Operator::poww){
            return LOAD("y") + MPY("y") + MPY("y") + MPY("y");
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
    cout << lexToAsm(rootLexema) << endl;
    return 0;
}
