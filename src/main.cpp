#include "mathparser.hpp"
#include <iostream>

int main() {
    MathParser math;
    std::cout<<math.evaluate("x=5")<<'\n';
    std::cout<<math.evaluate("y=7")<<'\n';
    std::cout<<math.evaluate("x = x+y+1")<<'\n';
    std::cout<<math.evaluate("variable = x - y")<<'\n';
    std::cout<<math.getVariableValue("variable");
    return 0;
}