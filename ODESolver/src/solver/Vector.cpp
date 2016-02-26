#include "Vector.h"

using namespace std;

Vector::Vector(int n) {
	this->n = n;
	this->a = new double[n];
	for (int i = 0; i < n; i++)
	{
		a[i] = 0.0;
	}
}

Vector::Vector(int n, double val) {
	this->n = n;
	this->a = new double[n];
	for (int i = 0; i < n; i++)
	{
		this->a[i] = val;
	}
}

Vector::Vector(const double *arr, int n)
{
	this->n = n;
	this->a = new double[n];
	for (int i = 0; i < n; i++)
	{
		this->a[i] = arr[i];
	}
}

Vector::Vector(const Vector &obj)
{
	this->n = obj.n;
	this->a = new double[n];
	for (int i = 0; i < n; i++)
	{
		a[i] = obj[i];
	}
}

Vector::~Vector()
{
	delete[] this->a;
}

double Vector::LInfinityNorm() const
{
	double max = 0;

	for (int i = 0; i < n; i++)
	{
		if (abs(a[i]) > max)
			max = a[i];
	}

	return abs(max);
}

double &Vector::operator[] (int i) const {
	if (i < this->n) {
		return this->a[i];
	} else {
		throw 0;
	}
}

Vector Vector::operator*(double m) const
{
	Vector Result = Vector(*this);
	for (int i = 0; i < n; i++)
	{
		Result.a[i] *= m;
	}
 	return Result;
}

Vector Vector::operator/(double m) const
{
	Vector Result = Vector(*this);
	for (int i = 0; i < n; i++)
	{
		Result.a[i] /= m;
	}
	return Result;
}

Vector Vector::operator+(const Vector & obj) const
{
	Vector Result = Vector(obj.n);
	for (int i = 0; i < n; i++)
	{
		Result.a[i] = this->a[i] + obj.a[i];
	}
	return Result;
}

Vector Vector::operator-(const Vector & obj) const
{
	Vector Result = Vector(obj.n);
	for (int i = 0; i < n; i++)
	{
		Result.a[i] = this->a[i] - obj.a[i];
	}
	return Result;
}

Vector &Vector::operator=(const Vector & obj)
{
	if (this != &obj)
	{
		if (n == obj.n)
		{
			for (int i = 0; i < n; i++)
			{
				a[i] = obj[i];
			}
		}
		else
			throw "Cant assign vectors with different sizes";
	}
	return *this;
}

Matrix *Vector::operator&(const Vector & obj)
{
	int m = this->n, n = obj.n;
	Matrix *res = new Matrix(m, n);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			res->ElementAt(i, j) = (*this)[i] * obj[j];
		}
	}
	return res;
}

Vector *Vector::Lerp(double t, double t0, Vector &v0, double t1, Vector &v1)
{
	return new Vector((v0 * (t1 - t) + v1 * (t - t0)) / (t1 - t0));
}

ostream& operator<<(ostream& os, const Vector& obj) {
	for (int i = 0; i < obj.n; i++)
	{ 
		os << obj.a[i] << " ";
	}
	return os << endl;
}
