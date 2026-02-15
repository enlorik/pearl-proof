#include "PearlProofVisitor.h"
#include "PearlProofParser.h"
#include <iostream>
#include <vector>

std::any PearlProofVisitor::visitFile(PearlProofParser::FileContext *ctx) {
    std::vector<CheckResult> results;
    int lineNum = 1;
    
    for (auto lineCtx : ctx->line()) {
        auto result = visit(lineCtx);
        CheckResult check;
        check.lineNum = lineNum++;
        check.success = std::any_cast<bool>(result);
        results.push_back(check);
    }
    
    return results;
}

std::any PearlProofVisitor::visitLine(PearlProofParser::LineContext *ctx) {
    // Get the two expressions
    auto exprs = ctx->expr();
    ExprPtr left = std::any_cast<ExprPtr>(visit(exprs[0]));
    ExprPtr right = std::any_cast<ExprPtr>(visit(exprs[1]));
    
    // Convert to polynomials
    try {
        Polynomial leftPoly = left->toPolynomial();
        Polynomial rightPoly = right->toPolynomial();
        
        // Compute difference
        Polynomial diff = subtractPolynomials(leftPoly, rightPoly);
        
        // Check if zero
        return isZeroPolynomial(diff);
    } catch (const std::exception& e) {
        std::cerr << "Error evaluating expression: " << e.what() << std::endl;
        return false;
    }
}

std::any PearlProofVisitor::visitInt(PearlProofParser::IntContext *ctx) {
    long long value = std::stoll(ctx->INT()->getText());
    return ExprPtr(new IntExpr(value));
}

std::any PearlProofVisitor::visitVar(PearlProofParser::VarContext *ctx) {
    std::string name = ctx->ID()->getText();
    return ExprPtr(new VarExpr(name));
}

std::any PearlProofVisitor::visitParens(PearlProofParser::ParensContext *ctx) {
    return visit(ctx->expr());
}

std::any PearlProofVisitor::visitAddSub(PearlProofParser::AddSubContext *ctx) {
    ExprPtr left = std::any_cast<ExprPtr>(visit(ctx->expr(0)));
    ExprPtr right = std::any_cast<ExprPtr>(visit(ctx->expr(1)));
    
    BinOpExpr::Op op = (ctx->children[1]->getText() == "+") ? BinOpExpr::ADD : BinOpExpr::SUB;
    
    return ExprPtr(new BinOpExpr(op, left, right));
}

std::any PearlProofVisitor::visitMulDiv(PearlProofParser::MulDivContext *ctx) {
    ExprPtr left = std::any_cast<ExprPtr>(visit(ctx->expr(0)));
    ExprPtr right = std::any_cast<ExprPtr>(visit(ctx->expr(1)));
    
    BinOpExpr::Op op = (ctx->children[1]->getText() == "*") ? BinOpExpr::MUL : BinOpExpr::DIV;
    
    return ExprPtr(new BinOpExpr(op, left, right));
}

std::any PearlProofVisitor::visitPower(PearlProofParser::PowerContext *ctx) {
    ExprPtr left = std::any_cast<ExprPtr>(visit(ctx->expr(0)));
    ExprPtr right = std::any_cast<ExprPtr>(visit(ctx->expr(1)));
    
    return ExprPtr(new BinOpExpr(BinOpExpr::POW, left, right));
}
