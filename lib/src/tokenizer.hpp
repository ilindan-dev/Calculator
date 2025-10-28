#ifndef CALCULATOR_TOKENIZER_HPP
#define CALCULATOR_TOKENIZER_HPP

#include <string>
#include <vector>

// Token struct represents a single token in the tokenized expression.
struct Token {
    // Type enum class represents the type of the token.
    enum class Type {
        Number,
        Symbol,
        ParenthesisOpen,
        ParenthesisClose,
        Comma,
        Unknown
    };

    // type is the type of the token.
    Type type;
    // value is the string value of the token.
    std::string value;
};

class Tokenizer {
public:
    // Constructor
    Tokenizer();

    // addSymbol adds a new symbol to the tokenizer's recognized symbols.
    void addSymbol(const std::string& symbol);

    // tokenize tokenizes the given expression into a vector of Tokens.
    [[nodiscard]] std::vector<Token> tokenize(const std::string& expression) const;
private:
    // knownSymbols is the list of known symbols for tokenization.
    std::vector<std::string> knownSymbols;
};


#endif //CALCULATOR_TOKENIZER_HPP