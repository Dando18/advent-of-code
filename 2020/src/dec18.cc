/*
    This was a way over-the-top solution implementing a lexer
    and recursive-descent parser, but it works 
*/

#include <cstdio>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>

enum TokenType {
    PLUS, MUL, LPAREN, RPAREN, VAL, INVALID
};

struct Token {
    TokenType type;
    std::string val;
};

class Lexer {
    public:
    Lexer(std::string const& text) : tokens(), s(text), curIdx(0) {
        s.erase(std::remove(std::begin(s), std::end(s), ' '), std::end(s));
        
        std::string num = "";
        for (size_t i = 0; i < s.size(); i++) {
            const char c = s.at(i);
            if (c == '(') {
                tokens.push_back({LPAREN, "("});
            } else if (c == ')') {
                tokens.push_back({RPAREN, ")"});
            } else if (c == '+') {
                tokens.push_back({PLUS, "+"});
            } else if (c == '*') {
                tokens.push_back({MUL, "*"});
            } else if (std::isdigit(c)) {
                num.push_back(c);
                if (i == s.size()-1 || !std::isdigit(s.at(i+1))) {
                    tokens.push_back({VAL, num});
                    num = "";
                }
            } else {
                fprintf(stderr, "unknown token: %c\n", c);
            }
        }
    }
    Token nextToken() {
        auto token = tokens.at(curIdx);
        curIdx += 1;
        return token;
    }

    std::vector<Token> tokens;
    std::string s;
    int curIdx;
};

class Expr {
    public:
    virtual ~Expr() = default;
    virtual int64_t eval() = 0;
};

class Value : public Expr {
    int64_t val_;
    public:
    Value(int64_t val) : val_(val) {}

    virtual int64_t eval() { return val_; }
};

class Op : public Expr {
    Expr *left_, *right_;
    char op_;
    public:
    Op(Expr *left, char op, Expr *right) : left_(left), right_(right), op_(op) {}
    Op(const Op& o) = default;
    Op& operator=(const Op& o) = default;

    virtual int64_t eval() { 
        switch (op_) {
            case '+': return left_->eval() + right_->eval();
            case '*': return left_->eval() * right_->eval();
            default:
                fprintf(stderr, "Uknown op %c\n", op_);
                return -1;
        }
    }
};

class Parser {
    public:
    Parser(std::string const &s) : lex(Lexer(s)), curTokenIdx(0), currentToken(lex.tokens.front()) {}

    Token getNextToken() {
        curTokenIdx += 1;
        if (curTokenIdx == lex.tokens.size()) {
            return {INVALID, ""};
        } else {
            return lex.tokens.at(curTokenIdx);
        }
    }

    /*  expr : term ((MUL|PLUS) term)*
        term : VAL | LPAREN expr RPAREN
    */

    void match(TokenType t) {
        if (currentToken.type == t) {
            currentToken = getNextToken();
        } else {
            fprintf(stderr, "Invalid match when parsing %s\n", currentToken.val.c_str());
        }
    }

    Expr *expr() {
        /* expr : term (MUL term) */
        auto e = term();

        while (currentToken.type == MUL || currentToken.type == PLUS) {
            if (currentToken.type == MUL) {
                match(MUL);
                e = new Op(e, '*', term());
            } else if (currentToken.type == PLUS) {
                match(PLUS);
                e = new Op(e, '+', term());
            }
        }
        return e;
    }

    Expr *term() {
        /* term : VAL | LPAREN expr RPAREN */
        Token token = currentToken;

        if (token.type == VAL) {
            match(VAL);
            return new Value(std::stoll(token.val));
        } else if (token.type == LPAREN) {
            match(LPAREN);
            auto e = expr();
            match(RPAREN);
            return e;
        }
        return nullptr;
    }

    Expr *parse() {
        return expr();
    }

    Lexer lex;
    size_t curTokenIdx;
    Token currentToken;
};

class ParserWithAdditionPrecedence {
    public:
    ParserWithAdditionPrecedence(std::string const &s) : lex(Lexer(s)), curTokenIdx(0), currentToken(lex.tokens.front()) {}

    Token getNextToken() {
        curTokenIdx += 1;
        if (curTokenIdx == lex.tokens.size()) {
            return {INVALID, ""};
        } else {
            return lex.tokens.at(curTokenIdx);
        }
    }

    /*  expr : term (PLUS term)*
        term : factor (MUL factor)*
        factor : VAL | LPAREN expr RPAREN
    */

    void match(TokenType t) {
        if (currentToken.type == t) {
            currentToken = getNextToken();
        } else {
            fprintf(stderr, "Invalid match when parsing %s\n", currentToken.val.c_str());
        }
    }

    Expr *expr() {
        /* expr : term (MUL term) */
        auto e = term();

        while (currentToken.type == MUL) {
            match(MUL);
            e = new Op(e, '*', term());
        }
        return e;
    }

    Expr *term() {
        /* expr : term (PLUS term) */
        auto e = factor();

        while (currentToken.type == PLUS) {
            match(PLUS);
            e = new Op(e, '+', factor());
        }
        return e;
    }

    Expr *factor() {
        /* term : VAL | LPAREN expr RPAREN */
        Token token = currentToken;

        if (token.type == VAL) {
            match(VAL);
            return new Value(std::stoll(token.val));
        } else if (token.type == LPAREN) {
            match(LPAREN);
            auto e = expr();
            match(RPAREN);
            return e;
        }
        return nullptr;
    }

    Expr *parse() {
        return expr();
    }

    Lexer lex;
    size_t curTokenIdx;
    Token currentToken;
};

int64_t evaluate(std::string const& expr, bool additionPrecedence=false) {

    Expr *root;
    if (additionPrecedence) {
        ParserWithAdditionPrecedence p(expr);
        root = p.parse();
    } else {
        Parser p(expr);
        root = p.parse();
    }

    return root->eval();
}

int main() {

    std::ifstream inFile("./inputs/dec18.txt");
    std::string line;
    std::vector<std::string> exprs;

    while (std::getline(inFile, line)) {
        exprs.push_back(line);
    }

    
    int64_t sum = 0;
    for (auto const& expr : exprs) {
        sum += evaluate(expr);
    }
    printf("sum of expr results = %ld\n", sum);

    sum = 0;
    for (auto const& expr : exprs) {
        sum += evaluate(expr, true);
    }
    printf("sum of expr results = %ld\n", sum);
}