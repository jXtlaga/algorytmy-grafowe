#include "graph.h"
#include "heap_min.h"
#include <stdlib.h>

/* Struktura wynikowa dla algorytmów najkrótszej ścieżki */
typedef struct Res_sp {
    U32f *distances;
    U32f *parents;
} Res_sp;

/* dijkstra_list
 * Implementuje algorytm Dijkstry dla reprezentacji listowej grafu.
 * Zwraca wskaźnik na strukturę z wynikami najkrótszych ścieżek.
 */
Res_sp* dijkstra_list(Graph *graph, U32f start_vertex) {
    if (!graph || start_vertex >= graph->num_v) {
        return NULL;
    }

    Res_sp *result = (Res_sp *)malloc(sizeof(Res_sp));
    result->distances = (U32f *)malloc(graph->num_v * sizeof(U32f));
    result->parents = (U32f *)malloc(graph->num_v * sizeof(U32f));

    Min_heap *min_heap = create_min_heap(graph->num_v);
    U32f v_bin = 1 + graph->num_v / 32; 
    U32* visited = (U32 *)malloc(v_bin * sizeof(U32));
    for (U32f i = 0; i < v_bin; i++) {
        visited[i] = 0;
    }
    for (U32f i = 0; i < graph->num_v; i++) {
        result->distances[i] = UINT_FAST32_MAX;
        result->parents[i] = UINT_FAST32_MAX;
        min_heap_insert(min_heap, i, UINT_FAST32_MAX);
    }
    min_heap_decrease_key(min_heap, start_vertex, 0);
    result->distances[start_vertex] = 0;
    result->parents[start_vertex] = start_vertex;
    visited[start_vertex / 32] |= (1 << (start_vertex % 32));
    
    while (min_heap->size > 0) {
        Min_heap_node min_node = min_heap_extract_min(min_heap);
        U32f u = min_node.vertex;
        visited[u / 32] |= (1 << (u % 32));

        Node *adj_list = graph->adjLists[u];
        while (adj_list) {
            U32f v = adj_list->vertex;
            if (visited[v / 32] & (1 << (v % 32))){
                adj_list = adj_list->next;
                continue;
            }
            U32f weight = adj_list->weight;

            if (result->distances[u] + weight < result->distances[v]) {
                result->distances[v] = result->distances[u] + weight;
                result->parents[v] = u;
                min_heap_decrease_key(min_heap, v, result->distances[v]);
            }
            adj_list = adj_list->next;
        }
    }

    free_min_heap(min_heap);
    free(visited);
    return result;
}

/* dijkstra_matrix
 * Implementuje algorytm Dijkstry dla reprezentacji macierzowej grafu.
 * Zwraca wskaźnik na strukturę z wynikami najkrótszych ścieżek.
 */
Res_sp* dijkstra_matrix(I32f *inc_matrix, U32f num_v, U32f denisty, U32f start_vertex){
    Res_sp *result = (Res_sp *)malloc(sizeof(Res_sp));
    result->distances = (U32f *)malloc(num_v * sizeof(U32f));
    result->parents = (U32f *)malloc(num_v * sizeof(U32f));

    Min_heap *min_heap = create_min_heap(num_v);

    U32f v_bin = 1 + num_v / 32;
    U32 *visited = (U32 *)malloc(v_bin * sizeof(U32));
    for (U32f i = 0; i < v_bin; i++) {
        visited[i] = 0;
    }

    for (U32f i = 0; i < num_v; i++) {
        result->distances[i] = UINT_FAST32_MAX;
        result->parents[i] = UINT_FAST32_MAX;
        min_heap_insert(min_heap, i, UINT_FAST32_MAX);
    }
    min_heap_decrease_key(min_heap, start_vertex, 0);
    result->distances[start_vertex] = 0;
    result->parents[start_vertex] = start_vertex;
    visited[start_vertex / 32] |= (1 << (start_vertex % 32));
    while (min_heap->size > 0) {
        Min_heap_node min_node = min_heap_extract_min(min_heap);
        U32f u = min_node.vertex;
        visited[u / 32] |= (1 << (u % 32));
        for (U32f e = 0; e < denisty; e++) {
            if (inc_matrix[u * denisty + e] >= 0)
                continue;
            for (U32f i = 0; i < num_v; i++) {
                if (visited[i / 32] & (1 << (i % 32)))
                    continue;
                I32f weight = inc_matrix[i * denisty + e];
                if (weight <= 0)
                    continue;
                if (result->distances[u] + weight < result->distances[i]) {
                    result->distances[i] = result->distances[u] + weight;
                    result->parents[i] = u;
                    min_heap_decrease_key(min_heap, i, result->distances[i]);
                }
            }
        }
    }
    free_min_heap(min_heap);
    free(visited);
    return result;
}

/* belman_ford_list
 * Implementuje algorytm Bellmana-Forda dla reprezentacji listowej grafu.
 * Zwraca wskaźnik na strukturę z wynikami najkrótszych ścieżek.
 */
