#ifndef MST_H
#define MST_H

#include <stdint.h>
#include "graph.h"
#include "heap_min.h"
#include "types.h"

// Struktura przechowująca rodzica i wagę krawędzi w MST
typedef struct Parent_weight {
    U32f parent;
    U32f weight;
} Parent_weight;

// Struktura zwracaja wynik MST
typedef struct Res_prim {
    Parent_weight *parent_weight;
    U32f sum_weights;
} Res_prim;




typedef struct Res_kruskal {
    Edge *edges;
    U32f sum_weights;
} Res_kruskal;

// Funkcja implementująca algorytm Prima, zwracająca tablicę Parent_weight
Res_prim* prim_list(Graph *graph, U32f start_vertex);
// Funkcja implementująca algorytm Prima dla macierzy incydencji
Res_prim* prim_matrix(U32f *inc_matrix, U32f numVertices, U32f density, U32f start_vertex);
// Funkcja implementująca algorytm Kruskala dla listy sąsiedztwa
Res_kruskal* kruskal_list(Graph *graph, U32f num_edges);
// Funkcja implementująca algorytm Kruskala dla macierzy incydencji
Res_kruskal* kruskal_matrix(U32f *inc_matrix, U32f num_verticies, U32f density);

// Funkcja zwalniająca pamięć dla wyniku MST
void free_res_prim(Res_prim *result);
// Funkcja zwalniająca pamięć dla wyniku Kruskala
void free_res_kruskal(Res_kruskal *result);

#endif // MST_H