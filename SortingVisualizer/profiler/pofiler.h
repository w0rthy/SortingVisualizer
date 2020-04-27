#pragma once
#include "../common.h"

struct Sort;

using profileFunc = function<int(int)>;

profileFunc profileSort(Sort* sort);

extern vector<Sort*> profilerRanking;

constexpr int profilerRankN = 1000;