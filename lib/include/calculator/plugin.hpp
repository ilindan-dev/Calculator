#ifndef CALCULATOR_PLUGIN_HPP
#define CALCULATOR_PLUGIN_HPP

#include "i_calculator_registrar.hpp"

// --------------- Windows DLL Export/Import ---------------
#if defined(_WIN32)
    #ifdef PLUGIN_EXPORTS
        // When compiling, we export the symbols
        #define PLUGIN_API __declspec(dllexport)
    #else
        // When using, we import characters
        #define PLUGIN_API __declspec(dllimport)
    #endif
#else
// ---------------------- Linux/macOS ----------------------
    // For Linux/macOS making characters visible by default
    #define PLUGIN_API __attribute__((visibility("default")))
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

// PluginFunction is type of C-function, forced to use it for reliability.
using PluginFunction = double (*)(const double* args, int argCount);

// OperationInfo struct contains information about a single operation provided by a plugin.
struct OperationInfo {
    // canonicalName is name of the operation, with whose will be worked RPN.
    const char* canonicalName{};

    // function if the function that implements the operation.
    PluginFunction function{};

    // numArguments is the number of arguments the operation takes.
    int numArguments{};

    // type is the type of the operation (Function, Infix, Prefix, Postfix).
    OperationType type{};

    // symbol is the symbol used to represent the operation in expressions.
    const char* symbol{};

    // precedence is the precedence level of the operation.
    int precedence = 0; // 0 for functions.

    // associativity is the associativity of the operation.
    Associativity associativity = Associativity::None; // None for functions.
};

// Entrance, that will be called by the calculator to register the plugin.
extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar);

#endif //CALCULATOR_PLUGIN_HPP