#include <iostream>
#include <cassert>
#include "../src/mathparser.hpp"

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

    test(mathParser.evaluate("---4.2")==-4.2, "---4.2 == -4.2");
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
    
    if (failures > 0) {
        std::cerr << "\n" << failures << " TESTS FAILED!\n";
        return 1;
    }
    
    std::cout << "\nALL TESTS PASSED!\n";
    return 0;
}