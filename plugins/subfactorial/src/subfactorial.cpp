#include <cmath>

#include "calculator/plugin.hpp"

double subfact_func(const double* args, int argCount) {
    if (argCount != 1) return NAN;
    if (args[0] != static_cast<int>(args[0])) return NAN;

    const int n = static_cast<int>( args[0]);
    if (n < 0) return NAN;
    if (n == 0) return 1.0;
    if (n == 1) return 0.0;
    double a = 1.0, b = 0.0, res = 0.0;
    for (int i = 2; i <= n; ++i) {
        res = (i - 1) * (a + b);
        a = b;
        b = res;
    }
    return res;
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    OperationInfo infoSubfact = {};
    infoSubfact.canonicalName = "subfact";
    infoSubfact.function = subfact_func;
    infoSubfact.numArguments = 1;
    infoSubfact.type = OperationType::Prefix;
    infoSubfact.symbol = "!";
    infoSubfact.precedence = 6;
    infoSubfact.associativity = Associativity::Right;
    registrar->registerOperation(infoSubfact);
}