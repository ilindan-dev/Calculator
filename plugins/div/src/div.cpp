#include "calculator/plugin.hpp"
#include <cmath>

namespace {
    double div_impl(const double* args, int argCount) {
        if (args[1] == 0.0) return NAN;
        return args[0] / args[1];
    }
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    registrar->registerOperation(OperationInfo{
        "div", 
        &div_impl, 
        2, 
        OperationType::Infix, 
        "/", 
        3, 
        Associativity::Left
    });
}