#include "graph.h"
#include "mst.h"
#include "sp.h"
#include "io_graph.h"
#include "configuration.h"

/* print_inc_undir_matrix
 * Wypisuje macierz incydencji dla grafu nieskierowanego.
 */
void print_inc_undir_matrix(U32f *inc_matrix, U32f vertices, U32f density){
    for (U32f i = 0; i < vertices; i++){
        for (U32f j = 0; j < density; j++){
            printf("%5" PRIuFAST32 "", inc_matrix[i * density + j]);
        }
        printf("\n");
    }
}

/* print_inc_dir_matrix
 * Wypisuje macierz incydencji dla grafu skierowanego.
 */
void print_inc_dir_matrix(I32f *inc_matrix, U32f vertices, U32f density){
    for (U32f i = 0; i < vertices; i++){
        for (U32f j = 0; j < density; j++){
            printf("%5" PRIdFAST32"", inc_matrix[i * density + j]);
        }
        printf("\n");
    }
}

/* print_graph
 * Wypisuje reprezentację grafu (listę sąsiedztwa).
 */
void print_graph(Graph* graph) {
    if (!graph) {
        return;
    }
    for (U32f v = 0; v < graph->num_v; v++) {
        Node* temp = graph->adjLists[v];
        printf("%" PRIuFAST32 ":" , v);
        while (temp) {
            printf("-> (%" PRIuFAST32 ",%5" PRIuFAST32 ") ", temp->vertex, temp->weight);
            temp = temp->next;
        }
        printf("\n");
    }
}

/* print_edges
 * Wypisuje wszystkie krawędzie grafu.
 */
void print_edges(Graph *graph){
    printf("\n");
    for (U32f i = 0; i < graph->num_v; i++){
        Node* temp = graph->adjLists[i];
        while (temp) {
            printf("%" PRIuFAST32 " %" PRIuFAST32 "\n",i, temp->vertex);
            temp = temp->next;
        }
    }
}

/* print_res_prim
 * Wypisuje wynik działania algorytmu Prima.
 */
void print_res_prim(Res_prim *mst, U32f numVertices, U32f start_vertex) {
    printf("start Vertex: %" PRIuFAST32 "\n", start_vertex);
    for (U32f i = 0; i < numVertices; i++){
        if (i == start_vertex) {
            continue;
        }
        printf("Vertex: %" PRIuFAST32 ", Parent: %" PRIuFAST32 ", Weight: %" PRIuFAST32 "\n", 
               i, mst->parent_weight[i].parent, mst->parent_weight[i].weight);
    }
    printf("suma wag: %" PRIuFAST32 "\n", mst->sum_weights);
}

/* print_res_kruskal
 * Wypisuje wynik działania algorytmu Kruskala.
 */
void print_res_kruskal(Res_kruskal *res, U32f num_edges){
    for (U32f i = 0; i < num_edges; i++){
        printf("Edge: %" PRIuFAST32 " - %" PRIuFAST32 ", Weight: %" PRIuFAST32 "\n", 
               res->edges[i].src, res->edges[i].dst, res->edges[i].weight);
    }
    printf("suma wag: %" PRIuFAST32 "\n", res->sum_weights);
}

/* print_res_sp
 * Wypisuje wynik działania algorytmu najkrótszej ścieżki.
 */
void print_res_sp(Res_sp *res, U32f numVertices, U32f start_vertex) {
    printf("Start Vertex: %" PRIuFAST32 "\n", start_vertex);
    U32 weight = 0;
    for (U32f i = 0; i < numVertices; i++){
        if (i == start_vertex) {
            continue;
        }
        printf("Vertex: %" PRIuFAST32 ", Parent: %" PRIuFAST32 ", Distance: %" PRIuFAST32 "\n", 
               i, res->parents[i], res->distances[i]);
    }
}

/* load_dir_graph_from_file
 * Wczytuje graf skierowany z pliku do struktury Graph.
 */
