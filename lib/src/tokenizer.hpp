#ifndef CALCULATOR_TOKENIZER_HPP
#define CALCULATOR_TOKENIZER_HPP

#include <string>
#include <vector>

// Token struct represents a single token in the tokenized expression.
struct Token {
    // Type enum class represents the type of the token.
    enum class Type {
        Number, // Numeric literal (e.g., "3.14")
        Symbol, // Operator or function name (e.g., "+", "sin")
        ParenthesisOpen, // "("
        ParenthesisClose, // ")"
        Comma, // ","
        Unknown // Unrecognized character
    };

    // type is the type of the token.
    Type type;
    // value is the string value of the token.
    std::string value;
};

// Tokenizer class is responsible for lexical analysis of mathematical expressions.
// It breaks a string into a sequence of Tokens based on registered symbols.
class Tokenizer {
public:
    // Constructor: Initializes the tokenizer with default symbols (parentheses, comma).
    Tokenizer();

    // addSymbol registers a new symbol (operator or function name) to be recognized.
    // Symbols are sorted by length to ensure longer symbols are matched first (e.g., "sin" before "s").
    void addSymbol(const std::string& symbol);

    // tokenize parses the given expression string into a vector of Tokens.
    [[nodiscard]] std::vector<Token> tokenize(const std::string& expression) const;
private:
    // knownSymbols stores all registered symbols, sorted by length in descending order.
    std::vector<std::string> knownSymbols;
};


#endif //CALCULATOR_TOKENIZER_HPP