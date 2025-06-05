#include "graph.h"
#include "heap_min.h"
#include "mst.h"
#include "quicksort.h"

/* prim_list
 * Implementuje algorytm Prima dla reprezentacji listowej grafu.
 * Zwraca wskaźnik na strukturę z wynikami MST.
 */
Res_prim *prim_list(Graph *graph, U32f start_vertex){
    U32f v_bin = 1 + graph->num_v / 32;
    U32 *visited = (U32 *)malloc(v_bin * sizeof(U32));
    for (U32f i = 0; i < v_bin; i++) {
        visited[i] = 0;
    }
    Res_prim *result = (Res_prim *)malloc(sizeof(Res_prim));
    Parent_weight *res = (Parent_weight *)calloc(graph->num_v, sizeof(Parent_weight));
    Min_heap *min_heap = create_min_heap(graph->num_v);
    for (U32f i = 0; i < graph->num_v; i++) {
        res[i].parent = UINT_FAST32_MAX;
        res[i].weight = UINT_FAST32_MAX;
        min_heap_insert(min_heap, i, UINT_FAST32_MAX);
    }
    result->sum_weights = 0;
    res[start_vertex].weight = 0;
    res[start_vertex].parent = start_vertex;
    min_heap_decrease_key(min_heap, start_vertex, 0);

    while (min_heap->size > 0) {
        Min_heap_node min_node = min_heap_extract_min(min_heap);
        U32f u = min_node.vertex;
        if (visited[u / 32] & (1 << (u % 32))) {
            continue;
        }
        visited[u / 32] |= (1 << (u % 32));
        result->sum_weights += min_node.weight;

        Node *adj_list = graph->adjLists[u];
        while (adj_list) {
            U32f v = adj_list->vertex;
            U32f weight = adj_list->weight;
            if (!(visited[v / 32] & (1 << (v % 32))) && weight < res[v].weight) {
                res[v].parent = u;
                res[v].weight = weight;
                min_heap_decrease_key(min_heap, v, weight);
            }
            adj_list = adj_list->next;
        }
    }
    free(visited);
    free_min_heap(min_heap);
    result->parent_weight = res;
    
    return result;
}

/* get_second_vertex
 * Zwraca drugi wierzchołek krawędzi w macierzy incydencji, pomijając start_vertex.
 */
U32f get_second_vertex(U32f *inc_matrix, U32f num_verticies, U32f density, U32f column, U32f start_vertex) {
    for (U32f i = 0; i < num_verticies; i++) {
        if (inc_matrix[i * density + column] > 0 && i != start_vertex) {
            return i;
        }
    }
    return UINT_FAST32_MAX;
}

/* prim_matrix
 * Implementuje algorytm Prima dla reprezentacji macierzowej grafu.
 * Zwraca wskaźnik na strukturę z wynikami MST.
 */
Res_prim *prim_matrix(U32f *inc_matrix, U32f num_verticies, U32f density, U32f start_vertex) {
    U32f v_bin = 1 + num_verticies / 32;
    U32 *visited_v = (U32 *)malloc(v_bin * sizeof(U32));
    for (U32f i = 0; i < v_bin; i++) {
        visited_v[i] = 0;
    }
    U32 e_bin = 1 + density / 32;
    U32 *visited_e = (U32 *)malloc(e_bin * sizeof(U32));
    for (U32f i = 0; i < e_bin; i++) {
        visited_e[i] = 0;
    }
    Res_prim *result = (Res_prim *)malloc(sizeof(Res_prim));
    Parent_weight *res = (Parent_weight *)calloc(num_verticies, sizeof(Parent_weight));
    Min_heap *min_heap = create_min_heap(num_verticies);
    for (U32f i = 0; i < num_verticies; i++) {
        res[i].parent = UINT_FAST32_MAX;
        res[i].weight = UINT_FAST32_MAX;
        min_heap_insert(min_heap, i, UINT_FAST32_MAX);
    }
    result->sum_weights = 0;
    res[start_vertex].weight = 0;
    res[start_vertex].parent = 0;
    min_heap_decrease_key(min_heap, start_vertex, 0);

    while (min_heap->size > 0) {
        Min_heap_node min_node = min_heap_extract_min(min_heap);
        U32f u = min_node.vertex;
        if (visited_v[u / 32] & (1 << (u % 32))) {
            continue;
        }
        visited_v[u / 32] |= (1 << (u % 32));
        result->sum_weights += min_node.weight;
        for (U32f e = 0; e < density; e++) {
            if (visited_e[e / 32] & (1 << (e % 32))) {
                continue;
            }
            U32f weight = inc_matrix[u * density + e];
            U32f v = get_second_vertex(inc_matrix, num_verticies, density, e, u);
            if (weight > 0 && !(visited_e[e / 32] & (1 << (e % 32)))) {
                if (v != UINT_FAST32_MAX && weight < res[v].weight) {
                    res[v].parent = u;
                    res[v].weight = weight;
                    min_heap_decrease_key(min_heap, v, weight);
                    visited_e[e / 32] |= (1 << (e % 32));
                }
            }
        }
    }
    free(visited_v);
    free(visited_e);
    free_min_heap(min_heap);
    result->parent_weight = res;

    return result;
}

