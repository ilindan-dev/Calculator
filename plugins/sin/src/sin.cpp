#include "calculator/plugin.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double sin_func(const double* args, int argCount) {
    if (argCount != 1) {
        return NAN;
    }

    const double degrees = args[0];
    const double radians = degrees * (M_PI / 180.0);
    return std::sin(radians);
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    OperationInfo info = {};
    info.canonicalName = "sin";
    info.function = sin_func;
    info.numArguments = 1;
    info.type = OperationType::Function;
    info.symbol = "sin";

    registrar->registerOperation(info);
}