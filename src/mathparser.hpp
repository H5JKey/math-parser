#pragma once
#include <string>
#include <stack>
#include <vector>

class MathParser {
private:
    bool isOperation(std::string op);
    int priority(std::string op);
public:
    MathParser() = default;
     ~MathParser() = default;

    double evaluate(const std::string& expression);
    double evaluateRPN(const std::vector<std::string>& RPN);  //evaluate Reverse Polish notation
    std::vector<std::string> toRPN(const std::string& expression);  //make Reverse Polish notation
};