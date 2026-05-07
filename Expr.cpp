#include "Expr.h"
#include <stdexcept>
#include <cmath>

// Convert integer to polynomial
Polynomial IntExpr::toPolynomial() const {
    Polynomial result;
    if (value != 0) {
        Monomial constant; // Empty monomial represents constant term
        result[constant] = value;
    }
    return result;
}

// Convert variable to polynomial
Polynomial VarExpr::toPolynomial() const {
    Polynomial result;
    Monomial mono;
    mono[name] = 1;
    result[mono] = 1;
    return result;
}

// Convert unary operation to polynomial
Polynomial UnaryExpr::toPolynomial() const {
    Polynomial operandPoly = operand->toPolynomial();
    
    switch (op) {
        case NEG: {
            // Negate all coefficients
            Polynomial result;
            for (const auto& term : operandPoly) {
                result[term.first] = -term.second;
            }
            return result;
        }
    }
    return Polynomial();
}

// Convert binary operation to polynomial
Polynomial BinOpExpr::toPolynomial() const {
    Polynomial leftPoly = left->toPolynomial();
    Polynomial rightPoly = right->toPolynomial();
    
    switch (op) {
        case ADD:
            return addPolynomials(leftPoly, rightPoly);
        case SUB:
            return subtractPolynomials(leftPoly, rightPoly);
        case MUL:
            return multiplyPolynomials(leftPoly, rightPoly);
        case DIV:
            return dividePolynomials(leftPoly, rightPoly);
        case POW: {
            // For power, right side must be a constant integer
            // Special case: if rightPoly is empty, the exponent value is 0
            if (rightPoly.empty()) {
                // When exponent is 0: base^0 = 1 (by convention, including 0^0)
                // Note: 0^0 is mathematically undefined, but is treated as 1 here
                Polynomial result;
                Monomial constant;
                result[constant] = 1;
                return result;
            }
            if (rightPoly.size() != 1) {
                throw std::runtime_error("Exponent must be a constant integer");
            }
            auto term = rightPoly.begin();
            if (!term->first.empty()) {
                throw std::runtime_error("Exponent must be a constant integer (not a variable expression)");
            }
            int exp = term->second;
            if (exp < 0) {
                throw std::runtime_error("Negative exponents not supported");
            }
            return powerPolynomial(leftPoly, exp);
        }
    }
    return Polynomial();
}

// Convert integer to rational polynomial
RationalPolynomial IntExpr::toRationalPolynomial() const {
    return { toPolynomial(), onePolynomial() };
}

// Convert variable to rational polynomial
RationalPolynomial VarExpr::toRationalPolynomial() const {
    return { toPolynomial(), onePolynomial() };
}

// Convert unary operation to rational polynomial
RationalPolynomial UnaryExpr::toRationalPolynomial() const {
    RationalPolynomial rat = operand->toRationalPolynomial();
    switch (op) {
        case NEG:
            return { negatePolynomial(rat.num), rat.den };
    }
    return { Polynomial(), onePolynomial() };
}

// Convert binary operation to rational polynomial
RationalPolynomial BinOpExpr::toRationalPolynomial() const {
    RationalPolynomial leftRat = left->toRationalPolynomial();
    RationalPolynomial rightRat = right->toRationalPolynomial();

    switch (op) {
        case ADD:
            return addRationals(leftRat, rightRat);
        case SUB:
            return subtractRationals(leftRat, rightRat);
        case MUL:
            return multiplyRationals(leftRat, rightRat);
        case DIV:
            return divideRationals(leftRat, rightRat);
        case POW: {
            // Exponent must be a non-negative integer constant
            // The right-side rational must have denominator 1 and a constant numerator
            if (!isZeroPolynomial(subtractPolynomials(rightRat.den, onePolynomial()))) {
                throw std::runtime_error("Exponent must be a constant integer");
            }
            const Polynomial& expPoly = rightRat.num;
            if (expPoly.empty()) {
                // Exponent is 0: result is 1
                return { onePolynomial(), onePolynomial() };
            }
            if (expPoly.size() != 1) {
                throw std::runtime_error("Exponent must be a constant integer");
            }
            auto term = expPoly.begin();
            if (!term->first.empty()) {
                throw std::runtime_error("Exponent must be a constant integer (not a variable expression)");
            }
            int exp = static_cast<int>(term->second);
            if (exp < 0) {
                throw std::runtime_error("Negative exponents not supported");
            }
            return powerRational(leftRat, exp);
        }
    }
    return { Polynomial(), onePolynomial() };
}

// Negate a polynomial
Polynomial negatePolynomial(const Polynomial& p) {
    Polynomial result;
    for (const auto& term : p) {
        result[term.first] = -term.second;
    }
    return result;
}

