#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>

// Exception for operations outside PearlProof's supported domain
class UnsupportedOperationError : public std::runtime_error {
public:
    explicit UnsupportedOperationError(const std::string& msg)
        : std::runtime_error(msg) {}
};

// Forward declarations
class Expr;
using ExprPtr = std::shared_ptr<Expr>;

// Monomial: map of variable -> exponent
using Monomial = std::map<std::string, int>;
// Polynomial: map of monomial -> coefficient
using Polynomial = std::map<Monomial, long long>;

// Rational polynomial: numerator and denominator as polynomials
struct RationalPolynomial {
    Polynomial num; // numerator
    Polynomial den; // denominator
};

// Abstract base class for expressions
class Expr {
public:
    virtual ~Expr() = default;
    virtual Polynomial toPolynomial() const = 0;
    virtual RationalPolynomial toRationalPolynomial() const = 0;
};

// Integer constant
class IntExpr : public Expr {
public:
    long long value;
    explicit IntExpr(long long v) : value(v) {}
    Polynomial toPolynomial() const override;
    RationalPolynomial toRationalPolynomial() const override;
};

// Variable
class VarExpr : public Expr {
public:
    std::string name;
    explicit VarExpr(const std::string& n) : name(n) {}
    Polynomial toPolynomial() const override;
    RationalPolynomial toRationalPolynomial() const override;
};

// Unary operation
class UnaryExpr : public Expr {
public:
    enum Op { NEG };
    Op op;
    ExprPtr operand;
    
    UnaryExpr(Op o, ExprPtr e) : op(o), operand(e) {}
    Polynomial toPolynomial() const override;
    RationalPolynomial toRationalPolynomial() const override;
};

// Binary operation
class BinOpExpr : public Expr {
public:
    enum Op { ADD, SUB, MUL, DIV, POW };
    Op op;
    ExprPtr left;
    ExprPtr right;
    
    BinOpExpr(Op o, ExprPtr l, ExprPtr r) : op(o), left(l), right(r) {}
    Polynomial toPolynomial() const override;
    RationalPolynomial toRationalPolynomial() const override;
};

// Helper functions for polynomial operations
Polynomial negatePolynomial(const Polynomial& p);
Polynomial addPolynomials(const Polynomial& a, const Polynomial& b);
Polynomial subtractPolynomials(const Polynomial& a, const Polynomial& b);
Polynomial multiplyPolynomials(const Polynomial& a, const Polynomial& b);
Polynomial dividePolynomials(const Polynomial& a, const Polynomial& b);
Polynomial powerPolynomial(const Polynomial& a, int exp);
bool isZeroPolynomial(const Polynomial& p);

// Helper functions for rational polynomial operations
Polynomial onePolynomial();
RationalPolynomial addRationals(const RationalPolynomial& a, const RationalPolynomial& b);
RationalPolynomial subtractRationals(const RationalPolynomial& a, const RationalPolynomial& b);
RationalPolynomial multiplyRationals(const RationalPolynomial& a, const RationalPolynomial& b);
RationalPolynomial divideRationals(const RationalPolynomial& a, const RationalPolynomial& b);
RationalPolynomial powerRational(const RationalPolynomial& a, int exp);
bool rationalEqual(const RationalPolynomial& a, const RationalPolynomial& b);

#endif // EXPR_H
