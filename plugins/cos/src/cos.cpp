#include "calculator/plugin.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double cos_func(const double* args, int argCount) {
    if (argCount != 1) {
        return NAN;
    }

    const double degrees = args[0];
    const double radians = degrees * (M_PI / 180.0);
    return std::cos(radians);
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    OperationInfo info = {};
    info.canonicalName = "cos";
    info.function = cos_func;
    info.numArguments = 1;
    info.type = OperationType::Function;
    info.symbol = "cos";

    registrar->registerOperation(info);
}