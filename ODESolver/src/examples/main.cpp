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
	Vector x0 = Vector(3);
	x0[0] = 1.0; x0[1] = 0.0; x0[2] = 0.0;
	
	/*Vector x0 = Vector(19);
	
    x0[0] = 1000.0;
    x0[1] = 100000.0;
    x0[2] = 1000.0;
    x0[3] = 1000.0;
    x0[6] = 1000.0;
    x0[11] = 3000000.0;
    x0[12] = 30000.0;*/
	Options opts = Options();
	opts.MaxStep = 1.0;
	Gear solver = Gear(0.0, x0, &F, opts);
    int count = 0;
        
    start = clock();
        SolPoint s = SolPoint(0.0, x0);
        do{
        	count++;
            //std::cout <<"time=" << s.GetTime() << ": " << s.GetSolve()[0] << " " << s.GetSolve()[1] << std::endl;            
        } while ((s = solver.Solve()).GetTime() < 500.0);
    
    end = clock();
    float diff = (((float)end - (float)start) / 1000000.0F ) * 1000;   
        std::cout << "process took " << diff << " milliseconds, " << count << " points, " << solver.fails << " fails."  << std::endl;
	std::cin >> i;
	return 0;
}  