/* find
 * Funkcja pomocnicza do znajdowania korzenia w zbiorze rozłącznym (dla Kruskala).
 */
U32f find(U32f *parent, U32f vertex) {
    if (parent[vertex] != vertex) {
        parent[vertex] = find(parent, parent[vertex]); 
    }
    return parent[vertex];
}

/* union_sets
 * Funkcja pomocnicza do łączenia zbiorów w algorytmie Kruskala.
 */
void union_sets(U32f *parent, U32f *rank, U32f u, U32f v) {
    if (rank[u] < rank[v]) {
        parent[u] = v;
    } else if (rank[u] > rank[v]) {
        parent[v] = u;
    } else {
        parent[v] = u;
        rank[u]++;
    }
}

/* kruskal_list
 * Implementuje algorytm Kruskala dla reprezentacji listowej grafu.
 * Zwraca wskaźnik na strukturę z wynikami MST.
 */
Res_kruskal* kruskal_list(Graph *graph, U32f num_edges){
    Res_kruskal *result = (Res_kruskal *)malloc(sizeof(Res_kruskal));
    result->sum_weights = 0;
    Edge *edges = (Edge *)malloc(num_edges * sizeof(Edge));
    if (!edges )  {
        printf("Memory allocation failed\n");
        return NULL;
    }

    U32f edge_count = 0;
    for (U32f i = 0; i < graph->num_v; i++) {
        Node *temp = graph->adjLists[i];
        while (temp) {
            if (i < temp->vertex) {
                edges[edge_count].src = i;
                edges[edge_count].dst = temp->vertex;
                edges[edge_count].weight = temp->weight;
                edge_count++;
            }
            temp = temp->next;
        }
    }
    quicksort(edges, 0, edge_count - 1);

    U32f *parent = (U32f *)malloc(graph->num_v * sizeof(U32f));
    U32f *rank = (U32f *)calloc(graph->num_v, sizeof(U32f));

    for (U32f i = 0; i < graph->num_v; i++) {
        parent[i] = i;
    }
    Edge *mst_edges = (Edge *)malloc((graph->num_v - 1) * sizeof(Edge));
    U32f mst_edge_count = 0;
    for (U32f i = 0; i < edge_count && mst_edge_count < graph->num_v - 1; i++) {
        U32f u_root = find(parent, edges[i].src);
        U32f v_root = find(parent, edges[i].dst);
        if (u_root != v_root) {
            union_sets(parent, rank, u_root, v_root);
            mst_edges[mst_edge_count++] = edges[i];
            result->sum_weights += edges[i].weight;
        }
    }

    free(edges);
    free(parent);
    free(rank);
    result->edges = mst_edges;
    return result;
}

/* kruskal_matrix
 * Implementuje algorytm Kruskala dla reprezentacji macierzowej grafu.
 * Zwraca wskaźnik na strukturę z wynikami MST.
 */
Res_kruskal* kruskal_matrix(U32f *inc_matrix, U32f num_verticies, U32f density) {
    Res_kruskal *result = (Res_kruskal *)malloc(sizeof(Res_kruskal));
    result->sum_weights = 0;
    Edge *edges = (Edge *)malloc(density* sizeof(Edge));

    for (U32f i = 0; i < num_verticies; i++) {
        for (U32f j = 0; j < density; j++) {
            if(inc_matrix[i * density + j] > 0) {
                edges[j].src = i;
                edges[j].dst = get_second_vertex(inc_matrix, num_verticies, density, j, i);
                edges[j].weight = inc_matrix[i * density + j];
            }
        }
    }
    quicksort(edges, 0, density - 1);

    U32f *parent = (U32f *)malloc(num_verticies * sizeof(U32f));
    U32f *rank = (U32f *)calloc(num_verticies, sizeof(U32f));

    for (U32f i = 0; i < num_verticies; i++) {
        parent[i] = i;
    }
    Edge *mst_edges = (Edge *)malloc((num_verticies - 1) * sizeof(Edge));
    
    U32f mst_edge_count = 0;
    for (U32f i = 0; i < density && mst_edge_count < num_verticies - 1; i++) {
        U32f u_root = find(parent, edges[i].src);
        U32f v_root = find(parent, edges[i].dst);
        if (u_root != v_root) {
            union_sets(parent, rank, u_root, v_root);
            mst_edges[mst_edge_count++] = edges[i];
            result->sum_weights += edges[i].weight;
        }
    }

    free(edges);
    free(parent);
    free(rank);
    result->edges = mst_edges;
    
    return result;
}

/* free_res_prim
 * Zwalnia pamięć zaalokowaną przez strukturę Res_prim.
 */
void free_res_prim(Res_prim *result) {
    if (result) {
        free(result->parent_weight);
        free(result);
    }
}

/* free_res_kruskal
 * Zwalnia pamięć zaalokowaną przez strukturę Res_kruskal.
 */
void free_res_kruskal(Res_kruskal *result) {
    if (result) {
        free(result->edges);
        free(result);
    }
}
