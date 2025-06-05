#ifndef SP_H
#define SP_H

#include <stdint.h>
#include "graph.h"
#include "heap_min.h"
#include "types.h"
#define U32f uint_fast32_t

typedef struct Res_sp{
    U32f *distances;
    U32f *parents;
} Res_sp;

// Algorytm Dijkstry dla listy sąsiedztwa
Res_sp* dijkstra_list(Graph *graph, U32f start_vertex);

// Algorytm Dijkstry dla macierzy incydencji
Res_sp* dijkstra_matrix(I32f *inc_matrix, U32f num_v, U32f density, U32f start_vertex);

// Algorytm Bellmana-Forda dla listy sąsiedztwa
Res_sp* belman_ford_list(Graph *graph, U32f start_vertex);

// Algorytm Bellmana-Forda dla macierzy incydencji bez listy krawędzi
Res_sp* belman_ford_matrix_no_edge_list(I32f *inc_matrix, U32f num_v, U32f density, U32f start_vertex);

// Algorytm Bellmana-Forda dla macierzy incydencji z listą krawędzi
Res_sp* belman_ford_matrix_edge_list(I32f *inc_matrix, U32f num_v, U32f density, U32f start_vertex);

// Zwalnianie pamięci wyniku Dijkstry/Bellmana-Forda
void free_res_dijkstra(Res_sp* result);

#endif // SP_H