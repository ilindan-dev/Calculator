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
    std::sort(knownSymbols.begin(),
        knownSymbols.end(),
        [](const std::string& a, const std::string& b) {
            return a.length() > b.length();
    });
}

// tokenize tokenizes the given expression into a vector of Tokens.
std::vector<Token> Tokenizer::tokenize(const std::string& expression) const {
    std::vector<Token> tokens;
    size_t pos = 0;

    while (pos < expression.length()) {
        const char currentChar = expression[pos];

        if (std::isspace(static_cast<unsigned char>(currentChar))) {
            ++pos;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(currentChar)) ||
            (currentChar == '.' && pos + 1 < expression.length() &&
                std::isdigit(static_cast<unsigned char>(expression[pos + 1])))) {
            std::string numberString;
            bool hasDecimal = (currentChar == '.');
            numberString += currentChar;
            pos++;

            while (pos < expression.length()) {
                if (const char nextChar = expression[pos]; std::isdigit(static_cast<unsigned char>(nextChar))) {
                    numberString += nextChar;
                } else if (nextChar == '.' && !hasDecimal) {
                    numberString += nextChar;
                    hasDecimal = true;
                } else {
                    break;
                }
                pos++;
            }
            tokens.push_back({Token::Type::Number, numberString});
            continue;
        }

        bool symbolFound = false;
        for (const auto& symbol : knownSymbols) {
            if (expression.compare(pos, symbol.length(), symbol) == 0) {
                auto type = Token::Type::Symbol;
                if (symbol.length() == 1) {
                    switch (symbol[0]) {
                        case '(': type = Token::Type::ParenthesisOpen; break;
                        case ')': type = Token::Type::ParenthesisClose; break;
                        case ',': type = Token::Type::Comma; break;
                        default:  type = Token::Type::Symbol; break;
                    }
                }

                tokens.push_back({type, symbol});
                pos += symbol.length();
                symbolFound = true;
                break;
            }
        }

        if (symbolFound) {
            continue;
        }

        tokens.push_back({Token::Type::Unknown, std::string(1, currentChar)});
        pos++;
    }
    return tokens;
}
