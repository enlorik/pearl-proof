# pearl-proof

A C++17 math-checker language built with ANTLR4. PearlProof verifies mathematical equations by converting expressions to canonical polynomial form.

## Features

- Support for integers, variables, and operators: +, -, *, /, ^
- Parentheses for grouping
- Checks equations line by line
- Converts expressions to canonical polynomial form for comparison

## Building

Prerequisites:
- CMake 3.10+
- C++17 compiler
- ANTLR4 runtime for C++
- ANTLR4 tool (Java version)

Build steps:
```bash
mkdir build
cd build
cmake ..
make
```

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

## Implementation

The checker works by:
1. Parsing input using ANTLR4 grammar
2. Building an Expression AST via visitor pattern
3. Converting expressions to canonical polynomial form (map<monomial, coefficient>)
4. Computing poly(A) - poly(B) for each check
5. Reporting OK if result is zero polynomial, ERROR otherwise
