#pragma once
#include "../common.h"

//Test parameters
constexpr int numTests = 5;
//constexpr int testSizes[] = { 1,10,100,500,1000,1000 };
constexpr int testSizes[] = { 1,10,100,500,1000,400,500 };
constexpr int testElemSizes[] = { 1,1,1,1,1,1,1 };
constexpr int numTestsPer = 100;
constexpr double medAvg = 0.6;

//Term parameters
constexpr int numTerms = 4;
string termNames[] = { "","n","nlogn","n^2","k" };
function<double(double,double)> termFunctions[] = { 
	[](double n, double k) {return 1; },
	[](double n, double k) {return n; },
	[](double n, double k) {return n * log2(n); },
	[](double n, double k) {return n * n; },
	[](double n, double k) {return k; }
};