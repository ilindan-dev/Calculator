#include "tokenizer.hpp"

#include <algorithm>
#include <cctype>
#include <string>

// Constructor
Tokenizer::Tokenizer() {
    // Add default symbols
    addSymbol("(");
    addSymbol(")");
    addSymbol(",");
}

// addSymbol adds a new symbol to the tokenizer's recognized symbols.
void Tokenizer::addSymbol(const std::string& symbol) {
    knownSymbols.push_back(symbol);
    std::ranges::sort(knownSymbols,
                      [](const std::string& a, const std::string& b) {
                          return a.length() > b.length();
                      });
}

// tokenize tokenizes the given expression into a vector of Tokens.
std::vector<Token> Tokenizer::tokenize(const std::string& expression) const {
    std::vector<Token> tokens;
    size_t pos = 0;

    while (pos < expression.length()) {
        char c = expression[pos];
        if (isspace(c)) {
            ++pos;
            continue;
        }

        if (std::isdigit(c) || (c == '.' && pos + 1 < expression.length() && std::isdigit(expression[pos + 1]))) {
            std::string num_str;
            bool has_decimal = (c == '.');
            num_str += c;
            pos++;

            while (pos < expression.length()) {
                if (std::isdigit(expression[pos])) {
                    num_str += expression[pos];
                } else if (expression[pos] == '.' && !has_decimal) {
                    num_str += expression[pos];
                    has_decimal = true;
                } else {
                    break;
                }
                pos++;
            }
            tokens.push_back({Token::Type::Number, num_str});
            continue;
        }

        bool symbolFound = false;
        for (const auto& symbol : knownSymbols) {
            if (expression.substr(pos, symbol.length()) == symbol) {
                if (symbol == "(") {
                    tokens.push_back({Token::Type::ParenthesisOpen, symbol});
                } else if (symbol == ")") {
                    tokens.push_back({Token::Type::ParenthesisClose, symbol});
                } else if (symbol == ",") {
                    tokens.push_back({Token::Type::Comma, symbol});
                } else {
                    tokens.push_back({Token::Type::Symbol, symbol});
                }
                pos += symbol.length();
                symbolFound = true;
                break;
            }
        }

        if (symbolFound) {
            continue;
        }

        tokens.push_back({Token::Type::Unknown, std::string(1, c)});
        pos++;
    }
    return tokens;
}
