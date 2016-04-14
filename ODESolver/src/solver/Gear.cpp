#include "Gear.h"
#include <iostream>

using namespace std;

Gear::Gear(double t0, Vector &x0, RightSide *rightSide, Options &opts)
{
    this->rightSide = rightSide;
    this->opts = opts;
    this->n = x0.Dimension();
    
    double dt;
    //Vector x = Vector(x0);
    if (opts.OutputStep > 0.0) {
        tout = t0 + opts.OutputStep;
        xout = new Vector(x0);
        resX = new Vector(x0);
        last = new SolPoint(t0, x0);
        next = new SolPoint(t0, x0);
    }
    
    Vector dx = f(t0, x0);
    if (opts.InitialStep != 0)
    {
        dt = opts.InitialStep;
    }
    
    else
    {
        double tol = opts.RelativeTolerance;
        Vector ewt = Vector(n);
        Vector ywt = Vector(n);
        double sum = 0.0;
        for (int i = 0; i < n; i++)
        {
                ewt[i] = opts.RelativeTolerance * abs(x0[i]) + opts.AbsoluteTolerance;
                ywt[i] = ewt[i] / tol;
                sum = sum + dx[i] * dx[i] / (ywt[i] * ywt[i]);
        }

        dt = sqrt(tol / (1.0 / (ywt[0] * ywt[0]) + sum / n));
    }

    dt = min(dt, opts.MaxStep);

    int qmax = 5;
    int qcurr = 2;

    //Compute Nordstieck's history matrix at t=t0;
    Matrix *zn = new Matrix(n, qmax + 1);
    for (int i = 0; i < n; i++)
    {
        zn->ElementAt(i, 0) = x0[i];
        zn->ElementAt(i, 1) = dt * dx[i];
        for (int j = qcurr; j < qmax + 1; j++)
        {
            zn->ElementAt(i, j) = 0.0;
        }
    }

    currstate.delta = 0.0;
    currstate.Dq = 0.0;
    currstate.dt = dt;
    currstate.tn = t0;
    currstate.qn = qcurr;
    currstate.qmax = qmax;
    currstate.nsuccess = 0;
    currstate.zn = zn;
    currstate.rFactor = 1.0;
    currstate.en = new Vector(n);
}

SolPoint Gear::Solve() {
    if (opts.OutputStep > 0.0) {
        while (tout >= next->GetTime()) {
            *last = *next;
            *next = SolveNext();
        }
        
        double toutLerp = tout;
        tout += opts.OutputStep;
        Vector lastSolve = last->GetSolve();
        Vector nextSolve = next->GetSolve();
        return SolPoint(tout, Vector::Lerp(toutLerp, last->GetTime(), lastSolve, next->GetTime(), nextSolve));
    } else
        return SolveNext();
}

SolPoint Gear::SolveNext()
{       
    bool isIterationFailed = false;

    // Predictor step
    Predictor();
    
    // Corrector step    
    do {
        Corrector(isIterationFailed);
        if (isIterationFailed) // If iterations are not finished - bad convergence
        {
            currstate.nsuccess = 0;
            currstate.dt = currstate.dt / 2.0;
            if (currstate.dt < currstate.epsilon || currstate.dt < opts.MinStep) 
                throw "Cannot generate numerical solution";
            Matrix *znew = NordsieckState::Rescale(*currstate.zn, 0.5);
            DELETE(currstate.zn)
            currstate.zn = znew;
        }
        else // Iterations finished
        {
            *resX = *currstate.xn;
            resT = currstate.tn;
            
            double r = min(1.1, max(0.2, currstate.rFactor));
            currstate.tn = currstate.tn + currstate.dt;

            if (opts.MaxStep < DBL_MAX)
            {
                    r = min(r, opts.MaxStep / currstate.dt);
            }

            if (opts.MinStep > 0)
            {
                    r = max(r, opts.MinStep / currstate.dt);
            }

            r = min(r, opts.MaxScale);
            r = max(r, opts.MinScale);

            currstate.dt = currstate.dt * r;

            Matrix *tmp = currstate.zn;
            currstate.zn = NordsieckState::Rescale(*currstate.zn, r);
            DELETE(tmp);
        }
    } while (isIterationFailed);
    
    return SolPoint(resT, *resX);
}

