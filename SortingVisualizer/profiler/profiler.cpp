#include "pofiler.h"
#include "../other/matrix.h"
#include "../sorts/sorts.h"
#include <sstream>
#include <iomanip>

constexpr int testsPer = 10;
constexpr int testSizes[] = { 1,10,20,30,40,50,60,70,80,90,100 };
constexpr int testNum = sizeof(testSizes) / sizeof(*testSizes);
function<double(double)> terms[] = {
	[&](double n) {return 1.0; },
	[&](double n) {return log2(n); },
	[&](double n) {return n; },
	[&](double n) {return n * log2(n); },
	[&](double n) {return n * n; }
};
const char* termStr[] = {"","logn","n","nlogn","n^2"};
constexpr int termNum = sizeof(terms) / sizeof(*terms);
constexpr double termThresh = 0.2;

//Number of elements for which to calculate the ranking
vector<Sort*> profilerRanking;

bool ready = false;
Matrix profileMat;

void profile_prepare() {
	if (ready)
		return;

	profileMat = Matrix(testNum, termNum);
	for (int i = 0; i < testNum; i++)
		for (int j = 0; j < termNum; j++)
			profileMat[i][j] = terms[j]((double)testSizes[i]);

	ready = true;
}

ArrayList<int>* genArr(int sz) {
	ArrayList<int>* tmp = new ArrayList<int>(sz);
	for (int i = 0; i < sz; i++)
		(*tmp)[i] = i + 1;
	return tmp;
}

profileFunc profileSort(Sort* sort) {
	profile_prepare();

	//Build y vector
	Matrix y(testNum, 1);
	for (int i = 0; i < testNum; i++) {
		vector<double> res;
		res.resize(testsPer);
		auto arr = genArr(testSizes[i]);
		for (int j = 0; j < testsPer; j++) {
			sort_shuffle->sortRaw(*arr);
			sort->sortRaw(*arr);
			res[j] = (double)state.accessCount;
		}
		delete arr;
		std::sort(res.begin(), res.end());
		y[i][0] = res[testsPer / 2]; //Take median. Could use a mediated average instead
	}
	state.token_sort--;

	//Solve
	RCLMatrix m(profileMat);

	Matrix result;

	bool done = false;
	while (!done) {
		done = true;
		result = m.pseudoinverse().unlocked() * y;
		for (int i = result.r - 1; i >= 0; i--) {
			if (m.isCLocked(i))
				continue;
			if (result[i][0] < termThresh) {
				done = false;
				m.lockAC(i);
				break;
			}
		}
	}

	profileFunc func = [&, result](int n) {
		double sum = 0.0;
		for (int i = 0; i < termNum; i++)
			sum += result.mat[i][0] * terms[i]((double)n);
		return (int)sum;
	};

	std::stringstream funcsstr;
	int count = 0;
	for (int i = termNum-1; i >= 0; i--) {
		double term = result.mat[i][0];
		if (term > 0.0) {
			if (count)
				funcsstr << " + ";
			funcsstr << std::fixed << std::setprecision(2) << term;
			funcsstr << termStr[i];
			count++;
		}
	}
	sort->accessFuncStr = funcsstr.str();

	//Update rankings
	if (sort->ranked) {
		int tmp = func(profilerRankN);
		auto pos = std::find_if(profilerRanking.begin(), profilerRanking.end(), [&](Sort* a) {return a == sort; });
		auto to = std::find_if(profilerRanking.begin(), profilerRanking.end(), [&](Sort* a) {return a != sort && a->accessFunc(profilerRankN) >= tmp; });
		if (pos == profilerRanking.end())
			profilerRanking.insert(to, sort);
		else
			if (pos < to)
				std::rotate(pos, pos + 1, to);
			else
				std::rotate(to, pos, pos + 1);
	}

	return func;
}