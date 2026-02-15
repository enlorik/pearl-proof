#include <iostream>
#include <fstream>
#include <string>
#include "antlr4-runtime.h"
#include "PearlProofLexer.h"
#include "PearlProofParser.h"
#include "PearlProofVisitor.h"

using namespace antlr4;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::ifstream stream(inputFile);
    
    if (!stream.is_open()) {
        std::cerr << "Error: Cannot open file " << inputFile << std::endl;
        return 1;
    }
    
    ANTLRInputStream input(stream);
    PearlProofLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    PearlProofParser parser(&tokens);
    
    // Parse the file
    PearlProofParser::FileContext* tree = parser.file();
    
    // Check for parse errors
    if (parser.getNumberOfSyntaxErrors() > 0) {
        std::cerr << "Parse errors encountered" << std::endl;
        return 1;
    }
    
    // Visit the parse tree
    PearlProofVisitor visitor;
    auto result = visitor.visitFile(tree);
    
    // Get results
    auto results = std::any_cast<std::vector<CheckResult>>(result);
    
    // Output results
    bool allPassed = true;
    for (const auto& check : results) {
        if (check.success) {
            std::cout << "Line " << check.lineNum << ": OK" << std::endl;
        } else {
            std::cout << "Line " << check.lineNum << ": ERROR" << std::endl;
            allPassed = false;
        }
    }
    
    return allPassed ? 0 : 1;
}
