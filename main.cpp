#include <iostream>
#include "ilcplex\cplex.h"
#include "ilcplex\ilocplex.h"
using namespace std;
int main()
{
	// Model Definition
	IloEnv myenv; // environment object
	IloModel mymodel(myenv); // model object
	IloArray<IloArray<IloNumVarArray>> x(myenv, 9);

	int puzzle[9][9] = {
	{0, 6, 0, 1, 0, 4, 0, 5, 0},
	{0, 0, 8, 3, 0, 5, 6, 0, 0},
	{2, 0, 0, 0, 0, 0, 7, 0, 0},
	{8, 0, 0, 4, 0, 7, 0, 0, 6},
	{0, 0, 6, 0, 0, 0, 3, 0, 0},
	{7, 0, 0, 9, 0, 1, 0, 0, 4},
	{5, 0, 0, 0, 0, 0, 0, 0, 2},
	{0, 0, 7, 2, 0, 6, 9, 0, 0},
	{0, 4, 0, 5, 0, 8, 0, 7, 0}
	};

	for (int i = 0; i < 9; ++i) {
		x[i] = IloArray<IloNumVarArray>(myenv, 9);
		for (int j = 0; j < 9; ++j) {
			x[i][j] = IloNumVarArray(myenv, 9);
			for (int k = 0; k < 9; ++k) {
				x[i][j][k] = IloNumVar(myenv, 0, 1, ILOBOOL); // Tạo biến nhị phân
			}
		}
	}
	
	// Lớp constraint 1: Đảm bảo các hàng có tổng bằng 1
	for (int k = 0; k < 9; ++k) {
		for (int j = 0; j < 9; ++j) {
			IloExpr expr1(myenv);
			for (int i = 0; i < 9; ++i) {
				expr1 += x[i][j][k];
			}
			mymodel.add(expr1 == 1);
		}
	}
	// Lớp constraint 2: Đảm bảo các cột có tổng bằng 1 
		for (int k = 0; k < 9; ++k) {
			for (int i = 0; i < 9; ++i) {
				IloExpr expr2(myenv);
				for (int j = 0; j < 9; ++j) {
					expr2 += x[i][j][k];
				}
				mymodel.add(expr2 == 1);
			}
		}
	// Lớp constraint 3: Đảm bảo các subgrid có tổng bằng 1
	for (int k = 0; k < 9; ++k) {
		IloExpr expr3(myenv);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				expr3 += x[i][j][k];
			}
		}
			mymodel.add(expr3 == 1);
	}
	// Lớp constraint 4: Mỗi ô chỉ có một số được điền
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			IloExpr sum(myenv);
			for (int k = 0; k < 9; ++k) {
				sum += x[i][j][k];
			}
			mymodel.add(sum == 1);
			sum.end();
		}
	}
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			if (puzzle[i][j] != 0) {
				mymodel.add(x[i][j][puzzle[i][j] - 1] == 1);
			}
		}
	}
	
	// Solving the model
	IloCplex cplex(mymodel);
	cplex.solve();

	// Display solution
	if (cplex.getStatus() == IloAlgorithm::Optimal) {
		cout << "Solution:" << std::endl;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 9; ++j) {
				for (int k = 0; k < 9; ++k) {
					if (cplex.getValue(x[i][j][k]) > 0.5) {
						cout << k + 1 << " ";
					}
				}
			}
			cout << std::endl;
		}
	}
	else {
		cout << "No solution found" << std::endl;
	}
	// Closing the Model
	myenv.end();
	return 0;
}