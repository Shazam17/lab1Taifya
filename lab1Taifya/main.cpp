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
    pointer
};

template<typename T>
struct Lexema {
    Operator opCode;
    T value = NULL;
    string varName;
    string pAdress = NULL;
    Lexema<T>* leftOperand;
    Lexema<T>* rightOperand;
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

template<typename T>
void parseToLexems(string formula) {
    //parsing formula to extract lexems
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
                if((getPrecedence(top) >= getPrecedence(val) || val == '-')) {
                   while(!opStack.empty()){
                       char op = opStack.top();
                       opStack.pop();
                       output.push(op);
                       cout << op << ' ';
                   }
                }
            }
            opStack.push(val);
        }else if (val != ' '){
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
    queue<char> tStack = output;
    while(!tStack.empty()){
        char val = tStack.front();
        tStack.pop();
        cout << val << ' ';
    }
    cout << endl;
    
    stack<int> vars;
    Lexema<int>* rootLexema = new Lexema<int>();
    
    
    //z equals our expression
//    rootLexema->opCode = Operator::equal;
//    rootLexema->leftOperand = new Lexema<int>();
//    rootLexema->leftOperand->opCode = Operator::constanta;
//    rootLexema->leftOperand->varName = "z";
    
    Lexema<int>* temp = NULL; // = rootLexema;
    
    while(!output.empty()){
        char operand1 = output.front();
        output.pop();
        cout << "operand now is : "<<operand1 << endl;
        if ( operand1 == '-' || operand1 == '+' || operand1 == '*' || operand1 == '/'){
            int operand2 = vars.top();
            vars.pop();
            int operand3 = vars.top();
            vars.pop();
            cout << "operand1 : " << operand2 << ' ' << operand1 << ' ' << "operand2 : " << operand3 << '|' << operand2 + operand3 << endl;
            if (temp == NULL) {
                temp = new Lexema<int>();
                
                switch (operand1) {
                    case '+':
                        temp->opCode = Operator::plus;
                        break;
                    case '-':
                        temp->opCode = Operator::minus;
                        break;
                    case '*':
                        temp->opCode = Operator::multiply;
                        break;
                    case '/':
                        temp->opCode = Operator::divide;
                        break;
                    default:
                        break;
                }
                
                temp->leftOperand = new Lexema<int>();
                temp->leftOperand->opCode = Operator::constanta;
                temp->leftOperand->value = operand2;
                
                temp->rightOperand = new Lexema<int>();
                temp->rightOperand->opCode = Operator::constanta;
                temp->rightOperand->value = operand3;
            }else{
                Lexema<int>* leftLeaf = temp;
                
                temp = new Lexema<int>();
                temp->leftOperand = leftLeaf;
                temp->rightOperand = new Lexema<int>();
                
                
            }
            
            
            vars.push(operand2 + operand3);
        }else {
            vars.push(charToInt(operand1));
        }
    }
    
}

//template<typename T>
//string lexemsToAsm(vector<Lexema<T>> lexems) {
//
//}


int main(int argc, const char * argv[]) {
    
    
    string formula;
    cout << "Enter formula: ";
    getline(cin , formula);
    parseToLexems<double>(formula);
    
    
    return 0;
}
