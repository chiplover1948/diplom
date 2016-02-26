#pragma once
#include "Matrix.h"
#include "Helpers.h"
#include <cmath>
#include <algorithm>

class NordsieckState {
public:
	Matrix *zn;

	double tn, dt, Dq, delta;

	int qn;

	int qmax;

	int nsuccess;

	double rFactor;

	Vector *xn, *en;
	static const double epsilon;

	NordsieckState(): zn(NULL), xn(NULL), en(NULL) {}
	NordsieckState(Matrix m) {
		this->zn = new Matrix(m);
	}
	~NordsieckState();

	NordsieckState &operator=(const NordsieckState &);

	static Matrix *ZNew(const Matrix &arg);
	static Matrix *Rescale(const Matrix &arg, double r);
	static Matrix *Jacobian(RightSide *f, Vector &x, double t);
};
