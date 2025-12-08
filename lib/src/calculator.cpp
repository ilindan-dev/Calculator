#include "calculator/calculator.hpp"
#include "calculator/plugin.hpp"
#include "tokenizer.hpp"
#include "dll_loader.hpp"

#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <cmath>
#include <stack>

namespace {
    // Helper function to check if a string is a valid number
    bool isNumber(const std::string& s) {
        if (s.empty()) return false;
        char* p;
        std::strtod(s.c_str(), &p);
        return *p == 0;
    }
}

// Impl is the implementation class for the Calculator for implementation hiding (Pimpl idiom).
class Calculator::Impl final : public ICalculatorRegistrar {
public:

    // functions is the map of registered functions, the key is CanonicalName.
    // <CanonicalName, Function>
    std::unordered_map<std::string, std::function<double(const std::vector<double>&)>> functions;

    // numArgs is the map of number of arguments for each operation, the key is CanonicalName.
    // <CanonicalName, ArgCount>
    std::unordered_map<std::string, int> numArgs;

    // precedences is the map of precedences for each operation, the key is CanonicalName.
    // <CanonicalName, Precedence>
    std::unordered_map<std::string, int> precedences;

    // associativities is the map of associativities for each operation, the key is CanonicalName.
    // <CanonicalName, Associativity>
    std::unordered_map<std::string, Associativity> associativities;

    // function_operations is the map of function operations, the key is Symbol, the value is CanonicalName.
    std::unordered_map<std::string, std::string> functionOperations;

    // infix_operations is the map of infix operations, the key is Symbol, the value is CanonicalName.
    std::unordered_map<std::string, std::string> infixOperations;

    // prefix_operations is the map of prefix operations, the key is Symbol, the value is CanonicalName.
    std::unordered_map<std::string, std::string> prefixOperations;

    // postfix_operations is the map of postfix operations, the key is Symbol, the value is CanonicalName.
    std::unordered_map<std::string, std::string> postfixOperations;

    // Dynamic Tokenizer
    Tokenizer tokenizer;

    // Store DllLoader objects to keep libraries loaded in memory
    std::vector<DllLoader> loadedPlugins;

    Impl() = default;
    ~Impl() override = default;

    // registerOperation registers a new operation from a plugin
    void registerOperation(const OperationInfo& info) override {
        const std::string canonicalName = info.canonicalName;
        const std::string symbol = info.symbol;

        if (functions.find(canonicalName) != functions.end()) {
            std::cerr << "Warning: Operation " << canonicalName << " registered. Skipping." << std::endl;
            return;
        }

        PluginFunction cFunc = info.function;

        const std::function<double(const std::vector<double>&)> cppWrapper =
            [cFunc, canonicalName](const std::vector<double>& args) {
                const double result = cFunc(args.data(), static_cast<int>(args.size()));
                // if (std::isnan(result)) {
                //     throw std::runtime_error("Error in operation: " + canonicalName);
                // }
                return result;
        };

        functions[canonicalName] = cppWrapper;
        numArgs[canonicalName] = info.numArguments;

        switch (info.type) {
            case OperationType::Function:
                functionOperations[symbol] = canonicalName;
                break;
            case OperationType::Infix:
                infixOperations[symbol] = canonicalName;
                precedences[canonicalName] = info.precedence;
                associativities[canonicalName] = info.associativity;
                break;
            case OperationType::Prefix:
                prefixOperations[symbol] = canonicalName;
                precedences[canonicalName] = info.precedence;
                associativities[canonicalName] = info.associativity;
                break;
            case OperationType::Postfix:
                postfixOperations[symbol] = canonicalName;
                precedences[canonicalName] = info.precedence;
                associativities[canonicalName] = info.associativity;
                break;
        }

        tokenizer.addSymbol(symbol);
    }

    // loadPlugin loads a single dynamic library using DllLoader
    void loadPlugin(const std::filesystem::path& pluginPath) {
        using RegisterPluginFunc = void(*)(ICalculatorRegistrar*);

        try {
            DllLoader loader(pluginPath.string());

            const auto registerPlugin =
                loader.getSymbol<RegisterPluginFunc>("registerPlugin");
            if (!registerPlugin) {
                throw std::runtime_error("Failed to find 'registerPlugin' symbol");
            }

            registerPlugin(this);

            loadedPlugins.push_back(std::move(loader));

        } catch (const std::exception& e) {
            std::cerr << "Error loading plugin " << pluginPath << ": " << e.what() << std::endl;
        }
    }
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------- Calculator -----------------------------------------------------

// Constructor.
Calculator::Calculator() : pImpl(std::make_unique<Impl>()) {}

// Destructor.
Calculator::~Calculator() = default;

// Move Constructor.
Calculator::Calculator(Calculator&& other) noexcept = default;

// Move Assignment.
Calculator& Calculator::operator=(Calculator&& other) noexcept = default;

// registerOperation registers a new operation in the calculator.
void Calculator::registerOperation(const OperationInfo& operation) {
    pImpl->registerOperation(operation);
}

// loadPlugins loads plugins from the specified directory.
void Calculator::loadPlugins(const std::string& pluginDir) const {
    if (!std::filesystem::exists(pluginDir) || !std::filesystem::is_directory(pluginDir)) {
        std::cout << "Failed to load plugins: " << pluginDir << std::endl;
        return;
    }
#ifndef PLUGIN_EXTENSION
#ifdef _WIN32
    const std::string extension = ".dll";
#else
    const std::string extension = ".so";
#endif
#else
    const std::string extension = PLUGIN_EXTENSION;
#endif

    for (const auto& entry : std::filesystem::recursive_directory_iterator(pluginDir)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            pImpl->loadPlugin(entry.path());
        }
    }
}

