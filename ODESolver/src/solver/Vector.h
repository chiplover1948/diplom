#pragma once
#include "Matrix.h"
#include <iostream>
#include <cmath>

class Matrix;

class Vector
{
public:
	Vector(int n);
	Vector(int n, double val);
	Vector(const double *, int n);
	Vector(const Vector &);
	~Vector();

	double LInfinityNorm() const;

	double &operator[](int i) const;
	Vector operator*(double m) const;
	Vector operator/(double m) const;
	Vector operator+(const Vector &obj) const;
	Vector operator-(const Vector &obj) const;
	Vector &operator=(const Vector &obj);
	Matrix *operator&(const Vector &b);
	int Dimension() const { return n; }
	friend std::ostream& operator<<(std::ostream& os, const Vector& obj);

	static Vector *Lerp(double t, double t0, Vector &v0, double t1, Vector &v1);

	double *GetArray() const { return a; }
	void Print() {std::cout<<*this;}
	
	double &SetElement(int i, double val) {return a[i] = val;}
	double GetElement(int i) {return a[i];}
private:
	double *a;
	int n;
};

