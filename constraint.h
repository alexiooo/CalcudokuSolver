#ifndef CALCUDOKUSOLVER_CONSTRAINT_H
#define CALCUDOKUSOLVER_CONSTRAINT_H

#include <vector>
#include <memory>
#include "variable.h"
using std::vector;
using std::shared_ptr;

class CspSolver;

class Constraint {
public:
    explicit Constraint(CspSolver & csp) : csp(csp) {}
    virtual ~Constraint() = default;
    virtual bool isSatisfied() = 0;
    virtual void addVariable(int v);

    vector<int> vars = vector<int>();
protected:
    CspSolver & csp;
};

class AlldiffConstraint : public Constraint {
public:
    explicit AlldiffConstraint(CspSolver & csp) : Constraint(csp) {}
    bool isSatisfied() override;
};

class PlusConstraint : public Constraint {
public:
    explicit PlusConstraint(CspSolver & csp, int target) : Constraint(csp), target(target) {}
    bool isSatisfied() override;
private:
    int target;
};

class MinusConstraint : public Constraint {
public:
    explicit MinusConstraint(CspSolver & csp, int target) : Constraint(csp), target(target) {}
    bool isSatisfied() override;
private:
    int target;
};

class MultiplyConstraint : public Constraint {
public:
    explicit MultiplyConstraint(CspSolver & csp, int target) : Constraint(csp), target(target) {}
    bool isSatisfied() override;
private:
    int target;
};

class DivideConstraint : public Constraint {
public:
    explicit DivideConstraint(CspSolver & csp, int target) : Constraint(csp), target(target) {}
    bool isSatisfied() override;
private:
    int target;
};

class ModuloConstraint : public Constraint {
public:
    explicit ModuloConstraint(CspSolver & csp, int target) : Constraint(csp), target(target) {}
    bool isSatisfied() override;
private:
    int target;
};




#endif //CALCUDOKUSOLVER_CONSTRAINT_H
