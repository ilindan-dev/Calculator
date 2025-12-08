#include "calculator/plugin.hpp"
#include <cmath>

double ln_func(const double* args, int argCount) {
    if (argCount != 1) {
        return NAN;
    }

    if (args[0] <= 0) return NAN;
    return std::log(args[0]);
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    OperationInfo info = {};
    info.canonicalName = "ln";
    info.function = ln_func;
    info.numArguments = 1;
    info.type = OperationType::Function;
    info.symbol = "ln";

    registrar->registerOperation(info);
}