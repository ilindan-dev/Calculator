#include <cmath>
#include "calculator/plugin.hpp"

double fact_func(const double* args, int argCount) {
    if (argCount != 1) return NAN;
    if (args[0] != static_cast<int>(args[0])) return NAN;

    const int n = static_cast<int>(args[0]);
    if (n < 0) return NAN;
    if (n == 0) return 1.0;
    double res = 1.0;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    OperationInfo infoFact = {};
    infoFact.canonicalName = "fact";
    infoFact.function = fact_func;
    infoFact.numArguments = 1;
    infoFact.type = OperationType::Postfix;
    infoFact.symbol = "!";
    infoFact.precedence = 5;
    infoFact.associativity = Associativity::Left;
    registrar->registerOperation(infoFact);
}