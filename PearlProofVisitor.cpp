#include "PearlProofVisitor.h"
#include "PearlProofParser.h"
#include <iostream>
#include <vector>

antlrcpp::Any PearlProofASTVisitor::visitFile(PearlProofParser::FileContext *ctx) {
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

antlrcpp::Any PearlProofASTVisitor::visitLine(PearlProofParser::LineContext *ctx) {
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

antlrcpp::Any PearlProofASTVisitor::visitInt(PearlProofParser::IntContext *ctx) {
    long long value = std::stoll(ctx->INT()->getText());
    return ExprPtr(new IntExpr(value));
}

antlrcpp::Any PearlProofASTVisitor::visitVar(PearlProofParser::VarContext *ctx) {
    std::string name = ctx->ID()->getText();
    return ExprPtr(new VarExpr(name));
}

antlrcpp::Any PearlProofASTVisitor::visitParens(PearlProofParser::ParensContext *ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any PearlProofASTVisitor::visitUnaryMinus(PearlProofParser::UnaryMinusContext *ctx) {
    ExprPtr operand = std::any_cast<ExprPtr>(visit(ctx->expr()));
    return ExprPtr(new UnaryExpr(UnaryExpr::NEG, operand));
}

antlrcpp::Any PearlProofASTVisitor::visitAddSub(PearlProofParser::AddSubContext *ctx) {
    ExprPtr left = std::any_cast<ExprPtr>(visit(ctx->expr(0)));
    ExprPtr right = std::any_cast<ExprPtr>(visit(ctx->expr(1)));
    
    // Get the operator from the middle token
    std::string opText = ctx->children[1]->getText();
    BinOpExpr::Op op = (opText == "+") ? BinOpExpr::ADD : BinOpExpr::SUB;
    
    return ExprPtr(new BinOpExpr(op, left, right));
}

antlrcpp::Any PearlProofASTVisitor::visitMulDiv(PearlProofParser::MulDivContext *ctx) {
    ExprPtr left = std::any_cast<ExprPtr>(visit(ctx->expr(0)));
    ExprPtr right = std::any_cast<ExprPtr>(visit(ctx->expr(1)));
    
    // Get the operator from the middle token
    std::string opText = ctx->children[1]->getText();
    BinOpExpr::Op op = (opText == "*") ? BinOpExpr::MUL : BinOpExpr::DIV;
    
    return ExprPtr(new BinOpExpr(op, left, right));
}

antlrcpp::Any PearlProofASTVisitor::visitPower(PearlProofParser::PowerContext *ctx) {
    ExprPtr left = std::any_cast<ExprPtr>(visit(ctx->expr(0)));
    ExprPtr right = std::any_cast<ExprPtr>(visit(ctx->expr(1)));
    
    return ExprPtr(new BinOpExpr(BinOpExpr::POW, left, right));
}
