#define MATTYPE_KEEP
#include "matrix.h"

Matrix::Matrix(int r, int c) {
	mat.resize(r);
	for (auto& v : mat)
		v.resize(c, (MATTYPE)0);

	this->r = r;
	this->c = c;
}

void Matrix::identity() {
	int dim = r < c ? r : c;
	for (int i = 0; i < dim; i++) {
		mat[i].clear();
		mat[i].resize(r, (MATTYPE)0);
		mat[i][i] = (MATTYPE)1;
	}
}

Matrix Matrix::operator*(Matrix m) {
	Matrix res(r, m.c);
	if (c != m.r)
		return res;
	MATTYPE sum;
	for (int k = 0; k < m.c; k++) {
		for (int i = 0; i < r; i++) {
			sum = (MATTYPE)0;
			for (int j = 0; j < c; j++) {
				sum += mat[i][j] * m[j][k];
			}
			res[i][k] = sum;
		}
	}
	return res;
}

Matrix Matrix::transpose() {
	Matrix res(c, r);

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			res[j][i] = mat[i][j];
		}
	}

	return res;
}

//Gauss Elim
Matrix Matrix::inverse() {
	Matrix res(r, c);
	if (r != c)
		return res;

	res.identity();

	//Scratch matrix
	Matrix m = (*this);

	//Lower Triangle
	for (int j = 0; j < c; j++) {
		for (int i = j; i < r; i++) {
			if (i == j) {
				MATTYPE div = m[i][j];
				if (div != 0)
					for (int k = 0; k < c; k++) {
						m[i][k] /= div;
						res[i][k] /= div;
					}
				continue;
			}

			MATTYPE amt = m[i][j];
			if (amt != 0)
				for (int k = 0; k < c; k++) {
					m[i][k] -= m[j][k] * amt;
					res[i][k] -= res[j][k] * amt;
				}
		}
	}

	//Finish
	for (int j = c-1; j > 0; j--) {
		for (int i = j - 1; i >= 0; i--) {
			MATTYPE amt = m[i][j];
			for (int k = 0; k < c; k++) {
				res[i][k] -= res[j][k] * amt;
			}
		}
	}

	return res;
}

Matrix Matrix::pseudoinverse() {
	Matrix trans = this->transpose();

	if (r > c)
		return (trans * (*this)).inverse() * trans;
	return trans * ((*this) * trans).inverse();
}