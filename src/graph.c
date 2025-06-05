#include "graph.h"
#include "io_graph.h"
#include <inttypes.h>

/* prufer_to_tree
 * Zamienia ciąg Prüfera na listę krawędzi drzewa.
 * Zwraca wskaźnik na tablicę krawędzi (rozmiar: 2*(len_seq+1)).
 */
U32f* prufer_to_tree(U32f *seq, U32f len_seq){
    U32f *edges = (U32f *)malloc(sizeof(U32f) * 2 * (len_seq + 1));
    int i_edges = 0;
    U32f length = len_seq + 2;
    U32f degree[length];
    for (int i = 0; i < length; i++){
        degree[i] = 1;
    }
    for (int i = 0; i < len_seq;i++){
        U32f node = seq[i];
        degree[node] ++;
    }

    for(int i = 0; i < len_seq; i++){
        U32f node_seq = seq[i];
        for(int j = 0; j < length; j++){
            if (degree[j] == 1){

                edges[i_edges*2] = node_seq;
                edges[i_edges*2+1] = j;
                i_edges++;
                
                degree[j] --;
                degree[node_seq] --;
                break;
            }
        }
    }
    U32f u,v;
    int first = 1; 
    for (U32f i = 0; i < length; i++){
        if (degree[i] == 1){
            if (first == 1){
                first = -1;
                u = i;
            }
            else v = i;

        }
    }
    edges[i_edges*2] = u;
    edges[i_edges*2+1] = v;
    return edges;
}

/* prufer_rand_seq
 * Generuje losowy ciąg Prüfera o zadanej długości.
 */
U32f *prufer_rand_seq(U32f verticies,int (*rng_func)(void)){
    U32f* seq = (U32f *)malloc(sizeof(U32f) * verticies);
    for (int i = 0; i < verticies; i++){
        seq[i] = rng_func() % verticies;
    }
    return seq;
}

/* swap
 * Zamienia miejscami dwie wartości typu U32f.
 */
void swap(U32f *a, U32f *b) {
    U32f temp = *a;
    *a = *b;
    *b = temp;
}

/* shuffle
 * Tasuje dwie tablice (arr_u, arr_v) o długości n.
 */
void shuffle(U32f *arr_u, U32f *arr_v, size_t n) {
    if (n <= 1) return;

    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1); 
        swap(&arr_u[i], &arr_u[j]);
        swap(&arr_v[i], &arr_v[j]);
    }
}

/* create_rand_undir_graph
 * Tworzy losowy graf nieskierowany o zadanej gęstości.
 */
void create_rand_undir_graph(Graph *graph, I32f density){
    U32f verticies = graph->num_v;
    U32f *seq = prufer_rand_seq(verticies-2, rand);
    U32f *edges = prufer_to_tree(seq, verticies-2);
    
    free(seq);
    
    for(int i = 0; i < verticies-1; i++){
        add_edge(graph, edges[i*2], edges[i*2+1], 0);
        add_edge(graph, edges[i*2+1], edges[i*2], 0);
    }

    free(edges);
    U32f max_edges = verticies * (verticies - 1);
    max_edges /= 2;
    U32f numb = max_edges - verticies + 1;
    U32f* candidates_u = (U32f *) malloc(sizeof(U32f) * numb);
    U32f* candidates_v = (U32f *) malloc(sizeof(U32f) * numb);
    U32f ind = 0;
    for (U32f i = 0; i < verticies; i++){
        for (U32f j = i + 1; j < verticies; j++){
            if (check_edge(graph, i, j))
                continue;
            candidates_u[ind] = i;
            candidates_v[ind] = j;
            ind ++;
        }
    }
    shuffle(candidates_u, candidates_v, ind);
    density = density - (verticies - 1);
    if (density > ind)
        density = ind;
    else if (density < 0)
        density = 0;

    for (U32f i = 0; i < density; i++){
        add_edge(graph, candidates_u[i], candidates_v[i], 0);
        add_edge(graph, candidates_v[i], candidates_u[i], 0);
    }
    free(candidates_u);
    free(candidates_v);
}

