Mini Compiler Construction Report
1. Introduction
1.1 Project Objectives
The objective of this project is to develop a mini compiler using C++ that can process a simple programming language. The compiler performs lexical analysis, parsing, and abstract syntax tree (AST) generation to handle basic arithmetic expressions and variable assignments.
1.2 Scope
The mini compiler is designed to:
•	Recognize integer declarations and assignments.
•	Handle arithmetic operations: addition, subtraction, multiplication, and division.
•	Generate an abstract syntax tree (AST) to represent the parsed code.
2. Design and Architecture
2.1 Overview
The compiler is divided into several key components:
•	Lexical Analysis (Lexer): Converts the input source code into a series of tokens.
•	Syntax Analysis (Parser): Parses the tokens to generate an abstract syntax tree (AST).
•	AST Representation: Represents the structure of the parsed code for further processing or analysis.
2.2 Component Details
2.2.1 Lexical Analysis
The Lexer class tokenizes the input source code by recognizing keywords, operators, identifiers, and literals. It handles whitespace and identifies unknown characters.
2.2.2 Syntax Analysis
The Parser class processes the tokens generated by the lexer to construct an AST. It supports parsing of:
•	Integer variable declarations and assignments.
•	Arithmetic expressions involving addition, subtraction, multiplication, and division.
2.2.3 AST Representation
The AST nodes represent different constructs of the language, including numbers, identifiers, binary operations, assignments, and declarations.
3. Implementation Details
3.1 Lexer Implementation
The Lexer class is implemented with a method tokenize that reads characters from the source code and generates tokens. Key functions include number and identifier for recognizing numbers and identifiers, respectively.
class Lexer {
public:
    Lexer(const string& source) : source(source), position(0) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        // Tokenization logic...
        return tokens;
    }

private:
    Token number() {
        // Logic for number tokenization...
    }

    Token identifier() {
        // Logic for identifier tokenization...
    }

    string source;
    size_t position;
};
3.2 Parser Implementation
The Parser class uses recursive descent parsing to build the AST. It includes methods to parse declarations, expressions, terms, and factors.
class Parser {
public:
    explicit Parser(const vector<Token>& tokens) : tokens(tokens), position(0) {}

    vector<unique_ptr<ASTNode>> parse() {
        vector<unique_ptr<ASTNode>> statements;
        // Parsing logic...
        return statements;
    }

private:
    unique_ptr<ASTNode> parseDeclaration() {
        // Logic for parsing declarations...
    }

    unique_ptr<ASTNode> parseExpression() {
        // Logic for parsing expressions...
    }

    // Additional helper methods...
};
3.3 AST Node Implementation
Various AST nodes represent different parts of the syntax. For instance, NumberNode, IdentifierNode, BinaryOperationNode, AssignmentNode, and DeclarationNode.
struct NumberNode : ASTNode {
    string value;
    explicit NumberNode(const string& value) : value(value) {}
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "NumberNode(" << value << ")" << endl;
    }
};

// Similar implementations for other node types...

4. Testing and Validation
4.1 Testing Strategy
The compiler is tested with various input strings to ensure that the lexer, parser, and AST generation work correctly. Test cases include:
•	Valid integer declarations and assignments.
•	Arithmetic expressions with different operators.
•	Edge cases with unknown characters and invalid syntax.
4.2 Validation Results
The test cases confirm that the lexer correctly tokenizes the input and the parser generates the appropriate AST. For example:
Input Source Code:
int sum = a * b; int total = sum + 10;
Generated Tokens:
Token(INT, "int"), Token(IDENTIFIER, "sum"), Token(ASSIGN, "="), Token(IDENTIFIER, "a"),
Token(MULTIPLY, "*"), Token(IDENTIFIER, "b"), Token(SEMICOLON, ";"),
Token(INT, "int"), Token(IDENTIFIER, "total"), Token(ASSIGN, "="), Token(IDENTIFIER, "sum"),
Token(PLUS, "+"), Token(NUMBER, "10"), Token(SEMICOLON, ";"), Token(END, "")

Generated AST:
DeclarationNode(int)
  AssignmentNode
    IdentifierNode(sum)
    BinaryOperationNode(*)
      IdentifierNode(a)
      IdentifierNode(b)
DeclarationNode(int)
  AssignmentNode
    IdentifierNode(total)
    BinaryOperationNode(+)
      IdentifierNode(sum)
      NumberNode(10)
5. Results and Evaluation
5.1 Performance Metrics
The mini compiler performs efficiently for small inputs, with negligible processing time. For larger inputs, performance remains acceptable, demonstrating good scalability for the intended scope.
5.2 Evaluation
The mini compiler successfully meets its design goals, correctly handling variable declarations, assignments, and arithmetic expressions. The generated AST provides a clear and correct representation of the input source code.
6. Conclusion
6.1 Summary
The mini compiler project successfully demonstrates the fundamental concepts of compiler construction, including lexical analysis, parsing, and AST generation. The implementation in C++ effectively handles a simple language syntax.
6.2 Challenges
Key challenges included managing recursive parsing logic and ensuring robust error handling. These were addressed through careful design and iterative testing.
6.3 Future Enhancements
Potential future enhancements include:
•	Extending the language syntax to support additional constructs (e.g., conditionals, loops).
•	Implementing semantic analysis to check for type errors.
•	Adding optimization passes to improve generated code performance.
7. References
•	[The Dragon Book: Compilers: Principles, Techniques, and Tools by Alfred V. Aho, Monica S. Lam, Ravi Sethi, and Jeffrey D. Ullman]
•	[LLVM Compiler Infrastructure Project]
•	[Various online resources and tutorials on compiler construction]
