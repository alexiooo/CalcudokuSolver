#include <iostream>
#include <sstream>
#include "cspsolver.h"


using std::cout;
using std::cerr;
using std::endl;

void setValue( const CspSolver & csp,  Variable & var, int value ) {
    for (int v=0; v<csp.values; v++)
        var.valueSet[v] = v == value;
    var.value = value;
}

int main() {
    CspSolver csp(4);

    for (int r=0; r<4; r++) {
        for (int c=0; c<4; c++) {
            std::stringstream str;
            str << "r" << r << "c" << c;
            csp.addVariable(str.str());
        }
    }

    csp.generateAlldiff("r%ic%i");

    csp.getVariable("r0c1")->value = 1; // 1  :  r0c1
    csp.getVariable("r1c1")->value = 3; // 3  :  r1c1
    csp.getVariable("r1c2")->value = 2; // 2  :  r1c2
    csp.getVariable("r1c3")->value = 4; // 4  :  r1c3

    // 7+ :  r0c2,r0c3
    auto con = std::make_unique<PlusConstraint>(csp, 7);
    con->addVariable( csp.indexOfVariable("r0c2") );
    con->addVariable( csp.indexOfVariable("r0c3") );
    csp.constraints.emplace_back(std::move(con));

    // 3+ :  r0c0,r1c0
    con = std::make_unique<PlusConstraint>(csp, 3);
    con->addVariable( csp.indexOfVariable("r0c0") );
    con->addVariable( csp.indexOfVariable("r1c0") );
    csp.constraints.emplace_back(std::move(con));

    // 7+ :  r2c0,r2c1
    con = std::make_unique<PlusConstraint>(csp, 7);
    con->addVariable( csp.indexOfVariable("r2c0") );
    con->addVariable( csp.indexOfVariable("r2c1") );
    csp.constraints.emplace_back(std::move(con));

    // 3+ :  r2c2,r2c3
    con = std::make_unique<PlusConstraint>(csp, 3);
    con->addVariable( csp.indexOfVariable("r2c2") );
    con->addVariable( csp.indexOfVariable("r2c3") );
    csp.constraints.emplace_back(std::move(con));

    // 6+ :  r3c0,r3c1
    con = std::make_unique<PlusConstraint>(csp, 6);
    con->addVariable( csp.indexOfVariable("r3c0") );
    con->addVariable( csp.indexOfVariable("r3c1") );
    csp.constraints.emplace_back(std::move(con));

    // 4+ :  r3c2,r3c3
    con = std::make_unique<PlusConstraint>(csp, 4);
    con->addVariable( csp.indexOfVariable("r3c2") );
    con->addVariable( csp.indexOfVariable("r3c3") );
    csp.constraints.emplace_back(std::move(con));

    bool res = csp.solve();
    cout << endl
         << "Solved: " << ( res ? "Yay!" : "Nope" ) << endl;
    csp.print();

    return 0;
}