void load_dir_graph_from_file(const char *file_name, File_config *cfg_file, Config *cfg) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name);
    	exit(1);
    }
    
    U32f num_v, num_e;
    fscanf(file, "%" SCNuFAST32 " %" SCNuFAST32, &num_v, &num_e);
    if (num_v == 0 || num_e == 0) {
        fclose(file);
	exit(1);
    }
    cfg_file->num_v = num_v;
    cfg->density = num_e;
    cfg->graph = create_graph(num_v);
    if (cfg->graph == NULL) {
        fclose(file);
        exit(1);
    }
    
    U32f src, dst, weight;
    while (fscanf(file, "%" SCNuFAST32 " %" SCNuFAST32 " %" SCNuFAST32, &src, &dst, &weight) == 3) {
        add_edge(cfg->graph, src, dst, weight);
    }
    
    fclose(file);
    return;
}

/* load_undir_graph_from_file
 * Wczytuje graf nieskierowany z pliku do struktury Graph.
 */
void load_undir_graph_from_file(const char *file_name, File_config *cfg_file, Config* cfg) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name);
    	exit(1);
    }
    
    U32f num_v;
    U32f num_e;
    if (fscanf(file, "%" SCNuFAST32 " %" SCNuFAST32, &num_v, &num_e) != 2) {
        fclose(file);
        exit(1);
    }
    if (num_v == 0 || num_e == 0) {
        fclose(file);
        exit(1);
    }
    cfg_file->num_v = num_v;
    cfg->density = num_e;
    cfg->graph = create_graph(num_v);
    if (cfg->graph == NULL) {
        fclose(file);
        exit(1);
    }
    
    U32f src, dst, weight;
    while (fscanf(file, "%" SCNuFAST32 " %" SCNuFAST32 " %" SCNuFAST32, &src, &dst, &weight) == 3) {
        add_edge(cfg->graph, src, dst, weight);
        add_edge(cfg->graph, dst, src, weight);
    }
    
    fclose(file);
    return;
}

/* load_graph_from_file
 * Wczytuje graf z pliku w zależności od typu algorytmu (skierowany/nieskierowany).
 */
void load_graph_from_file(const char *file_name, File_config *cfg_file, Config* cfg) {
    if (cfg_file->alg_type == DIJKSTRA_LIST || 
        cfg_file->alg_type == DIJKSTRA_MATRIX || 
        cfg_file->alg_type == BELMAN_FORD_LIST || 
        cfg_file->alg_type == BELMAN_FORD_MATRIX_EDGE_LIST || 
        cfg_file->alg_type == BELMAN_FORD_MATRIX_NO_EDGE_LIST) {
        load_dir_graph_from_file(file_name, cfg_file, cfg);
        return;

    } 
    else if (cfg_file->alg_type == PRIM_LIST || cfg_file->alg_type == PRIM_MATRIX || 
               cfg_file->alg_type == KRUSKAL_LIST || cfg_file->alg_type == KRUSKAL_MATRIX) {
        load_undir_graph_from_file(file_name, cfg_file, cfg);
        return;
    }
    
}

/* save_edges
 * Zapisuje krawędzie grafu do pliku.
 */
int save_edges(Graph *graph, char* base_file_name, char *folder){
    char file_name[256];
    int file_index = 1;
    FILE *file;
    do {
        snprintf(file_name, sizeof(file_name), "%s/data_edges/%s/%d.txt", folder, base_file_name, file_index);
        file = fopen(file_name, "r");
        if (file != NULL) {
            fclose(file);
            file_index++;
        }
    } while (file != NULL);

    file = fopen(file_name, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name);
        return -1;
    }
    
    for (U32f i = 0; i < graph->num_v; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            fprintf(file, "%" PRIuFAST32 " %" PRIuFAST32 " %" PRIuFAST32 "\n", i, temp->vertex, temp->weight);
            temp = temp->next;
        }
    }
    
    fclose(file);
    return file_index;
}

