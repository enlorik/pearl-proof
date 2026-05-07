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

## Rational Expressions

PearlProof supports rational expressions (fractions of polynomials) such as `x/x`, `(x^2-1)/(x-1)`, or `1/2`.

### Mathematical semantics

Internally, every expression is represented as a **RationalPolynomial** — a pair `(numerator, denominator)` of polynomials. Equality of two rational expressions `A = B` (i.e. `A.num/A.den = B.num/B.den`) is checked by **cross-multiplication**:

```
A.num * B.den - B.num * A.den  ==  0  (as a polynomial)
```

This is a **rational-function identity check**: the equation is accepted if and only if both sides are equal as rational functions, i.e. they agree on every point in the domain where all denominators are nonzero.

For example:
```
check x/x = 1
```
is **accepted** because `x * 1 - 1 * x = 0` as a polynomial, even though the expression is undefined at `x = 0`. The checker treats this as an identity on the domain `x ≠ 0`.

### Division by the zero polynomial

If a denominator simplifies to the **zero polynomial** (identically zero for all variable values), PearlProof detects this and reports a clean error rather than accepting or crashing. Examples of such invalid expressions:

```
check 1/(x-x) = 1        → ERROR: Division by zero polynomial: denominator is identically zero for all variable values
check (x+1)/(x+1-x-1) = 0  → ERROR: Division by zero polynomial: denominator is identically zero for all variable values
```

These are kept in `invalid_denominator.proof` and are expected to produce error output.


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
