#include "Matrix.h"

using namespace std;

Matrix::~Matrix() {
	for (int i = 0; i < m; i++)
	{
		delete[] this->a[i];
	}
	delete[] this->a;
}

Matrix::Matrix(const Matrix &obj) {
	this->m = obj.m;
	this->n = obj.n;
	this->a = new double*[m];
	for (int i = 0; i < m; i++)
	{
		this->a[i] = new double[n];
		for (int j = 0; j < n; j++)
		{
			this->a[i][j] = obj.a[i][j];
		}
	}
}

Matrix::Matrix(int m, int n, double val) {
	this->m = m;
	this->n = n;
	this->a = new double*[m];
	for (int i = 0; i < m; i++)
	{
		this->a[i] = new double[n];
		for (int j = 0; j < n; j++)
		{
			this->a[i][j] = val;
		}
	}
}

Matrix::Matrix(int m, int n) {
	this->m = m;
	this->n = n;
	this->a = new double*[m];
	for (int i = 0; i < m; i++)
	{
		this->a[i] = new double[n];
	}
}

Matrix Matrix::Identity(int m, int n)
{
	Matrix A = Matrix(m, n);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			A.a[i][j] = i == j ? 1.0 : 0.0;
		}
	}
	return A;
}

double &Matrix::ElementAt(int m, int n) const {
	if (m <= this->m && n <= this->n)
	{
		return this->a[m][n];
	}
	else {
		throw 0;
	}
}

Matrix Matrix::operator * (const Matrix &obj) const {
	Matrix n = Matrix(this->m, obj.n);
	for (int i = 0; i < this->m; i++)
	{
		for (int j = 0; j < this->n; j++)
		{
			double sum = 0;
			for (int k = 0; k < this->n; k++)
			{
				sum += this->a[i][k] * obj.a[k][i];
			}
			n.a[i][j] = sum;
		}
	}
	return n;
}

Matrix Matrix::operator * (double val) const {
	Matrix n = Matrix(*this);
	for (int i = 0; i < this->m; i++)
	{
		for (int j = 0; j < this->n; j++)
		{
			n.a[i][j] *= val;
		}
	}
	return n;
}

Matrix & Matrix::operator=(const Matrix & obj)
{
	if (this != &obj)
	{
		if (m == obj.m && n == obj.n)
		{
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					a[i][j] = obj.a[i][j];
				}
			}
		}
		else
			throw "Cant assign matrices with different sizes";
	}
	return *this;
}

Vector Matrix::SolveGE(const Vector &v)
{
	double *tmp = Matrix::Solve(this->a, v);
	Vector res = Vector(tmp, v.Dimension());
	delete[] tmp;
	return res;
}

Matrix Matrix::operator + (const Matrix &obj) const {
	Matrix n = Matrix(*this);
	for (int i = 0; i < this->m; i++)
	{
		for (int j = 0; j < this->n; j++)
		{
			n.a[i][j] += obj.a[i][j];
		}
	}
	return n;
}

Matrix Matrix::operator - (const Matrix &obj) const {
	Matrix n = Matrix(*this);
	for (int i = 0; i < this->m; i++)
	{
		for (int j = 0; j < this->n; j++)
		{
			n.a[i][j] -= obj.a[i][j];
		}
	}
	return n;
}

Vector Matrix::GetColumn(int n) {
	Vector res = Vector(this->m);
	for (int i = 0; i < m; i++)
	{
		res[i] = this->a[i][n];
	}
	return res;
}

ostream& operator<<(ostream& os, const Matrix& obj) {
	for (int i = 0; i < obj.m; i++)
	{
		for (int j = 0; j < obj.n; j++)
		{
			os << obj.a[i][j] << " ";
		}
		os << "\n";
	}
	return os << endl;
}
