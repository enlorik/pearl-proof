#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include <string>
#include <map>
#include <vector>

// Forward declarations
class Expr;
using ExprPtr = std::shared_ptr<Expr>;

// Monomial: map of variable -> exponent
using Monomial = std::map<std::string, int>;
// Polynomial: map of monomial -> coefficient
using Polynomial = std::map<Monomial, long long>;

// Abstract base class for expressions
class Expr {
public:
    virtual ~Expr() = default;
    virtual Polynomial toPolynomial() const = 0;
};

// Integer constant
class IntExpr : public Expr {
public:
    long long value;
    explicit IntExpr(long long v) : value(v) {}
    Polynomial toPolynomial() const override;
};

// Variable
class VarExpr : public Expr {
public:
    std::string name;
    explicit VarExpr(const std::string& n) : name(n) {}
    Polynomial toPolynomial() const override;
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
};

// Helper functions for polynomial operations
Polynomial addPolynomials(const Polynomial& a, const Polynomial& b);
Polynomial subtractPolynomials(const Polynomial& a, const Polynomial& b);
Polynomial multiplyPolynomials(const Polynomial& a, const Polynomial& b);
Polynomial dividePolynomials(const Polynomial& a, const Polynomial& b);
Polynomial powerPolynomial(const Polynomial& a, int exp);
bool isZeroPolynomial(const Polynomial& p);

#endif // EXPR_H
