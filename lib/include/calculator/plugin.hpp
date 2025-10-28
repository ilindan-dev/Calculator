#ifndef CALCULATOR_PLUGIN_HPP
#define CALCULATOR_PLUGIN_HPP

#include "i_calculator_registrar.hpp"
#include <string>
#include <vector>
#include <functional>

#if defined(_WIN32)
    #ifdef PLUGIN_EXPORTS
        #define PLUGIN_API __declspec(dllexport)
    #else
        #define PLUGIN_API __declspec(dllimport)
    #endif
#else
    #define PLUGIN_API
#endif

// OperationType is enum class of the type an operation.
enum class OperationType {
    Function,
    Infix,
    Prefix,
    Postfix
};

// Associativity is enum class of the associativity of an operator.
enum class Associativity {
    Left,
    Right,
    None
};

// OperationInfo struct contains information about a single operation provided by a plugin.
struct OperationInfo {
    // canonicalName is name of the operation, with whose will be worked RPN.
    std::string canonicalName;

    // function if the function that implements the operation.
    std::function<double(const std::vector<double>&)> function;

    // numArguments is the number of arguments the operation takes.
    int numArguments;

    // type is the type of the operation (Function, Infix, Prefix, Postfix).
    OperationType type;

    // symbol is the symbol used to represent the operation in expressions.
    std::string symbol;

    // precedence is the precedence level of the operation.
    int precedence = 0; // 0 for functions.

    // associativity is the associativity of the operation.
    Associativity associativity = Associativity::None; // None for functions.
};

// Entrance, that will be called by the calculator to register the plugin.
extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar);

#endif //CALCULATOR_PLUGIN_HPP