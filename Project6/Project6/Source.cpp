#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cctype>

using namespace std;

enum TokenType {
    INT,
    IDENTIFIER,
    NUMBER,
    ASSIGN,
    MULTIPLY,
    DIVIDE,
    PLUS,
    MINUS,
    SEMICOLON,
    END
};

struct Token {
    TokenType type;
    string value;
};

class Lexer {
public:
    Lexer(const string& source) : source(source), position(0) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (position < source.size()) {
            char current = source[position];
            if (isspace(current)) {
                position++;
            }
            else if (current == '+') {
                tokens.push_back({ PLUS, "+" });
                position++;
            }
            else if (current == '-') {
                tokens.push_back({ MINUS, "-" });
                position++;
            }
            else if (current == '*') {
                tokens.push_back({ MULTIPLY, "*" });
                position++;
            }
            else if (current == '/') {
                tokens.push_back({ DIVIDE, "/" });
                position++;
            }
            else if (current == '=') {
                tokens.push_back({ ASSIGN, "=" });
                position++;
            }
            else if (current == ';') {
                tokens.push_back({ SEMICOLON, ";" });
                position++;
            }
            else if (isdigit(current)) {
                tokens.push_back(number());
            }
            else if (isalpha(current)) {
                tokens.push_back(identifier());
            }
            else {
                cerr << "Unknown character: " << current << endl;
                position++;
            }
        }
        tokens.push_back({ END, "" });
        return tokens;
    }

private:
    Token number() {
        string value;
        while (position < source.size() && isdigit(source[position])) {
            value += source[position++];
        }
        return { NUMBER, value };
    }

    Token identifier() {
        string value;
        while (position < source.size() && isalnum(source[position])) {
            value += source[position++];
        }
        if (value == "int") {
            return { INT, value };
        }
        return { IDENTIFIER, value };
    }

    string source;
    size_t position;
};

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print(int indent = 0) const = 0;
};

struct NumberNode : ASTNode {
    string value;
    explicit NumberNode(const string& value) : value(value) {}
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "NumberNode(" << value << ")" << endl;
    }
};

struct IdentifierNode : ASTNode {
    string name;
    explicit IdentifierNode(const string& name) : name(name) {}
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "IdentifierNode(" << name << ")" << endl;
    }
};

struct BinaryOperationNode : ASTNode {
    string op;
    unique_ptr<ASTNode> left;
    unique_ptr<ASTNode> right;

    BinaryOperationNode(string op, unique_ptr<ASTNode> left, unique_ptr<ASTNode> right)
        : op(op), left(move(left)), right(move(right)) {}
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "BinaryOperationNode(" << op << ")" << endl;
        left->print(indent + 2);
        right->print(indent + 2);
    }
};

struct AssignmentNode : ASTNode {
    unique_ptr<IdentifierNode> identifier;
    unique_ptr<ASTNode> expression;

    AssignmentNode(unique_ptr<IdentifierNode> identifier, unique_ptr<ASTNode> expression)
        : identifier(move(identifier)), expression(move(expression)) {}
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "AssignmentNode" << endl;
        identifier->print(indent + 2);
        expression->print(indent + 2);
    }
};

struct DeclarationNode : ASTNode {
    string type;
    unique_ptr<AssignmentNode> assignment;

    DeclarationNode(string type, unique_ptr<AssignmentNode> assignment)
        : type(type), assignment(move(assignment)) {}
    void print(int indent = 0) const override {
        cout << string(indent, ' ') << "DeclarationNode(" << type << ")" << endl;
        assignment->print(indent + 2);
    }
};

class Parser {
public:
    explicit Parser(const vector<Token>& tokens) : tokens(tokens), position(0) {}

    vector<unique_ptr<ASTNode>> parse() {
        vector<unique_ptr<ASTNode>> statements;
        while (!isAtEnd()) {
            auto stmt = parseDeclaration();
            if (stmt) {
                statements.push_back(move(stmt));
            }
            else {
                break;
            }
        }
        return statements;
    }

private:
    unique_ptr<ASTNode> parseDeclaration() {
        if (match(INT)) {
            string type = previous().value;
            unique_ptr<IdentifierNode> identifier = make_unique<IdentifierNode>(consume(IDENTIFIER, "Expect identifier").value);
            consume(ASSIGN, "Expect '=' after variable name");
            unique_ptr<ASTNode> expression = parseExpression();
            consume(SEMICOLON, "Expect ';' after expression");
            return make_unique<DeclarationNode>(type, make_unique<AssignmentNode>(move(identifier), move(expression)));
        }
        return nullptr;
    }

    unique_ptr<ASTNode> parseExpression() {
        return parseTerm();
    }

    unique_ptr<ASTNode> parseTerm() {
        unique_ptr<ASTNode> node = parseFactor();

        while (match(MULTIPLY) || match(DIVIDE) || match(PLUS) || match(MINUS)) {
            string op = previous().value;
            unique_ptr<ASTNode> right = parseFactor();
            node = make_unique<BinaryOperationNode>(op, move(node), move(right));
        }

        return node;
    }

    unique_ptr<ASTNode> parseFactor() {
        if (match(NUMBER)) {
            return make_unique<NumberNode>(previous().value);
        }
        if (match(IDENTIFIER)) {
            return make_unique<IdentifierNode>(previous().value);
        }
        cerr << "Unexpected token: " << peek().value << endl;
        return nullptr;
    }

    bool match(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    bool check(TokenType type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    Token advance() {
        if (!isAtEnd()) position++;
        return previous();
    }

    bool isAtEnd() {
        return peek().type == END;
    }

    Token peek() {
        return tokens[position];
    }

    Token previous() {
        return tokens[position - 1];
    }

    Token consume(TokenType type, const string& message) {
        if (check(type)) return advance();
        cerr << message << endl;
        return {};
    }

    const vector<Token>& tokens;
    size_t position;
};

void printTokens(const vector<Token>& tokens) {
    for (const auto& token : tokens) {
        cout << "Token(";
        switch (token.type) {
        case INT: cout << "INT"; break;
        case IDENTIFIER: cout << "IDENTIFIER"; break;
        case NUMBER: cout << "NUMBER"; break;
        case ASSIGN: cout << "ASSIGN"; break;
        case MULTIPLY: cout << "MULTIPLY"; break;
        case DIVIDE: cout << "DIVIDE"; break;
        case PLUS: cout << "PLUS"; break;
        case MINUS: cout << "MINUS"; break;
        case SEMICOLON: cout << "SEMICOLON"; break;
        case END: cout << "END"; break;
        }
        cout << ", " << token.value << ")" << endl;
    }
}

int main() {
    string source = "int sum = a * b; int total = sum + 10;";

    Lexer lexer(source);
    vector<Token> tokens = lexer.tokenize();

    cout << "Tokens:" << endl;
    printTokens(tokens);

    Parser parser(tokens);
    vector<unique_ptr<ASTNode>> syntaxTree = parser.parse();

    if (!syntaxTree.empty()) {
        cout << "Parsing completed successfully." << endl;
        cout << "Parse Tree:" << endl;
        for (const auto& node : syntaxTree) {
            node->print();
        }
    }
    else {
        cout << "Parsing failed." << endl;
    }

    return 0;
}