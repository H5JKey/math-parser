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
    registerOperator("=", {
        Operator::Type::BINARY, 
        0, false, false, 2,
        [this](auto args) {
            if (args[0].type != Token::Type::VARIABLE) 
                throw std::runtime_error("Left operand must be a variable");
            
            std::string varName = std::get<std::string>(args[0].value);
            
            double value = tokenToDouble(args[1]);
            
            variables[varName] = value;
            return Token{Token::Type::NUMBER, value};
        },
        "="
    });
    registerOperator("(", {
        Operator::Type::BRACKET, 
        0, false, false, 0, nullptr, "("
    });
    
    registerOperator(")", {
        Operator::Type::BRACKET, 
        0, false, false, 0, nullptr, ")"
    });

    registerOperator(",", {
        Operator::Type::COMMA, 
        0, false, false, 0, nullptr,
        ","
    });
    
    registerOperator("+", {
        Operator::Type::BINARY, 
        1, false, false, 2,
        [this](auto args) {
            double left = tokenToDouble(args[0]);
            double right = tokenToDouble(args[1]);
            return Token{Token::Type::NUMBER, left + right}; 
        },
        "+"
    });
    
    registerOperator("-", {
        Operator::Type::BINARY, 
        1, false, false, 2,
        [this](auto args) {
            double left = tokenToDouble(args[0]);
            double right = tokenToDouble(args[1]);
            return Token{Token::Type::NUMBER, left - right}; 
        },
        "-"
    });
    
    registerOperator("*", {
        Operator::Type::BINARY, 
        2, false, false, 2,
        [this](auto args) {
            double left = tokenToDouble(args[0]);
            double right = tokenToDouble(args[1]);
            return Token{Token::Type::NUMBER, left * right}; 
        },
        "*"
    });
    
    registerOperator("/", {
        Operator::Type::BINARY, 
        2, false, false, 2,
        [this](auto args) {
            double left = tokenToDouble(args[0]);
            double right = tokenToDouble(args[1]);
            if (right==0) throw std::runtime_error("Division by zero");
            return Token{Token::Type::NUMBER, left / right}; 
        },
        "/"
    });
    
    registerOperator("^", {
        Operator::Type::BINARY, 
        4, true, false, 2,
        [this](auto args) {
            double left = tokenToDouble(args[0]);
            double right = tokenToDouble(args[1]);
            return Token{Token::Type::NUMBER, std::pow(left , right)}; 
        },
        "^"
    });

    registerOperator("~", {
        Operator::Type::UNARY, 
        3, true, false, 1,
       [this](auto args) {
            double arg = tokenToDouble(args[0]);
            return Token{Token::Type::NUMBER, -arg}; 
        },
        "~"
    });
    
    registerOperator("sqrt", {
        Operator::Type::FUNCTION, 
        5, false, false, 1,
        [this](auto args) {
            double arg = tokenToDouble(args[0]);
            return Token{Token::Type::NUMBER, sqrt(arg)}; 
        },
        "sqrt"
    });
    registerOperator("sin", {
        Operator::Type::FUNCTION, 
        5, false, false, 1,
        [this](auto args) {
            double arg = tokenToDouble(args[0]);
            return Token{Token::Type::NUMBER, sin(arg)}; 
        },
        "sin"
    });
    registerOperator("cos", {
        Operator::Type::FUNCTION, 
        5, false, false, 1,
        [this](auto args) {
            double arg = tokenToDouble(args[0]);
            return Token{Token::Type::NUMBER, cos(arg)}; 
        },
        "cos"
    });
    registerOperator("tan", {
        Operator::Type::FUNCTION, 
        5, false, false, 1,
        [this](auto args) {
            double arg = tokenToDouble(args[0]);
            return Token{Token::Type::NUMBER, tan(arg)};
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

double MathParser::tokenToDouble(const MathParser::Token& token) const {
    switch (token.type) {
        case Token::Type::NUMBER:
            return std::get<double>(token.value);
            
        case Token::Type::CONSTANT: {
            const auto& name = std::get<std::string>(token.value);
            auto it = constants.find(name);
            if (it == constants.end())
                throw std::runtime_error("Unknown constant: " + name);
            return it->second;
        }
            
        case Token::Type::VARIABLE: {
            const auto& name = std::get<std::string>(token.value);
            auto it = variables.find(name);
            if (it == variables.end())
                throw std::runtime_error("Undefined variable: " + name);
            return it->second;
        }
            
        default:
            throw std::runtime_error("Cannot convert token to number");
    }
}

std::vector<MathParser::Token> MathParser::toRPN(const std::string& expression) {
    std::stack<Operator> opStack;
    std::vector<Token> RPN;

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
            RPN.push_back(Token{Token::Type::NUMBER, stod(number)});
            i=j;
        }
        else if (expression[i]=='(') {
            opStack.push(operators["("]);
            i++;
            expectOperand = true;
        }
        else if (expression[i]==')') {
            
            while (!opStack.empty() && opStack.top().type!=Operator::Type::BRACKET) {
                RPN.push_back(Token{Token::Type::OPERATOR,opStack.top().symbol});
                opStack.pop();
            }
            
            if (opStack.empty()) throw std::runtime_error("Mismatched parentheses");
            opStack.pop();
            if (!opStack.empty() && opStack.top().type == Operator::Type::FUNCTION) {
                RPN.push_back(Token{Token::Type::OPERATOR,opStack.top().symbol});
                opStack.pop();
            }
            i++;
        }
        else if (isalpha(expression[i])) {
            int j=i;
            while (j<expression.length() && isalpha(expression[j])) {
                j++;
            }
            //functions
            std::string symbol = expression.substr(i,j-i);
            if (isOperator(symbol)) {
                opStack.push(operators[symbol]);
                expectOperand = true;
            }
            //handle constant
            else if (isConstant(symbol)) {
                RPN.push_back(Token{Token::Type::CONSTANT,symbol});
                expectOperand = false;
            }
            //variables
            else {
                RPN.push_back(Token{Token::Type::VARIABLE,symbol});
                expectOperand = false;
            }
            i=j;
        }
        else if (isOperator(expression[i])) {
            std::string symbol = std::string(1,expression[i]);
            //if we get - before operand its unary minus
            if (symbol=="-" && expectOperand) symbol = "~";
            auto it = operators.find(symbol);
            if (it == operators.end())
                throw std::runtime_error("Unknown operator: " + symbol);
            Operator curOp = it->second;
            while (!opStack.empty() && opStack.top().type !=Operator::Type::BRACKET) {
                Operator topOp = opStack.top();
                if (topOp.type == Operator::Type::COMMA || topOp.type == Operator::Type::FUNCTION) break;
                if (topOp.precedence < curOp.precedence) break;
                if (topOp.precedence == curOp.precedence && curOp.isRightAssociative) break;
                opStack.pop();
                RPN.push_back(Token{Token::Type::OPERATOR,topOp.symbol});
            }
            opStack.push(curOp);
            if (curOp.isPostfix) expectOperand = false;
            else expectOperand = true;
            i++;
        }
    }
    while (!opStack.empty()) {
        Operator op = opStack.top();
        RPN.push_back(Token{Token::Type::OPERATOR,op.symbol});
        opStack.pop();
    }
    return RPN;

}

double MathParser::evaluate(const std::vector<Token>& compiled) {
    std::stack<Token> numStack;
    for (const Token& token: compiled) {
        if (token.type == Token::Type::OPERATOR) {
           const std::string& opSymbol = std::get<std::string>(token.value);
            auto it = operators.find(opSymbol);
            if (it == operators.end())
                throw std::runtime_error("Unknown operator: " + opSymbol);
            const Operator& op = it->second;
            std::vector<Token> operands;
            for (int i=0; i<op.operandCount; i++) {
                operands.insert(operands.begin(),numStack.top());
                numStack.pop();
            }
            numStack.push(op.evaluate(operands));
        }
        else {
            numStack.push(token);
        }
    }
    return tokenToDouble(numStack.top());
}

double MathParser::evaluate(const std::string& expression) {
    auto compiled = compile(expression);
    return evaluate(compiled);
}

std::vector<MathParser::Token>  MathParser::compile(const std::string& expression) {
    auto RPN = toRPN(expression);
    std::vector<Token> compiled;
    for (const Token& token: RPN) {
        if (token.type == Token::Type::OPERATOR) {
            const std::string& opName = std::get<std::string>(token.value);
            auto it = operators.find(opName);
            if (it == operators.end())
                throw std::runtime_error("Unknown operator: " + opName);
            Operator op = it->second;
            if (op.type==Operator::Type::COMMA) continue;
            bool allConstants = true;
            for (int i=0; i<op.operandCount; i++) {
                if ((compiled.end()-1-i)->type!=Token::Type::NUMBER) {
                    allConstants = false;
                    break;
                }
            }
            if (allConstants) {
                std::vector<Token> operands;
                for (int i=0; i<op.operandCount; i++) {
                    operands.insert(operands.begin(),compiled.back());
                    compiled.pop_back();
                }
                compiled.push_back(op.evaluate(operands));
            }
            else {
                compiled.push_back(token);
            }
        }
        else {
            if (token.type==Token::Type::CONSTANT) {
                const std::string& constName = std::get<std::string>(token.value);
                auto it = constants.find(constName);
                if (it == constants.end())
                    throw std::runtime_error("Unknown constant: " + constName);
                compiled.push_back(Token{Token::Type::NUMBER, it->second});
            }
            else
                compiled.push_back(token);
        }
    }
    return compiled;
}

double MathParser::getVariableValue(const std::string& varName) {
    return tokenToDouble(Token{Token::Type::VARIABLE, varName});
}
