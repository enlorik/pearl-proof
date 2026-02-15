# pearl-proof

A C++17 math-checker language built with ANTLR4. PearlProof verifies mathematical equations by converting expressions to canonical polynomial form.

## Features

- Support for integers, variables, and operators: +, -, *, /, ^
- Parentheses for grouping
- Checks equations line by line
- Converts expressions to canonical polynomial form for comparison

## Building

### Prerequisites
- CMake 3.10+
- C++17 compiler (g++, clang++)
- ANTLR4 C++ runtime library (version 4.10)
- Java Runtime Environment (for ANTLR4 tool)

### Installing Dependencies

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install cmake g++ libantlr4-runtime-dev default-jre
```

**Note**: The ANTLR4 tool (JAR file) is included in this repository as `antlr-4.10-complete.jar` to ensure version compatibility with the C++ runtime. The build system will automatically use this JAR file to generate the parser and lexer.

### Build Steps
```bash
mkdir build
cd build
cmake ..
make
```

This will create the `pearlproof` executable in the `build` directory.

## Usage

Create a `.proof` file with equations to check:
```
check 1 + 1 = 2
check x + x = 2 * x
check x^2 - 1 = (x + 1) * (x - 1)
```

Run the checker:
```bash
./pearlproof test.proof
```

Output:
```
Line 1: OK
Line 2: OK
Line 3: OK
```

## Implementation Details

The checker works by:
1. **Parsing**: Using ANTLR4 grammar to parse input files
2. **AST Building**: Converting parse tree to Expression AST via visitor pattern
3. **Polynomial Form**: Converting expressions to canonical polynomial form (map<monomial, coefficient>)
   - Monomial: map of variable names to exponents
   - Polynomial: map of monomials to coefficients
4. **Verification**: Computing poly(A) - poly(B) for each check
5. **Result**: Reporting OK if result is zero polynomial, ERROR with line number otherwise

### Supported Operations
- Addition and subtraction of polynomials
- Multiplication of polynomials
- Division by integer constants
- Exponentiation by non-negative integer constants

## Examples

### Valid Equations (OK)
```
check 0 = 0
check x - x = 0
check (a + b)^2 = a^2 + 2*a*b + b^2
check (x + y) * (x - y) = x^2 - y^2
check x^3 = x * x * x
check 10 / 2 = 5
```

### Invalid Equations (ERROR)
```
check x + 1 = x
check (x + 1)^2 = x^2 + 1
check 2 + 2 = 5
```
