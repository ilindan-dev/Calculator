#include "calculator/plugin.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double rad_func(const double* args, int argCount) {
    if (argCount != 1) {
        return NAN;
    }

    const double degrees = args[0];
    return degrees * (M_PI / 180.0);
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    OperationInfo info = {};
    info.canonicalName = "rad";
    info.function = rad_func;
    info.numArguments = 1;
    info.type = OperationType::Function;
    info.symbol = "rad";

    registrar->registerOperation(info);
}