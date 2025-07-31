#include "mathparser.hpp"
#include <iostream>


MathParser::MathParser() {
    init();
}

void MathParser::registerOperator(const std::string& symbol, Operator op) {
    op.symbol = symbol;
    operators[symbol] = op;
}

void MathParser::registerConstant(const std::string& symbol, double value) {
    constants[symbol] = value;
}

void MathParser::init() {
    registerOperator("(", {
        Operator::Type::BRACKET, 
        0, false, 0, nullptr, "("
    });
    
    registerOperator(")", {
        Operator::Type::BRACKET, 
        0, false, 0, nullptr, ")"
    });

    registerOperator(",", {
        Operator::Type::COMMA, 
        0, false, 0, nullptr,
        ","
    });
    
    registerOperator("+", {
        Operator::Type::BINARY, 
        1, false, 2,
        [](auto args) { return args[0] + args[1]; },
        "+"
    });
    
    registerOperator("-", {
        Operator::Type::BINARY, 
        1, false, 2,
        [](auto args) { return args[0] - args[1]; },
        "-"
    });
    
    registerOperator("*", {
        Operator::Type::BINARY, 
        2, false, 2,
        [](auto args) { return args[0] * args[1]; },
        "*"
    });
    
    registerOperator("/", {
        Operator::Type::BINARY, 
        2, false, 2,
        [](auto args) { 
            if (args[1] == 0) throw std::runtime_error("Division by zero");
            return args[0] / args[1]; 
        },
        "/"
    });
    
    registerOperator("^", {
        Operator::Type::BINARY, 
        4, true, 2,
        [](auto args) { return pow(args[0], args[1]); },
        "^"
    });

    registerOperator("~", {
        Operator::Type::UNARY, 
        3, true, 1,
        [](auto args) { return -args[0]; },
        "~"
    });
    
    registerOperator("sqrt", {
        Operator::Type::FUNCTION, 
        5, false, 1,
        [](auto args) { 
            if (args[0] < 0) throw std::runtime_error("Square root of negative");
            return sqrt(args[0]); 
        },
        "sqrt"
    });
    registerOperator("sin", {
        Operator::Type::FUNCTION, 
        5, false, 1,
        [](auto args) { 
            return sin(args[0]); 
        },
        "sin"
    });
    registerOperator("cos", {
        Operator::Type::FUNCTION, 
        5, false, 1,
        [](auto args) { 
            return cos(args[0]); 
        },
        "cos"
    });
    registerOperator("tan", {
        Operator::Type::FUNCTION, 
        5, false, 1,
        [](auto args) { 
            return tan(args[0]); 
        },
        "tan"
    });

    registerConstant("pi", M_PIl);
    registerConstant("PI", M_PIl);
    registerConstant("e", M_E);
    registerConstant("E", M_E);
}

bool MathParser::isOperator(const std::string& symbol) {
    return operators.find(symbol)!=operators.end();
}

bool MathParser::isOperator(char symbol) {
    return isOperator(std::string(1,symbol));
}

bool MathParser::isConstant(const std::string& symbol) {
    return constants.find(symbol)!=constants.end();
}

std::vector<std::string> MathParser::toRPN(const std::string& expression) {
    std::stack<Operator> opStack;
    std::vector<std::string> RPN;

    bool expectOperand = true;
    int i=0;
    while (i<expression.length()) 
    {
        //skip space character
        if (isspace(expression[i])) {
            i++;
            continue;
        }
        if (isdigit(expression[i])) {
            expectOperand = false;
            int j=i;
            while (j<expression.length() && (isdigit(expression[j]) || expression[j]=='.')) {
                j++;
            }
            std::string number = expression.substr(i,j-i);
            RPN.push_back(number);
            i=j;
        }
        else if (expression[i]=='(') {
            opStack.push(operators["("]);
            i++;
            expectOperand = true;
        }
        else if (expression[i]==')') {
            
            while (!opStack.empty() && opStack.top().type!=Operator::Type::BRACKET) {
                RPN.push_back(opStack.top().symbol);
                opStack.pop();
            }
            
            if (opStack.empty()) throw std::runtime_error("Mismatched parentheses");
            opStack.pop();
            if (!opStack.empty() && opStack.top().type == Operator::Type::FUNCTION) {
                RPN.push_back(opStack.top().symbol);
                opStack.pop();
            }
            i++;
        }
        else if (isalpha(expression[i])) {
            int j=i;
            while (j<expression.length() && isalpha(expression[j])) {
                j++;
            }
            std::string symbol = expression.substr(i,j-i);
            if (isOperator(symbol)) {
                opStack.push(operators[symbol]);
                expectOperand = true;
            }
            //handle constant (variable)
            else if (isConstant(symbol)) {
                RPN.push_back(symbol);
                expectOperand = false;
            }
            else {
                throw std::runtime_error("Unknown symbol");
            }
            i=j;
        }
        else if (isOperator(expression[i])) {
            std::string symbol = std::string(1,expression[i]);
            //if we get - before operand its unary minus
            if (symbol=="-" && expectOperand) symbol = "~";
            Operator curOp = operators[symbol];
            while (!opStack.empty() && opStack.top().type !=Operator::Type::BRACKET) {
                Operator topOp = opStack.top();
                if (topOp.type == Operator::Type::COMMA || topOp.type == Operator::Type::FUNCTION) break;
                if (topOp.precedence < curOp.precedence) break;
                if (topOp.precedence == curOp.precedence && curOp.isRightAssociative) break;
                opStack.pop();
                RPN.push_back(topOp.symbol);
            }
            opStack.push(curOp);
            expectOperand = true;
            i++;
        }
    }
    while (!opStack.empty()) {
        Operator op = opStack.top();
        RPN.push_back(op.symbol);
        opStack.pop();
    }
    return RPN;

}

double MathParser::evaluateRPN(const std::vector<std::string>& RPN) {
    std::stack<double> numStack;
    for (const std::string& symbol: RPN) {
        if (isOperator(symbol)) {
            Operator op = operators[symbol];
            if (op.type==Operator::Type::COMMA) continue;
            std::vector<double> operands;
            for (int i=0; i<op.operandCount; i++) {
                operands.insert(operands.begin(),numStack.top());
                numStack.pop();
            }
            numStack.push(op.evaluate(operands));
        }
        else {
            if (isConstant(symbol))
                numStack.push(constants[symbol]);
            else
                numStack.push(stod(symbol));
        }
    }
    return numStack.top();
}

double MathParser::evaluate(const std::string& expression) {
    return evaluateRPN(toRPN(expression));
}