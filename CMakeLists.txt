cmake_minimum_required(VERSION 3.10)
project(PearlProof)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ANTLR4 runtime
find_package(antlr4-runtime REQUIRED)

# Generate ANTLR4 parser and lexer
find_program(ANTLR4_EXECUTABLE antlr4)
if(NOT ANTLR4_EXECUTABLE)
    message(FATAL_ERROR "ANTLR4 executable not found. Please install ANTLR4.")
endif()

set(GRAMMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/PearlProof.g4)
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

# Create output directory
file(MAKE_DIRECTORY ${GENERATED_DIR})

# Generate parser and lexer
add_custom_command(
    OUTPUT ${GENERATED_DIR}/PearlProofLexer.cpp
           ${GENERATED_DIR}/PearlProofParser.cpp
           ${GENERATED_DIR}/PearlProofBaseVisitor.cpp
           ${GENERATED_DIR}/PearlProofVisitor.cpp
           ${GENERATED_DIR}/PearlProofLexer.h
           ${GENERATED_DIR}/PearlProofParser.h
           ${GENERATED_DIR}/PearlProofBaseVisitor.h
           ${GENERATED_DIR}/PearlProofVisitor.h
    COMMAND ${ANTLR4_EXECUTABLE} -Dlanguage=Cpp -visitor -no-listener -o ${GENERATED_DIR} ${GRAMMAR_FILE}
    DEPENDS ${GRAMMAR_FILE}
    COMMENT "Generating ANTLR4 parser and lexer"
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GENERATED_DIR}
    ${ANTLR4_INCLUDE_DIR}
)

# Source files
set(SOURCES
    main.cpp
    Expr.cpp
    PearlProofVisitor.cpp
    ${GENERATED_DIR}/PearlProofLexer.cpp
    ${GENERATED_DIR}/PearlProofParser.cpp
    ${GENERATED_DIR}/PearlProofBaseVisitor.cpp
    ${GENERATED_DIR}/PearlProofVisitor.cpp
)

# Create executable
add_executable(pearlproof ${SOURCES})

# Link ANTLR4 runtime
target_link_libraries(pearlproof antlr4_static)
