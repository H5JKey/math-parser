# Math Parser Library

A flexible and extensible C++ library for parsing and evaluating mathematical expressions.

## Features

- Basic arithmetic operations (`+`, `-`, `*`, `/`, `^`)
- Unary minus (`-x`)
- Math functions (`sqrt`, `sin`, `cos`, `tan`)
- Constants (`pi`, `e`)
- Parentheses and operator precedence
- Floating-point number support
- Easy to add new operators, functions and constans

## Quick Start

```cpp
#include "mathparser.hpp"

int main() {
    MathParser parser;

    // Evaluate simple expression
    double result = parser.evaluate("2 + 2 * 2"); // 6
    
    // Add custom operator
    parser.registerOperator("max", parser::Operator{
        parser::Operator::Type::FUNCTION, 
        4, false, 2,
        [](auto args) { return std::max(args[0], args[1]); },
        "max"
    });

    // Evaluate custom function
    result = parser.evaluate("max(2-2*2, 2^2-1)"); // 3
    
    return 0;
}
```

## Extending Functionality

### Adding Custom Function or Operator
```cpp
// Register logarithmic function
parser.registerOperator("log", parser::Operator{
    parser::Operator::Type::FUNCTION,
    4,         // Precedence
    false,     // Right associativity
    1,         // Single argument
    [](auto args) { return std::log(args[0]); },
    "log"      // Function name
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
