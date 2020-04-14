#pragma once
#include "../common.h"

struct Sort;

using profileFunc = function<int(int)>;

profileFunc profileSort(Sort* sort);