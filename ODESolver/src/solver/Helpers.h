#pragma once
#include "Vector.h"
#include <float.h>

#define DELETE(arg) { if (arg) delete arg; arg = NULL; }

class Vector;
typedef void RightSide(double, double *, double *);

struct Options {
    double InitialStep;
    double AbsoluteTolerance;
    double RelativeTolerance;
    double OutputStep;
    double MaxStep, MinStep;
    double MaxScale, MinScale;
    Matrix *Jacobian;
    int NumberOfIterations;

    Options() {
            InitialStep = 0.0;
            AbsoluteTolerance = 1e-10;
            RelativeTolerance = 1e-10;
            MaxStep = 1.0;
            MinStep = 0.0;
            MaxScale = 1.1;
            MinScale = 0.9;
            OutputStep = 0.0;
            NumberOfIterations = 5;
            Jacobian = NULL;
    }
};

class SolPoint {
private:
    Vector solve;
    double time;    
public:
    Vector GetSolve() {return solve;}
    double GetTime() {return time;}
    SolPoint(double t, Vector s): solve(s), time(t) {}
    friend std::ostream& operator<<(std::ostream& os, SolPoint& obj);
    void Print() {std::cout << *this;}
};
