#include "calculator/calculator.hpp"
#include "calculator/plugin.hpp"

#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <cmath>
#include <stack>

#include "tokenizer.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

// isNumber checks if the given string is a valid number.
static bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    char* p;
    std::strtod(s.c_str(), &p);
    return *p == 0;
}

// -- Internal function for basic operations ---
// Attention! Functions must comply with the C-API, so they cannot throw exceptions.

static double internalAdd(const double* args, int argCount) {
    return args[0] + args[1];
}
static double internalSub(const double* args, int argCount) {
    return args[0] - args[1];
}
static double internalMul(const double* args, int argCount) {
    return args[0] * args[1];
}
static double internalDiv(const double* args, int argCount) {
    if (args[1] == 0) return NAN;
    return args[0] / args[1];
}
static double internalPow(const double* args, int argCount) {
    return std::pow(args[0], args[1]);
}
static double internalNegate(const double* args, int argCount) {
    return -args[0];
}

// Impl is the implementation class for the Calculator for implementation hiding (Pimpl idiom).
class Calculator::Impl final : public ICalculatorRegistrar {
public:

    // functions is the map of registered functions, the key is CanonicalName.
    // <CanonicalName, Function>
    std::map<std::string, std::function<double(const std::vector<double>&)>> functions;

    // numArgs is the map of number of arguments for each operation, the key is CanonicalName.
    // <CanonicalName, ArgCount>
    std::map<std::string, int> numArgs;

    // precedences is the map of precedences for each operation, the key is CanonicalName.
    // <CanonicalName, Precedence>
    std::map<std::string, int> precedences;

    // associativities is the map of associativities for each operation, the key is CanonicalName.
    // <CanonicalName, Associativity>
    std::map<std::string, Associativity> associativities;

    // --- Search map for parser ---

    // function_operations is the map of function operations, the key is Symbol, the value is CanonicalName.
    std::map<std::string, std::string> functionOperations;

    // infix_operations is the map of infix operations, the key is Symbol, the value is CanonicalName.
    std::map<std::string, std::string> infixOperations;

    // prefix_operations is the map of prefix operations, the key is Symbol, the value is CanonicalName.
    std::map<std::string, std::string> prefixOperations;

    // postfix_operations is the map of postfix operations, the key is Symbol, the value is CanonicalName.
    std::map<std::string, std::string> postfixOperations;

    // --- Dynamic Tokenizer ---
    Tokenizer tokenizer;

    // --- Plugin handles ---
#ifdef _WIN32
    std::vector<HMODULE> pluginHandles;
#else
    std::vector<void*> pluginHandles;
#endif

    // Constructor
    Impl() {
        registerOperation({
            "add", internalAdd, 2,
            OperationType::Infix, "+", 2, Associativity::Left
        });
        registerOperation({
            "sub", internalSub, 2,
            OperationType::Infix, "-", 2, Associativity::Left
        });
        registerOperation({
            "mul", internalMul, 2,
            OperationType::Infix, "*", 3, Associativity::Left
        });
        registerOperation({
            "div", internalDiv, 2,
            OperationType::Infix, "/", 3, Associativity::Left
        });
        registerOperation({
            "pow", internalPow, 2,
            OperationType::Infix, "^", 4, Associativity::Right
        });
        registerOperation({
            "negate", internalNegate, 1,
            OperationType::Prefix, "-", 5, Associativity::Right
        });
    }

    // Destructor
    ~Impl() override {
        for (auto handle : pluginHandles) {
#ifdef _WIN32
            FreeLibrary(handle);
#else
            dlclose(handle);
#endif
        }
    }

