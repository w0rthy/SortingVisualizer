#pragma once
#include "../fwk/fwk_sort.h"

void shuffleSort(ArrayList<int>& arr, int l, int h);

void selectionSort(ArrayList<int>& arr, int l, int h);

void insertionSort(ArrayList<int>& arr, int l, int h);

extern Sort* sort_shuffle;
extern Sort* sort_bogo_improved;
extern Sort* sort_bubble;
extern Sort* sort_bwradix_lsd;
extern Sort* sort_bwradix_lsd_inplace;
extern Sort* sort_bwradix_msd;
extern Sort* sort_bwradix_msd_inplace;
extern Sort* sort_cocktail_shaker;
extern Sort* sort_counting;
extern Sort* sort_gravity;
extern Sort* sort_heap_max;
extern Sort* sort_insertion;
extern Sort* sort_merge_agg_oop;
extern Sort* sort_merge_inplace_standard;
extern Sort* sort_merge_inplace_shaker;
extern Sort* sort_merge_inplace_weave;
extern Sort* sort_merge_oop;
extern Sort* sort_pancake;
extern Sort* sort_quick;
extern Sort* sort_quick_linked;
extern Sort* sort_selection;
extern Sort* sort_selection_linked;
extern Sort* sort_shell;