/* get_dir_tree
 * Tworzy drzewo skierowane na podstawie grafu i zapisuje rodziców w tablicy parent.
 */
void get_dir_tree(Graph *graph, U32f *parent, U32f start_vertex, U32f *visited, U32f *index) {
    visited[start_vertex] = 1;
    for (Node *temp = graph->adjLists[start_vertex]; temp != NULL; temp = temp->next) {
        if (!visited[temp->vertex]) {
            parent[temp->vertex] = start_vertex;
            get_dir_tree(graph, parent, temp->vertex, visited, index);
        }
    }
}

/* create_rand_dir_graph
 * Tworzy losowy graf skierowany o zadanej gęstości i wierzchołku startowym.
 */
void create_rand_dir_graph(Graph *graph, I32f density, U32f start_vertex){
    U32f verticies = graph->num_v;
    U32f *seq = prufer_rand_seq(verticies-2, rand);
    U32f *edges = prufer_to_tree(seq, verticies-2);
    free(seq);
    Graph *t_graph = create_graph(verticies);
    for(int i = 0; i < verticies-1; i++){
        add_edge(t_graph, edges[i*2], edges[i*2+1], 0);
        add_edge(t_graph, edges[i*2+1], edges[i*2], 0);
    }
    free(edges);
    U32f *parent = (U32f *)malloc(verticies * sizeof(U32f));
    U32f *visited = (U32f *)calloc(verticies, sizeof(U32f));
    U32f index = start_vertex;
    get_dir_tree(t_graph, parent, start_vertex, visited, &index);
    free(visited);
    free_graph(t_graph); 
    for (U32f i = 0; i < verticies; i++) {
        if (i != start_vertex) {
            add_edge(graph, parent[i], i, 0);
        }
    }
    free(parent);
    U32f max_edges = verticies * (verticies - 1);
    U32f numb = max_edges - verticies + 1;
    U32f *candidates_u = (U32f *)malloc(sizeof(U32f) * numb);
    U32f *candidates_v = (U32f *)malloc(sizeof(U32f) * numb);
    I32f ind = 0;
    for (U32f i = 0; i < verticies; i++) {
        for (U32f j = 0; j < verticies; j++) {
            if (i == j || check_edge(graph, i, j)) {
                continue;
            }
            candidates_u[ind] = i;
            candidates_v[ind] = j;
            ind++;
        }
    }
    shuffle(candidates_u, candidates_v, ind);
    density = density - (verticies - 1);
    if (ind > 0 && density > ind) {
        density = ind;
    } else if (density < 0) {
        density = 0;
    }
    for (U32f i = 0; i < density; i++) {
        add_edge(graph, candidates_u[i], candidates_v[i], 0);
    }
    free(candidates_u);
    free(candidates_v);
}

/* create_inc_undir_matrix
 * Tworzy macierz incydencji dla grafu nieskierowanego.
 * Zwraca wskaźnik na zaalokowaną macierz.
 */
U32f *create_inc_undir_matrix(Graph* graph, U32f density){
    U32f *inc_matrix = (U32f *)calloc(graph->num_v * density, sizeof(U32f));
    if (inc_matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for incidence matrix.\n");
        return NULL;
    }
    U32f ind = 0;
    for (U32f i = 0; i < graph->num_v; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            U32f j = temp->vertex;
            if (i < j) {
                inc_matrix[j * density + ind] = temp->weight;
                inc_matrix[i * density + ind] = temp->weight;
                ind++;
            }
            temp = temp->next;
        }
    }
    return inc_matrix;
}

/* create_inc_dir_matrix
 * Tworzy macierz incydencji dla grafu skierowanego.
 * Zwraca wskaźnik na zaalokowaną macierz.
 */
