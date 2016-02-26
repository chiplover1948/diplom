#include "solver/Solver.h"
#include <emscripten/bind.h>

using namespace emscripten;


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

Gear *GetSolver(double t0, Vector &x0, std::uintptr_t f, Options &opts) {
	return new Gear(t0, x0, reinterpret_cast <RightSide *>(f), opts);
}

Gear *GetLotkaSolver(double t0, Vector &x0, Options &opts) {
	return new Gear(t0, x0, &Lotka, opts);
}

Gear *ChemistrySolver(double t0, Vector &x0, Options &opts) {
	return new Gear(t0, x0, &F, opts);
}

EMSCRIPTEN_BINDINGS(solver) {
	class_<Vector>("Vector")
		.constructor<int>()
		.function("SetElement", &Vector::SetElement)
		.function("GetElement", &Vector::GetElement)
		.function("Print", &Vector::Print);
		
	class_<Gear>("Gear")
		.constructor(&GetSolver, allow_raw_pointers())
		.constructor(&GetLotkaSolver, allow_raw_pointers())
		.function("Solve", &Gear::Solve);
			
	class_<Options>("Options")
		.constructor<>();
		
	function("ChemistrySolver", &ChemistrySolver, allow_raw_pointers());
		
	class_<SolPoint>("SolPoint")
		.constructor<double, Vector>()
		.function("Time", &SolPoint::GetTime)
		.function("Solve", &SolPoint::GetSolve)
		.function("Print", &SolPoint::Print);
}
