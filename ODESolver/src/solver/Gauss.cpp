#include "Matrix.h"

using namespace std;

double *Matrix::Solve(double *a[], Vector b)
{
	int n = b.Dimension();
	int *map = new int[n];
	for (int i = 0; i < n; i++)
	{
		map[i] = i;
	}
	double *x = new double[n];
	for (int i = 0; i < n; i++)
	{
		x[i] = 0.0;
	}
	for (int j = 0; j < n; j++)
	{
		// Find row with largest absolute value of j-st element
		int maxIdx = 0;
		for (int i = 0; i < n - j; i++) {
			if (abs(a[i][j]) > abs(a[maxIdx][j]))
				maxIdx = i;
		}

		if (abs(a[maxIdx][j]) < 1e-12)
			throw "Cannot apply Gauss method";

		// Divide this row by max value
		for (int i = j + 1; i < n; i++)
			a[maxIdx][i] /= a[maxIdx][j];
		b[maxIdx] /= a[maxIdx][j];
		a[maxIdx][j] = 1.0;

		// Move this row to bottom
		if (maxIdx != n - j - 1)
		{
			double *temp = a[n - j - 1];
			a[n - j - 1] = a[maxIdx];
			a[maxIdx] = temp;

			double temp3 = b[n - j - 1];
			b[n - j - 1] = b[maxIdx];
			b[maxIdx] = temp3;

			int temp2 = map[n - j - 1];
			map[n - j - 1] = map[maxIdx];
			map[maxIdx] = temp2;
		}

		double *an = a[n - j - 1];
		// Process all other rows
		for (int i = 0; i < n - j - 1; i++)
		{
			double *aa = a[i];
			if (aa[j] != 0)
			{
				for (int k = j + 1; k < n; k++)
					aa[k] -= aa[j] * an[k];
				b[i] -= aa[j] * b[n - j - 1];
				aa[j] = 0;
			}
		}
	}

	// Build answer
	for (int i = 0; i < n; i++)
	{
		double s = b[i];
		for (int j = n - i; j < n; j++)
			s -= x[j] * a[i][j];
		x[n - i - 1] = s;
	}
	delete[] map;
	return x;
}