Res_sp* belman_ford_list(Graph *graph, U32f start_vertex) {
    if (!graph || start_vertex >= graph->num_v) {
        return NULL;
    }

    Res_sp *result = (Res_sp *)malloc(sizeof(Res_sp));
    result->distances = (U32f *)malloc(graph->num_v * sizeof(U32f));
    result->parents = (U32f *)malloc(graph->num_v * sizeof(U32f));

    for (U32f i = 0; i < graph->num_v; i++) {
        result->distances[i] = UINT_FAST32_MAX;
        result->parents[i] = UINT_FAST32_MAX;
    }
    
    result->distances[start_vertex] = 0;
    result->parents[start_vertex] = start_vertex;
    char is_changed = 0;
    for (U32f i = 0; i < graph->num_v - 1; i++) {
        is_changed = 0;
        for (U32f u = 0; u < graph->num_v; u++) {
            Node *adj_list = graph->adjLists[u];
            while (adj_list) {
                U32f v = adj_list->vertex;
                U32f weight = adj_list->weight;

                if (result->distances[u] != UINT_FAST32_MAX && 
                    result->distances[u] + weight < result->distances[v]) {
                    result->distances[v] = result->distances[u] + weight;
                    result->parents[v] = u;
                    is_changed = 1;
    
                }
                adj_list = adj_list->next;
            }
        }
        if (!is_changed) {
            break;
        }
    }
    return result;
}

/* belman_ford_matrix_no_edge_list
 * Implementuje algorytm Bellmana-Forda dla macierzy incydencji (bez listy krawędzi).
 * Zwraca wskaźnik na strukturę z wynikami najkrótszych ścieżek.
 */
Res_sp* belman_ford_matrix_no_edge_list(I32f *inc_matrix, U32f num_v, U32f density, U32f start_vertex) {
    if (!inc_matrix || start_vertex >= num_v) {
        return NULL;
    }

    Res_sp *result = (Res_sp *)malloc(sizeof(Res_sp));
    result->distances = (U32f *)malloc(num_v * sizeof(U32f));
    result->parents = (U32f *)malloc(num_v * sizeof(U32f));

    for (U32f i = 0; i < num_v; i++) {
        result->distances[i] = INT_FAST32_MAX;
        result->parents[i] = INT_FAST32_MAX;
    }
    
    result->distances[start_vertex] = 0;
    result->parents[start_vertex] = start_vertex;
    char is_changed = 0;
    for (U32f i = 0; i < num_v - 1; i++) {
        is_changed = 0;
        for (U32f u = 0; u < num_v; u++) {
            for (U32f e = 0; e < density; e++) {
                if (inc_matrix[u * density + e] >= 0 || result->distances[u] == UINT_FAST32_MAX)
                    continue;
                for (U32f v = 0; v < num_v; v++) {
                    if (v == u)
                        continue;
                    I32f weight = inc_matrix[v * density + e];
                    if (weight <= 0)
                        continue;

                    if (result->distances[u] + weight < result->distances[v]) {
                        result->distances[v] = result->distances[u] + weight;
                        result->parents[v] = u;
                        is_changed = 1;
                        break;
                    }
                }
            }
        }
        if (!is_changed) {
            break;
        }
    }
    return result;
}

/* belman_ford_matrix_edge_list
 * Implementuje algorytm Bellmana-Forda dla macierzy incydencji z listą krawędzi.
 * Zwraca wskaźnik na strukturę z wynikami najkrótszych ścieżek.
 */
Res_sp* belman_ford_matrix_edge_list(I32f *inc_matrix, U32f num_v, U32f density, U32f start_vertex) {
    if (!inc_matrix || start_vertex >= num_v) {
        return NULL;
    }

    Res_sp *result = (Res_sp *)malloc(sizeof(Res_sp));
    result->distances = (U32f *)malloc(num_v * sizeof(U32f));
    result->parents = (U32f *)malloc(num_v * sizeof(U32f));

    for (U32f i = 0; i < num_v; i++) {
        result->distances[i] = UINT_FAST32_MAX;
        result->parents[i] = UINT_FAST32_MAX;
    }
    Edge *edges = (Edge *)malloc(density * sizeof(Edge));

    for (U32f i = 0; i < num_v; i++) {
        for (U32f e = 0; e < density; e++) {
            if (inc_matrix[i * density + e] > 0) {
                edges[e].dst= i;
                edges[e].weight = inc_matrix[i * density + e];
            }
            else if (inc_matrix[i * density + e] < 0) {
                edges[e].src = i;
            }
        }
    } 
    result->distances[start_vertex] = 0;
    result->parents[start_vertex] = start_vertex;
    char is_changed = 0;
    for (U32f i = 0; i < num_v - 1; i++) {
        is_changed = 0;
        for (U32f e = 0; e < density; e++) {
            U32f u = edges[e].src;
            U32f v = edges[e].dst;
            I32f weight = edges[e].weight;

            if (result->distances[u] != UINT_FAST32_MAX && 
                result->distances[u] + weight < result->distances[v]) {
                result->distances[v] = result->distances[u] + weight;
                result->parents[v] = u;
                is_changed = 1;
            }
        }
        if (!is_changed) {
            break;
        }
    }
    free(edges); 
    return result;
}

/* free_res_dijkstra
 * Zwalnia pamięć zaalokowaną przez strukturę Res_sp.
 */
void free_res_dijkstra(Res_sp *result) {
    if (result) {
        free(result->distances);
        free(result->parents);
        free(result);
    }
}
