#include "mathparser.hpp"
#include <iostream>

bool MathParser::isOperation(std::string op) {
    return op=="+" || 
        op=="-" || 
        op=="*" || 
        op=="/" ||
        op=="(" ||
        op==")" ||
        op=="~" ||
        op=="^";
}

int MathParser::priority(std::string op) {
    if (op== "^")
        return 4;
    if (op == "~")
        return 3;
    if (op == "*" || op=="/")
        return 2;
    if (op == "+" || op=="-")
        return 1;
    if (op == "(")
        return 0;
    return 0;
}

std::vector<std::string> MathParser::toRPN(const std::string& expression) {
    std::stack<std::string> operations;
    std::vector<std::string> RPN;

    bool expectOperand = true;
    int i=0;
    while (i<expression.length()) 
    {
        if (expression[i]==' ') {
            i++;
            continue;
        }
        if (isOperation(std::string(1,expression[i]))) {
            if (expression[i]=='(') {
                operations.push(std::string(1,expression[i]));
                i++;
            }
            else if (expression[i]==')') {
                while (operations.top()!="(") {
                    RPN.push_back(operations.top());
                    operations.pop();
                }
                operations.pop();
                i++;
            }
            else {
                std::string op = std::string(1,expression[i]);
                //if we get - before operand its unary minus
                if (op=="-" && expectOperand) op = "~";
                while (!operations.empty()) {
                    std::string c = operations.top();
                    int cPrioriety = priority(c);
                    int opPrioriety = priority(op);

                    if (cPrioriety < opPrioriety) break;
                    if (cPrioriety == opPrioriety && op == "~") break;

                    operations.pop();
                    RPN.push_back(c);
                }
                operations.push(op);
                expectOperand = true;
                i++;
            }
        }
        else {
            expectOperand = false;
            int j=i;
            while (j<expression.length() && !isOperation(std::string(1,expression[j]))) {
                j++;
            }
            std::string number = expression.substr(i,j-i);
            RPN.push_back(number);
            i=j;
        }
    }
    while (!operations.empty()) {
        RPN.push_back(operations.top());
        operations.pop();
    }
    return RPN;

}

double MathParser::evaluateRPN(const std::vector<std::string>& RPN) {
    std::stack<double> stack;
    for (const std::string& token: RPN) {
        if (isOperation(token)) {
            if (token=="~") {
                double operand = stack.top();
                stack.pop();
                stack.push(-operand);
            }
            else {
                double operand2 = stack.top();
                stack.pop();
                double operand1 = stack.top();
                stack.pop();
                
                double res;
                if (token=="+")
                    res = operand1 + operand2;
                else if (token=="-")
                    res = operand1 - operand2;
                else if (token=="*")
                    res = operand1 * operand2;
                else if (token=="/")
                    res = operand1 / operand2;
                else if (token=="^")
                    res = pow(operand1,  operand2);
                stack.push(res);
            }
        }
        else {
            stack.push(stod(token));
        }
    }
    return stack.top();
}

double MathParser::evaluate(const std::string& expression) {
    return evaluateRPN(toRPN(expression));
}