#ifdef __EMSCRIPTEN__

#include "cspsolver.h"
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(calcudoku_solver) {
    class_<CspSolver>("CspSolver")
        .constructor<int, int>()
        .constructor<int>()
        .constructor<>()
        .function("addVariable", &CspSolver::addVariable)
        .function("constraintFactory", &CspSolver::constraintFactory)
        .function("addVariableToConstraint", &CspSolver::addVariableToConstraint)
        .function("generateAlldiff", &CspSolver::generateAlldiff)
        .function("generateAlldiffBox", &CspSolver::generateAlldiffBox)
        .function("solve", &CspSolver::solve)
        .function("indexOfVariable", &CspSolver::indexOfVariable)
        //.function("getVariable", &CspSolver::getVariable)
        ;

/*
    value_object<Variable>("Variable")
        .field("name", &Variable::name)
        .field("value", &Variable::value)
        .field("values", &Variable::value)
        ;*/
}

#endif