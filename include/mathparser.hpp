#pragma once
#include <string>
#include <stack>
#include <vector>
#include <functional>
#include <cmath>
#include <map>
#include <variant>
#include <stdexcept>

class MathParser {
public:
    struct Token {
        enum class Type {
            OPERATOR,
            NUMBER,
            VARIABLE,
            CONSTANT
        };
        Type type;
        std::variant<double, std::string> value;

        bool operator<(const Token& other) const {
            if (type != other.type) 
                return static_cast<int>(type) < static_cast<int>(other.type);
            
            switch(type) {
                case Type::NUMBER:
                    return std::get<double>(value) < std::get<double>(other.value);
                default:
                    return std::get<std::string>(value) < std::get<std::string>(other.value);
            }
        }
    };
    using EvaluateFunc = std::function<Token(const std::vector<Token>&)>;
    struct Operator {
        enum class Type {
            UNARY,
            BINARY,
            FUNCTION,
            BRACKET,
            COMMA
        };
        Type type;
        int precedence;
        bool isRightAssociative;
        bool isPostfix;
        int operandCount;
        EvaluateFunc evaluate;
        std::string symbol;
    };  

private:
    std::unordered_map<std::string, Operator> operators;
    std::map<std::string, double> constants;
    std::map<std::string, double> variables;
    void init();

    bool isOperator(const std::string& symbol);
    bool isOperator(char symbol);

    bool isConstant(const std::string& symbol);

    std::vector<Token> toRPN(const std::string& expression);

public:
    MathParser();
    ~MathParser() = default;

    void registerOperator(const std::string& symbol, Operator op);
    void registerConstant(const std::string& symbol, double value);
    double evaluate(const std::string& expression);
    std::vector<Token> compile(const std::string& expression);
    double evaluate(const std::vector<Token>& complied);
    double tokenToDouble(const Token& token);
    double getVariableValue(const std::string& varName);
};