/* save_weight
 * Zapisuje wagę grafu do pliku.
 */
void save_weight(Graph *graph, char* base_file_name, char *folder, U32f weight, int file_index){
    char file_name_weights[256];
    FILE *file_weights;
    snprintf(file_name_weights, sizeof(file_name_weights), "../%s/data_weight/%s%d_weight.txt", folder, base_file_name, file_index);
    file_weights = fopen(file_name_weights, "w");
    if (file_weights == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name_weights);
        return;
    }
    
    fprintf(file_weights, "%" PRIuFAST32 "\n", weight);
    
    fclose(file_weights);
}

/* save_sp
 * Zapisuje wyniki algorytmu najkrótszej ścieżki do pliku.
 */
void save_sp(Res_sp *res, char* base_file_name, char *folder, U32f num_vert, U32f start_vertex, int file_index){
    char file_name[256];
    FILE *file;
    snprintf(file_name, sizeof(file_name), "../%s/data_sp/%s%d.txt", folder, base_file_name, file_index);
    file = fopen(file_name, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name);
        return;
    }
    
    for (U32f i = 0; i < num_vert; i++) {
        if (i == start_vertex)
            continue;
        fprintf(file, "%" PRIuFAST32 " %" PRIuFAST32 " %" PRIuFAST32 "\n", i,res->parents[i] ,res->distances[i]);
    }
    
    fclose(file);
}

/* print_edges_to_file
 * Wypisuje krawędzie grafu do pliku.
 */
void print_edges_to_file(Graph *graph, char* base_file_name, char *folder, U32f weight){
    char file_name[256];
    char file_name_weights[256];
    int file_index = 1;
    FILE *file;
    FILE *file_weights;
    do {
        snprintf(file_name, sizeof(file_name), "../%s/data/%s%d.txt", folder, base_file_name, file_index);
        file = fopen(file_name, "r");
        if (file != NULL) {
            fclose(file);
            file_index++;
        }
    } while (file != NULL);
    do {
        snprintf(file_name_weights, sizeof(file_name_weights), "../%s/data_weight/%s%d_weight.txt", folder, base_file_name, file_index);
        file_weights = fopen(file_name_weights, "r");
        if (file_weights != NULL) {
            fclose(file_weights);
            file_index++;
        }
    } while (file_weights != NULL);

    file = fopen(file_name, "w");
    file_weights = fopen(file_name_weights, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name);
        return;
    }
    if (file_weights == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name_weights);
        return;
    }
    
    fprintf(file_weights, "%" PRIuFAST32 "\n", weight);
    for (U32f i = 0; i < graph->num_v; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            fprintf(file, "%" PRIuFAST32 " %" PRIuFAST32 " %" PRIuFAST32 "\n", i, temp->vertex, temp->weight);
            temp = temp->next;
        }
    }
    fclose(file_weights);
    fclose(file);
}

/* print_edges_to_file_bitmask
 * Wypisuje krawędzie grafu do pliku na podstawie bitmaski. (dziala tylko dla grafu o 8 wierzchołkach). uzyta byla do sprawdzenia losowsci grafu.
 */
void print_edges_to_file_bitmask(Graph *graph, char* base_file_name, U64 bitmask){
    char file_name[256];
    int file_index = 1;
    FILE *file;
    do {
        snprintf(file_name, sizeof(file_name), "%s%d.txt", base_file_name, file_index);
        file = fopen(file_name, "r");
        if (file != NULL) {
            fclose(file);
            file_index++;
        }
    } while (file != NULL);

    file = fopen(file_name, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name);
        return;
    }
    fprintf(file, "%" PRIuFAST64 "\n", bitmask);
    for (U32f i = 0; i < graph->num_v; i++) {
        Node* temp = graph->adjLists[i];
        while (temp) {
            fprintf(file, "%" PRIuFAST32 " %" PRIuFAST32 "\n", i, temp->vertex);
            temp = temp->next;
        }
    }

    fclose(file);
}
