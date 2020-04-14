#define MATTYPE_KEEP
#include "matrix.h"

Matrix::Matrix(const Matrix& a) {
	r = a.r;
	c = a.c;
	mat = a.mat;
}

Matrix::Matrix(int r, int c) {
	mat.resize(r);
	for (auto& v : mat)
		v.resize(c, (MATTYPE)0);

	this->r = r;
	this->c = c;
}

void Matrix::print() {
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			printf("%f ", mat[i][j]);
		}
		printf("\n");
	}
}

void Matrix::identity() {
	int dim = r < c ? r : c;
	for (int i = 0; i < dim; i++) {
		mat[i].clear();
		mat[i].resize(r, (MATTYPE)0);
		mat[i][i] = (MATTYPE)1;
	}
}

//TODO: Can use m instead of the `tmp` matrices, since it is pbv
Matrix Matrix::operator+(Matrix m) {
	if (r != m.r || c != m.c)
		return (*this);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			m[i][j] += (*this)[i][j];
		}
	}
	return m;
}

Matrix Matrix::operator-(Matrix m) {
	if (r != m.r || c != m.c)
		return (*this);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			m[i][j] = (*this)[i][j] - m[i][j];
		}
	}
	return m;
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

Matrix Matrix::solveFor(Matrix y) {
	return pseudoinverse() * y;
}

Matrix Matrix::solveFor(Matrix y, function<Matrix(Matrix)> pruneFunc, int iterations) {
	//Initial solution
	Matrix pinv = pseudoinverse();
	Matrix sol = pinv * y;

	for (int i = 0; i < sol.r; i++) {
		for (int j = 0; j < sol.c; j++)
			printf("%f ", sol[i][j]);
		printf("\n");
	}
	printf("\n");

	for (int i = 0; i < iterations; i++) {
		//Prune the solution
		sol = pruneFunc(sol);
		//Test pruned solution
		Matrix tmp = y - ((*this) * sol);
		//Calculate new solution
		sol = sol + pinv * tmp;
	}

	sol = pruneFunc(sol);
	for (int i = 0; i < sol.r; i++) {
		for (int j = 0; j < sol.c; j++)
			printf("%f ", sol[i][j]);
		printf("\n");
	}
	printf("\n");

	return pruneFunc(sol);
}

//RCLMatrix
RCLMatrix::RCLMatrix(const RCLMatrix& a) : Matrix(a) {
	real = a.real;
	rindx = a.rindx;
	cindx = a.cindx;
	rbindx = a.rbindx;
	cbindx = a.cbindx;
}

RCLMatrix::RCLMatrix(Matrix a) : Matrix(a) {
	real = a;
	rindx.resize(r);
	cindx.resize(c);
	rbindx.resize(r);
	cbindx.resize(c);

	int i;
	for (i = 0; i < r; i++) {
		rindx[i] = i;
		rbindx[i] = i;
	}
	for (i = 0; i < c; i++) {
		cindx[i] = i;
		cbindx[i] = i;
	}
}

RCLMatrix RCLMatrix::operator+(Matrix m) {
	m = apply(m);

	RCLMatrix tmp = (*this);
	tmp.mat = ((Matrix)tmp + m).mat;
	return tmp;
}

RCLMatrix RCLMatrix::operator-(Matrix m) {
	m = apply(m);

	RCLMatrix tmp = (*this);
	tmp.mat = ((Matrix)tmp - m).mat;
	return tmp;
}

RCLMatrix RCLMatrix::operator*(Matrix m) {
	m = applyTC(m);
	
	RCLMatrix tmp((Matrix)(*this) * m);

	tmp.real = Matrix(real.r, m.c);
	tmp.rindx = rindx;
	tmp.rbindx = rbindx;

	return tmp;
}

RCLMatrix Matrix::operator*(RCLMatrix m) {
	Matrix t = m.applyTR(*this);
	
	RCLMatrix tmp(t * (Matrix)m);

	tmp.real = Matrix(t.r, m.real.c);
	tmp.cindx = m.cindx;
	tmp.cbindx = m.cbindx;

	return tmp;
}

//Not sure how usable of a behavior this is
RCLMatrix RCLMatrix::transpose() {
	RCLMatrix tmp = (*this);

	tmp.real = real.transpose();
	tmp.mat = ((Matrix)tmp).transpose().mat;

	tmp.rindx = cindx;
	tmp.cindx = rindx;

	tmp.rbindx = cbindx;
	tmp.cbindx = rbindx;

	tmp.r = c;
	tmp.c = r;

	return tmp;
}

RCLMatrix RCLMatrix::inverse() {
	RCLMatrix tmp = (*this);

	tmp.mat = ((Matrix)tmp).inverse().mat;
	tmp.real = Matrix(tmp.real.r, tmp.real.c); //0 Out the real matrix for intended unlocking behavior

	return tmp;
}

RCLMatrix RCLMatrix::pseudoinverse() {
	RCLMatrix tmp = (*this);

	Matrix pinv = ((Matrix)tmp).pseudoinverse();

	if (tmp.r != tmp.c && pinv.r != tmp.r) {
		tmp.real = Matrix(tmp.real.c, tmp.real.r); //0 Out the real matrix and transpose it

		tmp.rindx = cindx;
		tmp.cindx = rindx;

		tmp.rbindx = cbindx;
		tmp.cbindx = rbindx;
	}
	else
		tmp.real = Matrix(tmp.real.r, tmp.real.c); //Don't transpose if pseudoinverse fails

	tmp.mat = pinv.mat;
	tmp.r = pinv.r;
	tmp.c = pinv.c;

	return tmp;
}

void RCLMatrix::lockR(int a) {
	if (a >= r)
		return;
	
	int tmp = rbindx[a];
	for (int i = 0; i < c; i++)
		real[tmp][cbindx[i]] = mat[a][i];

	mat.erase(mat.begin() + a);
	rbindx.erase(rbindx.begin() + a);
	rindx[a] = -1;
	for (int i = a + 1; i < real.r; i++)
		rindx[i]--;
	r--;
}
void RCLMatrix::lockC(int a) {
	if (a >= c)
		return;

	int tmp = cbindx[a];
	for (int i = 0; i < r; i++)
		real[rbindx[i]][tmp] = mat[i][a];

	for (int i = 0; i < r; i++) {
		mat[i].erase(mat[i].begin() + a);
	}
	cbindx.erase(cbindx.begin() + a);
	cindx[a] = -1;
	for (int i = a + 1; i < real.c; i++)
		cindx[i]--;
	c--;
}

Matrix RCLMatrix::apply(Matrix m) {
	if (real.r != m.r || real.c != m.c)
		return m;

	RCLMatrix tmp(m);
	int i;
	for (i = 0; i < real.r; i++)
		if (rindx[i] < 0)
			tmp.lockR(i);
	for (i = 0; i < real.c; i++)
		if (cindx[i] < 0)
			tmp.lockC(i);
	return tmp;
}

Matrix RCLMatrix::applyTR(Matrix m) {
	if (real.r != m.c)
		return m;

	RCLMatrix tmp(m);
	for (int i = 0; i < real.r; i++) {
		if(rindx[i]<0)
			tmp.lockC(i);
	}
	return tmp;
}
Matrix RCLMatrix::applyTC(Matrix m) {
	if (real.c != m.r)
		return m;

	RCLMatrix tmp(m);
	for (int i = 0; i < real.c; i++) {
		if (cindx[i]<0)
			tmp.lockR(i);
	}
	return tmp;
}

Matrix RCLMatrix::unlocked() {
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			real[rbindx[i]][cbindx[j]] = mat[i][j];
		}
	}
	return real;
}