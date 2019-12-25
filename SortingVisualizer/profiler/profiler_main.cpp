#include "profiler_main.h"
#include "pofiler.h"
#include "../other/matrix.h"
#include "../fwk/fwk_sort.h"

void prepareArr(ArrayList<int>& arr, int mul) {
	for (int i = 0; i < arr.sz; i++) {
		arr.arr[i].val = (i + 1)*mul;
	}
	for (int i = 0; i < arr.sz; i++) {
		int pos = rand() % arr.sz;
		int tmp = arr.arr[i].val;
		arr.arr[i].val = arr.arr[pos].val;
		arr.arr[pos].val = tmp;
	}
}

bool checkSorted(ArrayList<int>& arr) {
	int tmp = arr.arr[0].val;
	int tmp2;
	for (int i = 1; i < arr.sz; i++) {
		tmp2 = arr.arr[i].val;
		if (tmp2 < tmp)
			return false;
		tmp = tmp2;
	}
	return true;
}

double calcMedAvg(vector<long long>& v) {
	std::sort(v.begin(), v.end());

	double sz = (double)v.size();
	double mid = sz / 2.0;
	double count = medAvg * sz;
	double half = count / 2.0;

	double start = mid - half;
	double end = mid + half;

	int tstart = (int)ceil(start);
	int tend = (int)floor(end);

	double sum = 0.0;

	for (int i = tstart; i < tend; i++) {
		sum += (double)v[i];
	}

	double tastart = (double)tstart;
	double taend = (double)tend;

	if (start < tastart)
		sum += (tastart-start) * (double)v[tstart - 1];
	if (end > taend)
		sum += (end - taend) * (double)v[tend];

	return sum / count;
}

struct ProfilingResult {
	Sort* sort;
	Matrix result;
};

//The amount of which to ignore a term
constexpr double cullThresh = 0.2;

void runProfiler() {
	srand((unsigned int)time(0));
	Matrix m(numTests,numTerms);
	for (int i = 0; i < numTests; i++) {
		int n = testSizes[i];
		int k = n*testElemSizes[i];
		for (int j = 0; j < numTerms; j++) {
			m[i][j] = termFunctions[j]((double)n, (double)k);
		}
	}

	m = m.pseudoinverse();

	vector<ProfilingResult> results;

	Matrix testResults(numTests, 1);

	vector<long long> perTestResults;
	perTestResults.resize(numTestsPer);

	for (auto sort : sorts) {
		if (!sort->profiled)
			continue;

		for (int i = 0; i < numTests; i++) {
			ArrayList<int> arr(testSizes[i]);
			List<int>* lp = &arr; //Required to call sort(), not actually needed
			for (int j = 0; j < numTestsPer; j++) {
				prepareArr(arr,testElemSizes[i]);

				sort->sort(arr,lp);

				perTestResults[j] = (long long)state.accessCount;
			}

			testResults[i][0] = calcMedAvg(perTestResults);
		}

		Matrix result = m * testResults;
		for (int i = 0; i < result.r; i++) {
			auto& e = result[i][0];
			if (/*e >= -cullThresh &&*/ e <= cullThresh)
				e = 0.0;
		}
		results.push_back({ sort,result });
	}

	constexpr long long targetN = 10000;

	std::sort(results.begin(), results.end(), [&](ProfilingResult& a, ProfilingResult& b) {
		double sum = 0.0;
		for (int i = a.result.r-1; i >= 0; i--) {
			sum += (a.result[i][0] - b.result[i][0]) * termFunctions[i](targetN, targetN);
			}
		return sum < 0.0;
		});

	for (int j = 0; j < results.size(); j++) {
		auto& e = results[j];
		printf("Results For %s:\n  accesses(n) = ",e.sort->name.c_str());
		double sum = 0;
		for (int i = e.result.r - 1; i >= 0; i--) {
			sum += e.result[i][0]*termFunctions[i](targetN,targetN);
			if (e.result[i][0] > 0.01) {
				printf(" %.2f%s +", e.result[i][0], termNames[i].c_str());
			}
		}
		printf("\b \n\n");
		//printf("\b \n  Predicted Accesses for n = %lld: %lld\n",targetN,(long long)sum);
	}
}