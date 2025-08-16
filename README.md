# Math Parser Library ![C++](https://img.shields.io/badge/C++-23-blue.svg) ![Build](https://img.shields.io/badge/Build-CMake-yellow.svg) ![Testing](https://img.shields.io/badge/Testing-CTest-green.svg)

A flexible and extensible C++ library for parsing and evaluating mathematical expressions.

## Features

- Basic arithmetic operations (`+`, `-`, `*`, `/`, `^`)
- Unary minus (`-x` or `~x`)
- Math functions (`sqrt`, `sin`, `cos`, `tan`)
- Constants (`pi`, `e`)
- Parentheses and operator precedence
- Floating-point number support
- Easy to add new operators, functions and constans
- Easy to add and use variables
- Math expression compilation for further using

## Quick Start

```cpp
#include "mathparser.hpp"
 #include <iostream>

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
    //Compilation
    auto compiled = parser.compile("2+2*2+2*x+10"); // 6+2*x+10
    parser.evaluate("x = 4");
    parser.evaluate(compiled); // 24
    parser.evaluate("x = 1");
    parser.evaluate(compiled); // 18

    return 0;
}
```

## Extending Functionality

### Adding Custom Function or Operator
```cpp
// Register logarithmic function
parser.registerOperator("!", parser::Operator{
    parser::Operator::Type::FUNCTION,
    4,         // Precedence
    false,     // Right associativity
    true,      // Postfix operator
    1,         // Single argument
    [](auto args) {
          double arg = parser.tokenToDouble(args[0]);
          return MathParser::Token{MathParser::Token::Type::NUMBER, std::tgamma(arg+1)};
    }, // Evaluating function
    "!"      // Function name
});
```

### Adding Constant
```cpp
// Register tau constant (2pi)
parser.registerConstant("tau", 6.28318);
```

## Installation

### Method 1: FetchContent (Recommended for CMake Projects)

Add to your CMakeLists.txt:
```cmake
include(FetchContent)
FetchContent_Declare(
    math_parser
    GIT_REPOSITORY https://github.com/H5JKey/math-parser.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(math_parser)

target_link_libraries(your_target math_parser_lib)
```

### Method 2: Manual Inclusion

1. Clone repository:
```bash
git clone https://github.com/H5JKey/math-parser.git
```

2. Add to your project:
```cmake
add_subdirectory(path/to/math-parser)
target_link_libraries(your_target math_parser_lib)
```


## Testing

To run tests:
```bash
cd build
ctest
```
