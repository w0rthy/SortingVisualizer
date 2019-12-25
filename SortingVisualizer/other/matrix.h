#pragma once
#include "../common.h"

#define MATTYPE double

struct Matrix {
	int r, c;
	vector<vector<MATTYPE>> mat;

	Matrix(int r, int c);

	void identity();

	inline vector<MATTYPE>& operator[](int r) {
		return mat[r];
	}

	Matrix operator*(Matrix m);

	Matrix transpose();

	Matrix inverse();

	Matrix pseudoinverse();
};

#ifndef MATTYPE_KEEP
#undef MATTYPE
#endif