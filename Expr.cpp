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
            if (rightPoly.size() != 1 || !rightPoly.begin()->first.empty()) {
                throw std::runtime_error("Exponent must be a constant integer");
            }
            int exp = rightPoly.begin()->second;
            if (exp < 0) {
                throw std::runtime_error("Negative exponents not supported");
            }
            return powerPolynomial(leftPoly, exp);
        }
    }
    return Polynomial();
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

// Subtract two polynomials
Polynomial subtractPolynomials(const Polynomial& a, const Polynomial& b) {
    Polynomial result = a;
    for (const auto& term : b) {
        result[term.first] -= term.second;
        if (result[term.first] == 0) {
            result.erase(term.first);
        }
    }
    return result;
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
