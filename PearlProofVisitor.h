#ifndef PEARLPROOF_VISITOR_H
#define PEARLPROOF_VISITOR_H

#include "PearlProofBaseVisitor.h"
#include "Expr.h"
#include <any>

// Structure to hold check results
struct CheckResult {
    int lineNum;
    bool success;
};

class PearlProofVisitor : public PearlProofBaseVisitor {
public:
    std::any visitFile(PearlProofParser::FileContext *ctx) override;
    std::any visitLine(PearlProofParser::LineContext *ctx) override;
    std::any visitInt(PearlProofParser::IntContext *ctx) override;
    std::any visitVar(PearlProofParser::VarContext *ctx) override;
    std::any visitParens(PearlProofParser::ParensContext *ctx) override;
    std::any visitAddSub(PearlProofParser::AddSubContext *ctx) override;
    std::any visitMulDiv(PearlProofParser::MulDivContext *ctx) override;
    std::any visitPower(PearlProofParser::PowerContext *ctx) override;
};

#endif // PEARLPROOF_VISITOR_H
