#pragma once
#include "Vector.h"
#include <float.h>

#define DELETE(arg) { if (arg) delete arg; arg = NULL; }

class Vector;
typedef void RightSide(double, double *, double *);

class Options {
public:
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
            AbsoluteTolerance = 1e-8;
            RelativeTolerance = 1e-6;
            MaxStep = DBL_MAX;
            MinStep = 0.0;
            MaxScale = 1.1;
            MinScale = 0.9;
            OutputStep = 0.0;
            NumberOfIterations = 5;
            Jacobian = NULL;
    }
    double getInitialStep() const {return InitialStep;}
    void setInitialStep(double val) {InitialStep = val;}
    double getOutputStep() const {return OutputStep;}
    void setOutputStep(double val) {OutputStep = val;}
    double getAbsoluteTolerance() const {return AbsoluteTolerance;}
    void setAbsoluteTolerance(double val) {AbsoluteTolerance = val;}
    double getRelativeTolerance() const {return RelativeTolerance;}
    void setRelativeTolerance(double val) {RelativeTolerance = val;}
    
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