void Gear::Predictor() 
{
    Matrix *z0 = NordsieckState::ZNew(*currstate.zn);
    DELETE(currstate.zn)
    DELETE(currstate.xn)
    currstate.zn = z0;
    currstate.xn = new Vector(currstate.zn->GetColumn(0));
    
}

void Gear::Corrector(bool &flag)
{
    Vector ecurr = Vector(n);
    Vector xcurr = currstate.zn->GetColumn(0);
    Vector x0 = currstate.zn->GetColumn(0);
    Matrix zcurr = *currstate.zn;
    Matrix z0 = *currstate.zn;
    int qcurr = currstate.qn;
    int qmax = currstate.qmax;
    double dt = currstate.dt;
    double t = currstate.tn;

    //Tolerance computation factors 
    double Cq = pow(qcurr + 1, -1.0);
    double tau = 1.0 / (Cq * Factorial(qcurr) * l[qcurr - 1][qcurr]);

    int count = 0;

    double Dq = 0.0, DqUp = 0.0, DqDown = 0.0;
    double delta = 0.0;

    //Scaling factors for the step size changing
    //with new method order q' = q, q + 1, q - 1, respectively
    double rSame, rUp, rDown;

    Vector xprev = Vector(n);
    Vector gm = Vector(n);
    Vector deltaE = Vector(n);
    Matrix *M;

    Matrix *J = opts.Jacobian == NULL ? NordsieckState::Jacobian(rightSide, xcurr, t + dt) : opts.Jacobian;
    Matrix P = Matrix::Identity(n, n) - *J * dt * b[qcurr - 1];

    do
    {
        xprev = xcurr;
        gm = f(t + dt, xcurr) * dt  - z0.GetColumn(1) - ecurr;
        ecurr = ecurr + P.SolveGE(gm);
        xcurr = x0 + ecurr * b[qcurr - 1];

        //Row dimension is smaller than zcurr has
        M = ecurr & Vector(l[qcurr - 1], qcurr + 1);
        //So, "expand" the matrix
        Matrix MBig = Matrix::Identity(zcurr.RowDimension(), zcurr.ColumnDimension());
        for (int i = 0; i < zcurr.RowDimension(); i++)
        {
            for (int j = 0; j < zcurr.ColumnDimension(); j++)
            {
                    MBig.ElementAt(i, j) = i < M->RowDimension() && j < M->ColumnDimension() ? M->ElementAt(i, j) : 0.0;
            }
        }
        DELETE(M)
        zcurr = Matrix(z0 + MBig);
        Dq = ToleranceNorm(ecurr, opts.RelativeTolerance, opts.AbsoluteTolerance, xprev);
        deltaE = ecurr - *currstate.en;
        deltaE = deltaE * (1.0 / (qcurr + 2) * l[qcurr - 1][qcurr - 1]);
        DqUp = ToleranceNorm(deltaE, opts.RelativeTolerance, opts.AbsoluteTolerance, xcurr);
        DqDown = ToleranceNorm(zcurr.GetColumn(qcurr - 1), opts.RelativeTolerance, opts.AbsoluteTolerance, xcurr);
        delta = Dq / (tau / (2 * (qcurr + 2)));
        count++;
    } while (delta > 1.0 && count < opts.NumberOfIterations);

    DELETE(J)
    //======================================

    int nsuccess = count < opts.NumberOfIterations ? currstate.nsuccess + 1 : 0;

    if (count < opts.NumberOfIterations)
    {
            flag = false;
            DELETE(currstate.zn)
            DELETE(currstate.xn)
            DELETE(currstate.en)
            currstate.zn = new Matrix(zcurr);
            currstate.xn = new Vector(zcurr.GetColumn(0));
            currstate.en = new Vector(ecurr);
    }
    else
    {
            flag = true;
    }

    //Compute step size scaling factors
    rUp = 0.0;

    if (currstate.qn < currstate.qmax)
    {
            rUp = rUp = 1.0 / 1.4 / (pow(DqUp, 1.0 / (qcurr + 2)) + 1e-6);
    }

    rSame = 1.0 / 1.2 / (pow(Dq, 1.0 / (qcurr + 1)) + 1e-6);

    rDown = 0.0;

    if (currstate.qn > 1)
    {
            rDown = 1.0 / 1.3 / (pow(DqDown, 1.0 / (qcurr)) + 1e-6);
    }

    //======================================
    currstate.nsuccess = nsuccess >= currstate.qn ? 0 : nsuccess;
    //Step size scale operations

    if (rSame >= rUp)
    {
        if (rSame <= rDown && nsuccess >= currstate.qn && currstate.qn > 1)
        {
            currstate.qn = currstate.qn - 1;
            currstate.Dq = DqDown;

            for (int i = 0; i < n; i++)
            {
                    for (int j = currstate.qn + 1; j < qmax + 1; j++)
                    {
                            currstate.zn->ElementAt(i, j) = 0.0;
                    }
            }
            nsuccess = 0;
            currstate.rFactor = rDown;
        }
        else
        {
            currstate.qn = currstate.qn;
            currstate.Dq = Dq;
            currstate.rFactor = rSame;
        }

    }
    else
    {
        if (rUp >= rDown)
        {
            if (rUp >= rSame && nsuccess >= currstate.qn && currstate.qn < currstate.qmax)
            {
                currstate.qn = currstate.qn + 1;
                currstate.Dq = DqUp;
                currstate.rFactor = rUp;
                nsuccess = 0;
            }
            else
            {
                currstate.rFactor = rSame;
            }
        }
        else
        {
            if (nsuccess >= currstate.qn && currstate.qn > 1)
            {
                currstate.qn = currstate.qn - 1;
                currstate.Dq = DqDown;

                for (int i = 0; i < n; i++)
                {
                        for (int j = currstate.qn + 1; j < qmax + 1; j++)
                        {
                                currstate.zn->ElementAt(i, j) = 0.0;
                        }
                }
                nsuccess = 0;
                currstate.rFactor = rDown;

            }
            else
            {                
                currstate.rFactor = rSame;
            }
        }
    }

    currstate.qmax = qmax;
    currstate.dt = dt;
    currstate.tn = t;
}

