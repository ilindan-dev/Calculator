#include "calculator/plugin.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double deg_func(const double* args, int argCount) {
    if (argCount != 1) {
        return NAN;
    }

    const double radians = args[0];
    return radians * (180.0 / M_PI);
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    OperationInfo info = {};
    info.canonicalName = "deg";
    info.function = deg_func;
    info.numArguments = 1;
    info.type = OperationType::Function;
    info.symbol = "deg";

    registrar->registerOperation(info);
}