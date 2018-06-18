#ifndef CALCUDOKUSOLVER_CSPSOLVER_H
#define CALCUDOKUSOLVER_CSPSOLVER_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "variable.h"
#include "constraint.h"

using std::string;
using std::vector;
using std::unique_ptr;
using VariableVector=std::vector<std::shared_ptr<Variable> >;

class CspSolver {
public:
    explicit CspSolver(int values = MAX_VAL, int valueOffset = 1)
            : values(values), valueOffset(valueOffset) {}

    // Getters
    int indexOfVariable(const string & name);
    shared_ptr<Variable>& getVariable(const string & name);
    shared_ptr<Variable>& getVariable(int index);

    // Building
    long addVariable(string name);
    long constraintFactory(string spec);
    void addVariableToConstraint(int constraint, int var);
    void generateAlldiff(const string & format);
    void generateAlldiffBox(const string & format);

    // Solving
    bool solve();

    void print();
    const VariableVector& getVars() { return vars; }
    const vector< unique_ptr<Constraint> >& getConstraints() {
        return constraints;
    }

//private:
    const int values;
    const int valueOffset;
    VariableVector vars;
    vector< unique_ptr<Constraint> > constraints;

    // Solving
    void nodeConsistency();     // Removes all unary constraints;
    void arcConsistency();
    bool isAdmissable();
    bool backtrack(int offset);
};


#endif //CALCUDOKUSOLVER_CSPSOLVER_H
