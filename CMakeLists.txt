cmake_minimum_required(VERSION 3.10)
project(PearlProof)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set ANTLR4 paths
set(ANTLR4_INCLUDE_DIR /usr/include/antlr4-runtime)
set(ANTLR4_LIB_DIR /usr/lib/x86_64-linux-gnu)
set(ANTLR4_JAR ${CMAKE_CURRENT_SOURCE_DIR}/antlr-4.10-complete.jar)

# Find java
find_program(JAVA_EXECUTABLE java)
if(NOT JAVA_EXECUTABLE)
    message(FATAL_ERROR "Java executable not found. Please install Java.")
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
           ${GENERATED_DIR}/PearlProofLexer.h
           ${GENERATED_DIR}/PearlProofParser.h
           ${GENERATED_DIR}/PearlProofBaseVisitor.h
    COMMAND ${JAVA_EXECUTABLE} -jar ${ANTLR4_JAR} -Dlanguage=Cpp -visitor -no-listener -o ${GENERATED_DIR} ${GRAMMAR_FILE}
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
)

# Create executable
add_executable(pearlproof ${SOURCES})

# Link ANTLR4 runtime
target_link_libraries(pearlproof ${ANTLR4_LIB_DIR}/libantlr4-runtime.a)
