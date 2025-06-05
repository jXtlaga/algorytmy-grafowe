#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "types.h"

typedef struct Node {
    U32f vertex;
    U32f weight;
    struct Node* next;
} Node;

typedef struct Graph {
    U32f num_v;
    Node** adjLists;
} Graph;

// Tworzy nowy węzeł listy sąsiedztwa
Node* create_node(U32f vertex, U32f weight);

// Tworzy nowy graf
Graph* create_graph(U32f vertices);

// Dodaje krawędź do grafu
void add_edge(Graph* graph, U32f src, U32f dst, U32f weight);

// Zwraca wskaźnik do krawędzi lub NULL
Node* get_edge(Graph* graph, U32f src, U32f dst);

// Sprawdza czy istnieje krawędź (src, dst)
int check_edge(Graph* graph, U32f src, U32f dst);

// Losowe wagi dla grafu nieskierowanego
void set_rand_weights_undir(Graph* graph, U32f min_weight, U32f max_weight);

// Losowe wagi dla grafu skierowanego
void set_rand_weights_dir(Graph* graph, U32f min_weight, U32f max_weight);

// Losowy graf nieskierowany
void create_rand_undir_graph(Graph* graph, I32f density);

// Losowy graf skierowany
void create_rand_dir_graph(Graph* graph, I32f density, U32f start_vertex);

// Macierz incydencji nieskierowana
U32f* create_inc_undir_matrix(Graph* graph, U32f density);

// Macierz incydencji skierowana
I32f *create_inc_dir_matrix(Graph* graph, U32f density);

// Zwalnia pamięć grafu
void free_graph(Graph* graph);

#endif // GRAPH_H
