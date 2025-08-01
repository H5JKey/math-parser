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

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/math-parser.git
```

2. Build with CMake:
```bash
mkdir build && cd build
cmake ..
make
```

3. Link to your project:
```cmake
add_subdirectory(path/to/math-parser)
target_link_libraries(your_target mathparser)
```

## Testing

To run tests:
```bash
cd build
ctest
```
