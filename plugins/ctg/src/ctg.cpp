#include "calculator/plugin.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double ctg_func(const double* args, int argCount) {
    if (argCount != 1) {
        return NAN;
    }

    const double degrees = args[0];
    const double radians = degrees * (M_PI / 180.0);

    const double sin_val = std::sin(radians);
    if ( std::abs(sin_val) < 1e-15) {
        return NAN;
    }

    return std::cos(radians)/sin_val;
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    OperationInfo info = {};
    info.canonicalName = "ctg";
    info.function = ctg_func;
    info.numArguments = 1;
    info.type = OperationType::Function;
    info.symbol = "ctg";

    registrar->registerOperation(info);
}