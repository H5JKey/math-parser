#include "mathparser.hpp"

int main() {
    MathParser parser;

    // Evaluate simple expression
    double result = parser.evaluate("2 + 2 * 2"); // 6
    
    // Add custom operator
    parser.registerOperator("max", MathParser::Operator{
        MathParser::Operator::Type::FUNCTION, 
        4, false, false, 2,
        [&](auto args) {
            // this function convert token to double, according of its type
            double left = parser.tokenToDouble(args[0]); 
            double right = parser.tokenToDouble(args[1]);
            return MathParser::Token{MathParser::Token::Type::NUMBER, std::max(left, right)}; 
        },
        "max"
    });

    // Evaluate custom function
    result = parser.evaluate("max(2-2*2, 2^2-1)"); // 3

    //Variables
    parser.evaluate("x = 3"); // 3
    parser.evaluate("y = 42"); // 42
    parser.evaluate("x = x + 10"); // 13
    parser.evaluate("var = y - x"); // 29
    parser.getVariableValue("var"); // 29
    
    return 0;
}