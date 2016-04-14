#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include <float.h>
#include "Matrix.h"
#include "Vector.h"
#include "Nordsieck.h"
#include "Helpers.h"


class Gear
{
public:
    Gear(double, Vector&, RightSide *, Options &);
    ~Gear() {
        if (opts.OutputStep > 0) {
            delete xout;
            delete resX;
            delete last;
            delete next;
        }
    }
    SolPoint Solve();
private:
    RightSide *rightSide;
    Options opts;
    NordsieckState currstate;
    int n;
    Vector *xout;
    Vector *resX;
    double resT, tout;
    SolPoint *last, *next;
    
    Vector f(double, Vector &);
    void Corrector(bool &);
    void Predictor();
    SolPoint SolveNext();
    static double Factorial(int n);
    static double ToleranceNorm(const Vector &, double, double, const Vector &);

    static const double l[6][7];

    static const double b[];
};