// evaluate evaluates the given mathematical expression and returns the result.
double Calculator::evaluate(const std::string& expression) const {
    const std::vector<Token> tokens = pImpl->tokenizer.tokenize(expression);

    std::vector<std::string> outputQueue;
    std::stack<std::string> operatorStack;
    bool wasLastTokenOperand = false;

    auto contains = [](const auto& map, const std::string& key) {
        return map.find(key) != map.end();
    };

    for (const auto& [type, value] : tokens) {
        switch (type) {
            case Token::Type::Number:
                outputQueue.push_back(value);
                wasLastTokenOperand = true;
                break;
            case Token::Type::Symbol: {
                std::string operationName;
                OperationType operationType = {};

                if (wasLastTokenOperand) {
                    if (contains(pImpl->infixOperations, value)) {
                        operationName = pImpl->infixOperations.at(value);
                        operationType = OperationType::Infix;
                    } else if (contains(pImpl->postfixOperations, value)) {
                        operationName = pImpl->postfixOperations.at(value);
                        operationType = OperationType::Postfix;
                    }
                } else {
                    if (contains(pImpl->prefixOperations, value)) {
                        operationName = pImpl->prefixOperations.at(value);
                        operationType = OperationType::Prefix;
                    } else if (contains(pImpl->functionOperations, value)) {
                        operationName = pImpl->functionOperations.at(value);
                        operationType = OperationType::Function;
                    }
                }

                if (operationName.empty()) {
                    throw std::runtime_error("Unknown symbol or context error: " + value);
                }

                if (operationType == OperationType::Function) {
                    operatorStack.push(operationName);
                } else {
                    const int precedence = pImpl->precedences.at(operationName);
                    const Associativity assoc = contains(pImpl->associativities, operationName) ?
                    pImpl->associativities.at(operationName) : Associativity::None;

                    while (!operatorStack.empty()) {
                        const std::string& topOperator = operatorStack.top();
                        if (!contains(pImpl->precedences, topOperator)) break;

                        if (const int topPrecedence = pImpl->precedences.at(topOperator);
                            (assoc == Associativity::Left && precedence <= topPrecedence) ||
                            (assoc == Associativity::Right && precedence < topPrecedence)) {
                            outputQueue.push_back(topOperator);
                            operatorStack.pop();
                            } else {
                                break;
                            }
                    }
                    operatorStack.push(operationName);
                }

                wasLastTokenOperand = (operationType == OperationType::Postfix);
                break;
            }
            case Token::Type::ParenthesisOpen:
                operatorStack.emplace("(");
                wasLastTokenOperand = false;
                break;
            case Token::Type::ParenthesisClose:
                while (!operatorStack.empty() && operatorStack.top() != "(") {
                    outputQueue.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                if (operatorStack.empty()) {
                    throw std::runtime_error("Mismatched parentheses (missing '(').");
                }
                operatorStack.pop();

                if (!operatorStack.empty()) {
                    if (const std::string& topOp = operatorStack.top(); contains(pImpl->functions, topOp) &&
                        !contains(pImpl->precedences,topOp)) {
                        outputQueue.push_back(topOp);
                        operatorStack.pop();
                    }
                }
                wasLastTokenOperand = true;
                break;
            case Token::Type::Comma:
                while (!operatorStack.empty() && operatorStack.top() != "(") {
                    outputQueue.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                if (operatorStack.empty()) {
                    throw std::runtime_error("Mismatched comma or parentheses.");
                }
                wasLastTokenOperand = false;
                break;
            case Token::Type::Unknown:
                throw std::runtime_error("Unknown symbol in expression: " + value);
        }
    }

    while (!operatorStack.empty()) {
        const std::string& topOperator = operatorStack.top();
        if (topOperator == "(") {
            throw std::runtime_error("Mismatched parentheses (missing ')').");
        }
        outputQueue.push_back(topOperator);
        operatorStack.pop();
    }

    std::stack<double> valueStack;

    for (const auto& tokenString : outputQueue) {
        if (isNumber(tokenString)) {
            valueStack.push(std::stod(tokenString));
        } else if (contains(pImpl->functions, tokenString)) {
            const std::string& operatorName = tokenString;
            const int argsCount = pImpl->numArgs.at(operatorName);
            if (valueStack.size() < static_cast<size_t>(argsCount)) {
                throw std::runtime_error("Insufficient values for operation: " + operatorName);
            }

            std::vector<double> args(argsCount);
            for (int i = 0; i < argsCount; ++i) {
                args[argsCount - 1 - i] = valueStack.top();
                valueStack.pop();
            }
            double result = pImpl->functions.at(operatorName)(args);
            valueStack.push(result);
        } else {
            throw std::runtime_error("Unknown operation in RPN: " + tokenString);
        }
    }

    if (valueStack.size() != 1) {
        throw std::runtime_error("Invalid expression: final value stack size is not 1.");
    }

    return valueStack.top();
}
