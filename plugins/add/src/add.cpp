#include "calculator/plugin.hpp"

namespace {
    double add_impl(const double* args, int argCount) {
        return args[0] + args[1];
    }
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    registrar->registerOperation(OperationInfo{
        "add",
        &add_impl,
        2,
        OperationType::Infix,
        "+",
        2,
        Associativity::Left
    });
}