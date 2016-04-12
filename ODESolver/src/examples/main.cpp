#include "../solver/Gear.h"
#include "../solver/Matrix.h"
#include "../solver/Vector.h"
#include "../solver/Helpers.h"
#include <iostream>
#include <time.h>

void F(double t, double *x, double *res) {
	res[0] = -0.1 * x[0] + 100.0 * x[1] * x[2];
	res[1] = 0.1 * x[0] - 100.0 * x[1] * x[2] - 1000.0 * x[1];
	res[2] = 1000.0 * x[1];
}


void CRN (double t, double *x, double *r) {
    double deg = 0.0008;
    double cat = 0.0008;
    double pol = 0.0167;
    double nick = 0.0167;
    double x0 = 4;
    double ann = 0.01;
    double bind = 5.4e-06;
    double bind2 = 0.001;
    double bind1 = 5e-05;
    double unbind = 0.1126;
    double Cmax = 1000;
    double c = 0.0008;
    double kt = 0.001;
    double ku = 0.001;
    double s = 2;
    double e = 2.71828183;
    double T = 298.15;

    // Assign states
    double L = x[0];
    double L$ = x[1];
    double R = x[2];
    double R$ = x[3];
    double V = x[4];
    double V$ = x[5];
    double Y = x[6];
    double Y$ = x[7];
    double sp9 = x[8];
    double sp10 = x[9];
    double sp11 = x[10];
    double sp12 = x[11];

    // Define reaction propensities
    double r_0 = (cat * R);
    double r_1 = (cat * R$);
    double r_2 = (cat * Y);
    double r_3 = (cat * Y$);
    double r_4 = (deg * sp9);
    double r_5 = (deg * sp10);
    double r_6 = (cat * sp9);
    double r_7 = (cat * sp10);
    double r_8 = (cat * sp9);
    double r_9 = (cat * sp10);
    double r_10 = (cat * sp11);
    double r_11 = (cat * sp12);
    double r_12 = (deg * V);
    double r_13 = (deg * V$);
    double r_14 = ((ann * sp12) * sp11);
    double r_15 = (0.2 * V);
    double r_16 = (0.2 * V$);
    double r_17 = (0.1 * Y);
    double r_18 = (0.1 * Y$);
    double r_19 = ((0.1 * Y) * Y$);
    double r_20 = ((0.01 * Y) * L);
    double r_21 = ((0.01 * Y$) * L$);
    double r_22 = (L$ * L);
    double r_23 = ((ann * R$) * R);
    double r_24 = ((ann * V$) * V);
    double r_25 = ((ann * sp10) * sp9);

    // Assign derivatives
    double dL = -r_22;
    double dL$ = -r_22;
    double dR = -r_23;
    double dR$ = -r_23;
    double dV = r_8 + r_10 - r_12 - r_24;
    double dV$ = r_9 + r_11 - r_13 - r_24;
    double dY = r_15 - r_17 - r_19 - r_20;
    double dY$ = r_16 - r_18 - r_19 - r_21;
    double dsp9 = r_0 + r_3 - r_4 - r_25;
    double dsp10 = r_1 + r_2 - r_5 - r_25;
    double dsp11 = r_6 - r_14;
    double dsp12 = r_7 - r_14;

    r[0] = dL, r[1] = dL$, r[2] = dR, r[3] = dR$, r[4] = dV, r[5] = dV$, r[6] = dY, r[7] = dY$, r[8] = dsp9, r[9] = dsp10, r[10] = dsp11, r[11] = dsp12;
}

void Lotka(double t, double *x, double *r)
{
    // Assign states
    double UnaryRx = x[0];
    double UnaryLxLL_1 = x[1];
    double UnaryLxLL = x[2];
    double SpeciesR = x[3];
    double sp_1 = x[4];
    double sp_0 = x[5];
    double SpeciesL = x[6];
    double sp_3 = x[7];
    double sp_2 = x[8];
    double sp_5 = x[9];
    double sp_4 = x[10];
    double BinaryLRxRR_1 = x[11];
    double BinaryLRxRR = x[12];
    double sp_7 = x[13];
    double sp_9 = x[14];
    double sp_8 = x[15];
    double sp_11 = x[16];
    double sp_10 = x[17];
    double sp_6 = x[18];

    double *dx = r;

    // Assign derivatives
    dx[0] = 0.0;
    dx[1] = 0.0;
    dx[2] = 0.0;
    dx[3] = -1E-05 * UnaryRx * SpeciesR - 1E-05 * BinaryLRxRR * SpeciesR + 0.1 * sp_7 + 2 * 1E-05 * BinaryLRxRR_1 * sp_8 - 1E-05 * sp_6 * SpeciesR;
    dx[4] = 1E-05 * UnaryRx * SpeciesR;
    dx[5] = 1E-05 * UnaryRx * SpeciesR;
    dx[6] = -1E-05 * UnaryLxLL * SpeciesL + 2 * 1E-05 * UnaryLxLL_1 * sp_2 - 1E-05 * BinaryLRxRR * SpeciesL - 1E-05 * sp_7 * SpeciesL + 0.1 * sp_6;
    dx[7] = 1E-05 * UnaryLxLL * SpeciesL;
    dx[8] = 1E-05 * UnaryLxLL * SpeciesL - 1E-05 * UnaryLxLL_1 * sp_2;
    dx[9] = 1E-05 * UnaryLxLL_1 * sp_2;
    dx[10] = 1E-05 * UnaryLxLL_1 * sp_2;
    dx[11] = 0.0;
    dx[12] = 0.0;
    dx[13] = 1E-05 * BinaryLRxRR * SpeciesR - 0.1 * sp_7 - 1E-05 * sp_7 * SpeciesL;
    dx[14] = 1E-05 * sp_7 * SpeciesL + 1E-05 * sp_6 * SpeciesR;
    dx[15] = 1E-05 * sp_7 * SpeciesL - 1E-05 * BinaryLRxRR_1 * sp_8 + 1E-05 * sp_6 * SpeciesR;
    dx[16] = 1E-05 * BinaryLRxRR_1 * sp_8;
    dx[17] = 1E-05 * BinaryLRxRR_1 * sp_8;
    dx[18] = 1E-05 * BinaryLRxRR * SpeciesL - 0.1 * sp_6 - 1E-05 * sp_6 * SpeciesR;

}


int main() {
	int i;
    double dt;
    clock_t start, end;
	Vector x0 = Vector(12);
	x0[2] = 4.0;
	
	/*Vector x0 = Vector(19);
	
    x0[0] = 1000.0;
    x0[1] = 100000.0;
    x0[2] = 1000.0;
    x0[3] = 1000.0;
    x0[6] = 1000.0;
    x0[11] = 3000000.0;
    x0[12] = 30000.0;*/
	Options opts = Options();
    opts.OutputStep = 100.0;
	Gear solver = Gear(0.0, x0, &CRN, opts);
    int count = 0;
        
    start = clock();
        SolPoint s = SolPoint(0.0, x0);
        do{
        	count++;
            std::cout <<"time=" << s.GetTime() << ": " << s.GetSolve()[6] << " " << s.GetSolve()[1] << std::endl;            
        } while ((s = solver.Solve()).GetTime() < 20000.0);
    
    end = clock();
    float diff = (((float)end - (float)start) / 1000000.0F ) * 1000;   
        std::cout << "process took " << diff << " milliseconds, " << count << " points " << std::endl;
	std::cin >> i;
	return 0;
}  
