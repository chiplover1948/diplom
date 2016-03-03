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
    SolPoint Solve();
    int fails;
private:
    RightSide *rightSide;
    Options opts;
    NordsieckState currstate;
    int n;
    
    Vector f(double, Vector &);
    void Corrector(bool &);
    void Predictor();
    static double Factorial(int n);
    static double ToleranceNorm(const Vector &, double, double, const Vector &);

    static const double l[6][7];

    static const double b[];
};




