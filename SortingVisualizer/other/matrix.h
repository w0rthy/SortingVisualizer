#pragma once
#include "../common.h"

#define MATTYPE double

struct RCLMatrix;

struct Matrix {
	int r, c;
	vector<vector<MATTYPE>> mat;

	Matrix() {}
	Matrix(const Matrix& a);
	Matrix(int r, int c);

	void print();

	void identity();

	inline vector<MATTYPE>& operator[](int r) {
		return mat[r];
	}

	Matrix operator+(Matrix m);

	Matrix operator-(Matrix m);

	Matrix operator*(Matrix m);
	RCLMatrix operator*(RCLMatrix m);

	Matrix transpose();

	Matrix inverse();

	Matrix pseudoinverse();

	Matrix solveFor(Matrix y);

	Matrix solveFor(Matrix y, function<Matrix(Matrix)> pruneFunc, int iterations);
};

struct RCLMatrix : public Matrix {
	friend struct Matrix;
private:
	Matrix real; //The real, unlocked matrix
	vector<int> rindx, cindx; //Maps row/columns from original matrix to the locked matrix (-1 if locked)
	vector<int> rbindx, cbindx; //Maps rows/columns in the locked matrix back to rows/columns in the unlocked matrix
public:
	RCLMatrix(const RCLMatrix& a);
	RCLMatrix(Matrix a);

	RCLMatrix operator+(Matrix m);

	RCLMatrix operator-(Matrix m);

	RCLMatrix operator*(Matrix m);

	RCLMatrix transpose();

	RCLMatrix inverse();

	RCLMatrix pseudoinverse();

	//Lock a relative row or column
	void lockR(int a);
	void lockC(int a);

	//Lock an absolute row or column
	inline void lockAR(int a) {
		int tmp = rindx[a];
		if (tmp >= 0)
			lockR(tmp);
	}
	inline void lockAC(int a) {
		int tmp = cindx[a];
		if (tmp >= 0)
			lockC(tmp);
	}

	inline bool isRLocked(int a) {
		return rindx[a] < 0;
	}
	inline bool isCLocked(int a) {
		return cindx[a] < 0;
	}

	//Apply this matrix's current locking scheme to another matrix
	Matrix apply(Matrix m);
	Matrix applyTR(Matrix m);
	Matrix applyTC(Matrix m);

	//Returns the real, unlocked version of this matrix
	Matrix unlocked();
};

#ifndef MATTYPE_KEEP
#undef MATTYPE
#endif