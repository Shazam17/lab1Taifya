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
    Operator opCode = Operator::constanta;
    T value;
    string varName = "";
    
    Lexema<T>* leftOperand = nullptr;
    Lexema<T>* rightOperand = nullptr;
};
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

string lexToAsm(Lexema<string>* root) {
    if(root != nullptr) {
        if(root->opCode == Operator::equal){
            return lexToAsm(root->rightOperand) + STORE("z");
        }
        if(root->opCode == Operator::plus && root->rightOperand->value.compare("-2") == 0 ){
            return lexToAsm(root->leftOperand) + ADDeq("-2");
        }
        if(root->opCode == Operator::plus && root->rightOperand->value.compare("x") == 0){
            return lexToAsm(root->leftOperand) + ADD("x");
        }
        if(root->opCode == Operator::multiply && root->leftOperand->value.compare("2") == 0){
            return LOAD("y") + MPYeq("2");
        }
        if(root->opCode == Operator::multiply && root->rightOperand->value.compare("y") == 0){
            return lexToAsm(root->leftOperand) + MPY("y");
        }
      
    }
    return "";
}


#define lex Lexema<string>*
#define makeLex new Lexema<string>()

int main(int argc, const char * argv[]) {
    
    
    lex root = makeLex;
    root->opCode = Operator::equal;
    root->leftOperand = makeLex;
    root->leftOperand->value = "z";
    root->leftOperand->id = 1;
    
    
    lex n6 = makeLex;
    n6->opCode = Operator::plus;
    n6->rightOperand = makeLex;
    n6->rightOperand->value = "-2";
    n6->rightOperand->id = 5;
    
    n6->leftOperand = makeLex;
    
    root->rightOperand = n6;
    
    lex n5 = n6->leftOperand;
    n5->opCode = Operator::plus;
    n5->rightOperand = makeLex;
    n5->rightOperand->value = "x";
    n5->rightOperand->id = 4;
    
    
    n5->leftOperand = makeLex;
    lex n4 = n5->leftOperand;
    n4->opCode = Operator::multiply;
    n4->rightOperand = makeLex;
    n4->rightOperand->value = "y";
    n4->rightOperand->id = 3;
    
    n4->leftOperand = makeLex;
    lex n3 = n4->leftOperand;
    n3->opCode = Operator::multiply;
    n3->rightOperand = makeLex;
    n3->rightOperand->value = "y";
    n3->rightOperand->id = 3;
    
    n3->leftOperand = makeLex;
    lex n2 = n3->leftOperand;
    n2->opCode = Operator::multiply;
    
    n2->rightOperand = makeLex;
    n2->rightOperand->value = "y";
    n3->rightOperand->id = 3;
    n2->leftOperand = makeLex;
    
    lex n1 = n2->leftOperand;
    n1->opCode = Operator::multiply;
    n1->leftOperand = makeLex;
    n1->leftOperand->value = "2";
    n1->leftOperand->id = 2;

    n1->rightOperand = makeLex;
    n1->rightOperand->value = "y";
    n1->rightOperand->id = 3;
    
    cout << printLexemTree(root) << endl;
    
    cout << lexToAsm(root) << endl;
    return 0;
}