    // registerOperation registers a new operation in the calculator.
    void registerOperation(const OperationInfo& info) override {
        const std::string canonicalName = info.canonicalName;
        const std::string symbol = info.symbol;

        if (functions.contains(canonicalName)) {
            std::cerr << "Warning: Operation " << canonicalName << " registered. Skipping." << std::endl;
            return;
        }


        PluginFunction cFunc = info.function;
        // Wrapper, whose handle NAN and safely turns them into C++ exceptions.
        const std::function<double(const std::vector<double>&)> cpp_wrapper =
            [cFunc, canonicalName](const std::vector<double>& args) {
                const double result = cFunc(args.data(), args.size());
                if (std::isnan(result)) {
                    throw std::runtime_error("Error in operation: " + canonicalName);
                }
                return result;
        };
        functions[canonicalName] = cpp_wrapper;
        numArgs[canonicalName] = info.numArguments;


        switch (info.type) {
            case OperationType::Function:
                functionOperations[symbol] = canonicalName;
                tokenizer.addSymbol(symbol);
                break;
            case OperationType::Infix:
                infixOperations[symbol] = canonicalName;
                precedences[canonicalName] = info.precedence;
                associativities[canonicalName] = info.associativity;
                tokenizer.addSymbol(symbol);
                break;
            case OperationType::Prefix:
                prefixOperations[symbol] = canonicalName;
                precedences[canonicalName] = info.precedence;
                associativities[canonicalName] = info.associativity;
                tokenizer.addSymbol(symbol);
                break;
            case OperationType::Postfix:
                postfixOperations[symbol] = canonicalName;
                precedences[canonicalName] = info.precedence;
                associativities[canonicalName] = info.associativity;
                tokenizer.addSymbol(symbol);
                break;
        }
    }

    // loadPlugin loads a single plugin from the specified path.
    void loadPlugin(const std::filesystem::path& pluginPath) {
        using RegisterPluginFunc = void(*)(ICalculatorRegistrar*);

        try {
#ifdef _WIN32
            HMODULE handle = LoadLibraryA(pluginPath.string().c_str());
            if (!handle) {
                throw std::runtime_error("Failed to load plugin: " + pluginPath.string());
            }
            pluginHandles.push_back(handle);

            auto registerPlugin = reinterpret_cast<RegisterPluginFunc>(GetProcAddress(handle, "registerPlugin"));

#else
            void* handle = dlopen(pluginPath.string().c_str(), RTLD_NOW);
            if (!handle) {
                throw std::runtime_error("Failed to load plugin: " + pluginPath.string() + " Error: " + dlerror());
            }
            pluginHandles.push_back(handle);
            const auto registerPlugin = reinterpret_cast<RegisterPluginFunc>(dlsym(handle, "registerPlugin"));
#endif
            if (!registerPlugin) {
                throw std::runtime_error("Failed to find registerPlugin function in: " + pluginPath.string());
            }
            registerPlugin(this);

            std::cout << "Registered plugin: " << pluginPath.filename().string() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to load plugin: " << pluginPath.string() << ": " << e.what() << std::endl;
        }
    }
};

// --- Calculator realize ---

// Constructor
Calculator::Calculator() : pImpl(std::make_unique<Impl>()) {}

// Destructor
Calculator::~Calculator() = default;

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
    const std::string extension = PLUGIN_EXTENSION;
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
                    if (pImpl->infixOperations.contains(value)) {
                        operationName = pImpl->infixOperations.at(value);
                        operationType = OperationType::Infix;
                    } else if (pImpl->postfixOperations.contains(value)) {
                        operationName = pImpl->postfixOperations.at(value);
                        operationType = OperationType::Postfix;
                    }
                } else {
                    if (pImpl->prefixOperations.contains(value)) {
                        operationName = pImpl->prefixOperations.at(value);
                        operationType = OperationType::Prefix;
                    } else if (pImpl->functionOperations.contains(value)) {
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
                    const Associativity assoc = pImpl->associativities.contains(operationName) ?
                    pImpl->associativities.at(operationName) : Associativity::None;

                    while (!operatorStack.empty()) {
                        const std::string& topOperator = operatorStack.top();
                        if (!pImpl->precedences.contains(topOperator)) break;

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
                    if (const std::string& topOp = operatorStack.top(); pImpl->functions.contains(topOp) &&
                        !pImpl->precedences.contains(topOp)) {
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
        } else if (pImpl->functions.contains(tokenString)) {
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
