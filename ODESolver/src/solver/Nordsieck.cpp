#include "Nordsieck.h"

using namespace std;

Matrix *NordsieckState::ZNew(const Matrix &arg)
{
	Matrix *res = new Matrix(arg);
	int q = arg.ColumnDimension();
	int n = arg.RowDimension();


	for (int k = 0; k < q - 1; k++)
	{
		for (int j = q - 1; j > k; j--)
		{
			for (int i = 0; i < n; i++)
			{
				res->ElementAt(i, j - 1) = res->ElementAt(i, j) + res->ElementAt(i, j - 1);
			}
		}
	}

	return res;
}

Matrix *NordsieckState::Rescale(const Matrix &arg, double r)
{
	Matrix *res = new Matrix(arg); ;
	double R = 1;
	int q = res->ColumnDimension();

	for (int j = 1; j < q; j++)
	{
		R = R * r;
		for (int i = 0; i < res->RowDimension(); i++)
		{
			res->ElementAt(i, j) = res->ElementAt(i, j) * R;
		}
	}
	return res;
}

Matrix *NordsieckState::Jacobian(RightSide * rightSide, Vector & x, double t)
{
	int N = x.Dimension();
	Matrix *J = new Matrix(N, N);

	Vector *variation = new Vector(N, 0.0);
	for (int i = 0; i < N; i++)
	{
		(*variation)[i] = sqrt(1e-6 * max(1e-5, abs(x[i])));
	}

	
	double *res = new double[N];
	rightSide(t, x.GetArray(), res);
	Vector *fold = new Vector(res, N);
	delete[] res;

	Vector **fnew = new Vector*[N];
	for (int i = 0; i < N; i++)
	{
		x[i] = x[i] + (*variation)[i];
		res = new double[N];
		rightSide(t, x.GetArray(), res);
		fnew[i] = new Vector(res, N);
		delete[] res;
		x[i] = x[i] - (*variation)[i];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{

			J->ElementAt(i, j) = ((*fnew[j])[i] - (*fold)[i]) / ((*variation)[j]);

		}
	}


	DELETE(variation);
	for (int i = 0; i < N; i++)
		DELETE(fnew[i]);
	delete[] fnew;
	DELETE(fold);

	return J;
}


NordsieckState::~NordsieckState() {
	DELETE(zn)
	DELETE(xn)
	DELETE(en)
}

NordsieckState & NordsieckState::operator=(const NordsieckState &obj)
{
	if (this != &obj)
	{
		*en = *obj.en;
		*xn = *obj.xn;
		*zn = *obj.zn;

		tn = obj.tn;
		dt = obj.dt;
		Dq = obj.Dq;
		delta = obj.delta;
		qn = obj.qn;
		qmax = obj.qmax;
		nsuccess = obj.nsuccess;
		rFactor = obj.rFactor;
	}
	return *this;
}


const double NordsieckState::epsilon = 1e-12;
