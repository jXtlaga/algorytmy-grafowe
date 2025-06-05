#include "quicksort.h"
void swapE(Edge *a, Edge *b) {
    Edge temp = *a;
    *a = *b;
    *b = temp;
}
U32f partition(Edge *edges, int left, int right){
    int p_i = (left + right) / 2;
    U32f pivot = edges[p_i].weight;
    swapE(&edges[p_i], &edges[right]);
    int j = left;
    for (int i = left; i < right; i++) {
        if (edges[i].weight < pivot) {
            swapE(&edges[i], &edges[j]);
            j++;
        }
    }
    swapE(&edges[j], &edges[right]);
    return j;
}
void quicksort(Edge *edges, int left, int right) {
    if (left < right) {
        U32f pivot_index = partition(edges, left, right);
        quicksort(edges, left, pivot_index - 1);
        quicksort(edges, pivot_index + 1, right);
    }
}