I32f *create_inc_dir_matrix(Graph* graph, U32f density) {
    I32f *inc_matrix = (I32f *)calloc(graph->num_v * density, sizeof(I32f));
    if (inc_matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for incidence matrix.\n");
        return NULL;
    }
    U32f ind = 0;
    for (U32f i = 0; i < graph->num_v; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            U32f j = temp->vertex;
            if (i != j) {
                inc_matrix[j * density + ind] = temp->weight;
                inc_matrix[i * density + ind] = -temp->weight;
                ind++;
            }
            temp = temp->next;
        }
    }
    return inc_matrix;
}

/* create_node
 * Tworzy nowy węzeł listy sąsiedztwa.
 */
Node* create_node(U32f vertex, U32f weight) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node) {
        node->vertex = vertex;
        node->weight = weight;
        node->next = NULL;
    }
    return node;
}

/* create_graph
 * Tworzy nowy graf o zadanej liczbie wierzchołków.
 */
Graph* create_graph(U32f vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph) {
        graph->num_v = vertices;
        graph->adjLists = (Node**)calloc(vertices, sizeof(Node*));
        if (!graph->adjLists) {
            free(graph);
            return NULL;
        }
    }
    return graph;
}

/* add_edge
 * Dodaje krawędź do grafu.
 */
void add_edge(Graph* graph, U32f src, U32f dst, U32f weight) {
    if (!graph || src >= graph->num_v || dst >= graph->num_v) {
        return;
    }
    Node* node = create_node(dst, weight);
    if (node) {
        node->next = graph->adjLists[src];
        graph->adjLists[src] = node;
    }
}

/* get_edge
 * Zwraca wskaźnik do krawędzi (src, dst) lub NULL jeśli nie istnieje.
 */
Node *get_edge(Graph* graph, U32f src, U32f dst) {
    if (!graph || src >= graph->num_v || dst >= graph->num_v) {
        return NULL;
    }
    Node* temp = graph->adjLists[src];
    while (temp) {
        if (temp->vertex == dst) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/* set_rand_weights_undir
 * Ustawia losowe wagi dla wszystkich krawędzi grafu nieskierowanego.
 */
void set_rand_weights_undir(Graph* graph, U32f min_weight, U32f max_weight){
    if (!graph) {
        return;
    }
    for (U32f i = 0; i < graph->num_v; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            if (temp->vertex > i) {
                temp->weight = min_weight + (rand() % (max_weight - min_weight + 1));
                Node* reverse_edge = get_edge(graph, temp->vertex, i);
                if (reverse_edge) {
                    reverse_edge->weight = temp->weight;
                }
            }
            temp = temp->next;
        }
    }
}

/* set_rand_weights_dir
 * Ustawia losowe wagi dla wszystkich krawędzi grafu skierowanego.
 */
void set_rand_weights_dir(Graph* graph, U32f min_weight, U32f max_weight) {
    if (!graph) {
        return;
    }
    for (U32f i = 0; i < graph->num_v; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            temp->weight = min_weight + (rand() % (max_weight - min_weight + 1));
            temp = temp->next;
        }
    }
}

/* check_edge
 * Sprawdza czy istnieje krawędź (src, dst) w grafie.
 * Zwraca 1 jeśli istnieje, 0 w przeciwnym wypadku.
 */
int check_edge(Graph* graph, U32f src, U32f dst) {
    if (!graph || src >= graph->num_v || dst >= graph->num_v) {
        return 0;
    }
    Node* temp = graph->adjLists[src];
    while (temp) {
        if (temp->vertex == dst) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

/* free_graph
 * Zwalnia pamięć zaalokowaną przez graf.
 */
void free_graph(Graph* graph) {
    if (!graph) {
        return;
    }
    for (U32f i = 0; i < graph->num_v; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->adjLists);
    free(graph);
}
