#pragma once
#include <string>
#include <stack>
#include <vector>
#include <functional>
#include <cmath>
#include <map>

class MathParser {
public:
    using EvaluateFunc = std::function<double(const std::vector<double>&)>;
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
        int operandCount;
        EvaluateFunc evaluate;
        std::string symbol;
    };   
private:
    std::unordered_map<std::string, Operator> operators;
    std::map<std::string, double> constants;
    void init();

    bool isOperator(const std::string& symbol);
    bool isOperator(char symbol);

    bool isConstant(const std::string& symbol);

public:
    MathParser();
    ~MathParser() = default;

    void registerOperator(const std::string& symbol, Operator op);
    void registerConstant(const std::string& symbol, double value);
    double evaluate(const std::string& expression);
    double evaluateRPN(const std::vector<std::string>& RPN);  //evaluate Reverse Polish notation
    std::vector<std::string> toRPN(const std::string& expression);  //make Reverse Polish notation
};