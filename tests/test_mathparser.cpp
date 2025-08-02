#include <iostream>
#include <cassert>
#include "mathparser.hpp"

int main() {
    MathParser mathParser;
    int failures = 0;
    
    auto test = [&](bool condition, const char* message) {
        if(!condition) {
            std::cerr << "[FAIL] " << message << "\n";
            failures++;
        } else {
            std::cout << "[OK] " << message << "\n";
        }
    };
    const double EPS =1e-6;

    mathParser.registerOperator("max", MathParser::Operator{
        MathParser::Operator::Type::FUNCTION, 
        4, false, false, 2,
        [&](auto args) { 
            double left = mathParser.tokenToDouble(args[0]);
            double right = mathParser.tokenToDouble(args[1]);
            return MathParser::Token{MathParser::Token::Type::NUMBER, std::max(left, right)};
        },
        "max"
    });

    test(mathParser.evaluate("-~-4.2")==-4.2, "---4.2 == -4.2");
    test(mathParser.evaluate("-1-2-3")==-6, "-1-2-3 == -6");
    test(mathParser.evaluate("2.1+4.5")==6.6, "2.1 + 4.5 == 6.6");
    test(mathParser.evaluate("   2     +2   *    2   ")==6, "2 + 2 * 2 == 6");
    test(mathParser.evaluate("5*(3+-3)")==0, "5 * (3+ -3) == 0");
    test(mathParser.evaluate("(2.9+3.1)/3.0")==2, "(2.9 + 3.1) / 3.0 == 2");
    test(mathParser.evaluate("1+1-1+1-1*333/333.0*333/333+1-(3-2)")==1, "1+1-1+1-1*333/333.0*333/333+1-(3-2) == 1");
    test(mathParser.evaluate("2^5")==32, "2^5 == 32");
    test(mathParser.evaluate("-2^4")==-16, "-2^4 == -16");
    test(mathParser.evaluate("(-2)^4")==16, "(-2)^4 == 16");
    test(mathParser.evaluate("2^3+3^2-2*2")==13, "2^3+3^2-2*2 == 13");
    test(mathParser.evaluate("9^(1/2)")==3, "9^(1/2) == 3");
    test(mathParser.evaluate("sqrt(3^2+4^2)")==5, "sqrt(3^2 + 4^2) == 5");
    test(fabs(mathParser.evaluate("sin(cos(-3*pi/2))"))<EPS, "sin(cos(-3*pi/2)) ~ 0");
    test(mathParser.evaluate("max(-1,-2)") == -1, "max(-1, -2) == -1");
    test(mathParser.evaluate("max(1,max(2,max(3,max(4,-5))))") == 4, "max(1,max(2,max(3,max(4,-5)))) == 4");

    std::cout<<"\nTesting variables...\n\n";
    test(mathParser.evaluate("x=5")==5, "x = 5");
    test(mathParser.evaluate("y=7")==7, "y = 7");
    test(mathParser.evaluate("x = x+y+1")==13, "x = x+y+1");
    test(mathParser.evaluate("x - y")==6, "x - y == 6");
    
    if (failures > 0) {
        std::cerr << "\n" << failures << " TESTS FAILED!\n";
        return 1;
    }
    
    std::cout << "\nALL TESTS PASSED!\n";
    return 0;
}