// Add two polynomials
Polynomial addPolynomials(const Polynomial& a, const Polynomial& b) {
    Polynomial result = a;
    for (const auto& term : b) {
        result[term.first] += term.second;
        if (result[term.first] == 0) {
            result.erase(term.first);
        }
    }
    return result;
}

// Subtract two polynomials (implemented as addition of negated polynomial)
Polynomial subtractPolynomials(const Polynomial& a, const Polynomial& b) {
    return addPolynomials(a, negatePolynomial(b));
}

// Multiply a monomial by a monomial
Monomial multiplyMonomials(const Monomial& a, const Monomial& b) {
    Monomial result = a;
    for (const auto& var : b) {
        result[var.first] += var.second;
        if (result[var.first] == 0) {
            result.erase(var.first);
        }
    }
    return result;
}

// Multiply two polynomials
Polynomial multiplyPolynomials(const Polynomial& a, const Polynomial& b) {
    Polynomial result;
    for (const auto& termA : a) {
        for (const auto& termB : b) {
            Monomial mono = multiplyMonomials(termA.first, termB.first);
            long long coeff = termA.second * termB.second;
            result[mono] += coeff;
            if (result[mono] == 0) {
                result.erase(mono);
            }
        }
    }
    return result;
}

// Divide two polynomials (only works if result is polynomial)
Polynomial dividePolynomials(const Polynomial& a, const Polynomial& b) {
    // For simplicity, only support division by constants
    if (b.size() != 1 || !b.begin()->first.empty()) {
        throw std::runtime_error("Division only supported by constants");
    }
    long long divisor = b.begin()->second;
    if (divisor == 0) {
        throw std::runtime_error("Division by zero");
    }
    
    Polynomial result;
    for (const auto& term : a) {
        if (term.second % divisor != 0) {
            throw std::runtime_error("Division does not result in integer polynomial");
        }
        result[term.first] = term.second / divisor;
    }
    return result;
}

// Raise polynomial to integer power
Polynomial powerPolynomial(const Polynomial& a, int exp) {
    if (exp == 0) {
        Polynomial result;
        Monomial constant;
        result[constant] = 1;
        return result;
    }
    
    Polynomial result = a;
    for (int i = 1; i < exp; i++) {
        result = multiplyPolynomials(result, a);
    }
    return result;
}

// Check if polynomial is zero
bool isZeroPolynomial(const Polynomial& p) {
    return p.empty();
}

// Return the constant polynomial 1
Polynomial onePolynomial() {
    Polynomial p;
    p[Monomial{}] = 1;
    return p;
}

// Add two rational polynomials: a/b + c/d = (a*d + c*b) / (b*d)
RationalPolynomial addRationals(const RationalPolynomial& a, const RationalPolynomial& b) {
    return {
        addPolynomials(multiplyPolynomials(a.num, b.den), multiplyPolynomials(b.num, a.den)),
        multiplyPolynomials(a.den, b.den)
    };
}

// Subtract two rational polynomials: a/b - c/d = (a*d - c*b) / (b*d)
RationalPolynomial subtractRationals(const RationalPolynomial& a, const RationalPolynomial& b) {
    return {
        subtractPolynomials(multiplyPolynomials(a.num, b.den), multiplyPolynomials(b.num, a.den)),
        multiplyPolynomials(a.den, b.den)
    };
}

// Multiply two rational polynomials: (a/b) * (c/d) = (a*c) / (b*d)
RationalPolynomial multiplyRationals(const RationalPolynomial& a, const RationalPolynomial& b) {
    return {
        multiplyPolynomials(a.num, b.num),
        multiplyPolynomials(a.den, b.den)
    };
}

// Divide two rational polynomials: (a/b) / (c/d) = (a*d) / (b*c)
RationalPolynomial divideRationals(const RationalPolynomial& a, const RationalPolynomial& b) {
    if (isZeroPolynomial(b.num)) {
        throw std::runtime_error("Division by zero polynomial: denominator is identically zero for all variable values");
    }
    return {
        multiplyPolynomials(a.num, b.den),
        multiplyPolynomials(a.den, b.num)
    };
}

// Raise rational polynomial to a non-negative integer power
RationalPolynomial powerRational(const RationalPolynomial& a, int exp) {
    return {
        powerPolynomial(a.num, exp),
        powerPolynomial(a.den, exp)
    };
}

// Check equality of two rational polynomials: a/b = c/d iff a*d - c*b = 0
bool rationalEqual(const RationalPolynomial& a, const RationalPolynomial& b) {
    Polynomial diff = subtractPolynomials(
        multiplyPolynomials(a.num, b.den),
        multiplyPolynomials(b.num, a.den)
    );
    return isZeroPolynomial(diff);
}