double Gear::Factorial(int arg)
{
    if (arg < 0) return -1;
    if (arg == 0) return 1;
    return arg * Factorial(arg - 1);
}

double Gear::ToleranceNorm(const Vector &v, double RTol, double ATol, const Vector &a)
{
	return v.LInfinityNorm() / (ATol + RTol * a.LInfinityNorm());
}

const double Gear::l[6][7] = {{1.0,1.0,0.0,0.0,0.0,0.0,0.0},
                             {2.0/3.0,1.0,1.0/3.0,0.0,0.0,0.0,0.0},
                             {6.0/11.0,1.0,6.0/11.0,1.0/11.0,0.0,0.0,0.0},
                             {24.0/50.0,1.0,35.0/50.0,10.0/50.0,1.0/50.0,0.0,0.0},
                             {120.0/274.0,1.0,225.0/274.0,85.0/274.0,15.0/274.0,1.0/274.0,0.0},
                             {720.0/1764.0,1.0,1624.0/1764.0,735.0/1764.0,175.0/1764.0,21.0/1764.0,1.0/1764.0}};

const double Gear::b[] = { 1.0, 2.0 / 3.0, 6.0 / 11.0, 24.0 / 50.0, 120.0 / 274.0, 720.0 / 1764.0 };


Vector Gear::f(double t, Vector &x) 
{
	double *res = new double[x.Dimension()];
	rightSide(t, x.GetArray(), res);
	Vector resV = Vector(res, x.Dimension());
	delete[] res;
    return resV;
}

ostream& operator<<(ostream& os, SolPoint& obj) 
{
    os << "x[" << obj.time << "]=" << obj.solve << endl;
    return os;
}
