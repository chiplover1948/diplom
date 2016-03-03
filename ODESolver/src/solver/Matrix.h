#pragma once
#include "Vector.h"
#include <iostream>
#include <algorithm>

class Vector;

class Matrix {
private:
	double **a;
	int m, n;
	
	static double *Solve(double *a[], Vector b);
public:
	int RowDimension() const { return m; }
	int ColumnDimension() const { return n; }
	double &ElementAt(int m, int n) const;

	static Matrix Identity(int m, int n);
	Matrix(int m, int n);
	Matrix(int m, int n, double val);
	Matrix(const Matrix &obj);
	~Matrix();

	Matrix operator + (const Matrix &obj) const;
	Matrix operator - (const Matrix &obj) const;
	Matrix operator * (const Matrix &obj) const;
	Matrix operator * (double val) const;
	Matrix &operator = (const Matrix &obj);

	Vector GetColumn(int n);
	Vector SolveGE(const Vector&);

	friend std::ostream& operator<<(std::ostream& os, const Matrix& obj);
};

