#ifndef PEARLPROOF_AST_VISITOR_H
#define PEARLPROOF_AST_VISITOR_H

#include "PearlProofBaseVisitor.h"
#include "Expr.h"
#include <any>
#include <string>

// Check status for an equation line
enum class CheckStatus {
    OK,          // equality proved by PearlProof
    ERROR,       // found to be false or contains an invalid expression
    UNSUPPORTED  // outside the current checker's supported domain
};

// Structure to hold check results
struct CheckResult {
    int lineNum;
    CheckStatus status;
    std::string message;
};

class PearlProofASTVisitor : public PearlProofBaseVisitor {
public:
    antlrcpp::Any visitFile(PearlProofParser::FileContext *ctx) override;
    antlrcpp::Any visitLine(PearlProofParser::LineContext *ctx) override;
    antlrcpp::Any visitInt(PearlProofParser::IntContext *ctx) override;
    antlrcpp::Any visitVar(PearlProofParser::VarContext *ctx) override;
    antlrcpp::Any visitParens(PearlProofParser::ParensContext *ctx) override;
    antlrcpp::Any visitUnaryMinus(PearlProofParser::UnaryMinusContext *ctx) override;
    antlrcpp::Any visitAddSub(PearlProofParser::AddSubContext *ctx) override;
    antlrcpp::Any visitMulDiv(PearlProofParser::MulDivContext *ctx) override;
    antlrcpp::Any visitPower(PearlProofParser::PowerContext *ctx) override;
};

#endif // PEARLPROOF_AST_VISITOR_H
