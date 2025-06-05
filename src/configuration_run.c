#include "configuration.h"
#include "types.h"
#include <inttypes.h>
#include "io_graph.h"

#define BREAK_LINE "--------------------------------\n"
#define SECTION_LINE "================================\n"

/* print_config_file
 * wyświetla zawartość struktury File_config
 */
void print_config_file(File_config *cfg_file) {
    printf("====CONFIG====\n");
    printf("Algorytm: %s\n", alg_names[cfg_file->alg_type]);
    printf("Liczba wierzchołków: %"PRIuFAST32"\n", cfg_file->num_v);
    printf("Gęstość: %lf\n", cfg_file->density);
    printf("Wierzchołek startowy: %" PRIuFAST32 "\n", cfg_file->start_vertex);
    printf("Wyjście macierzy: %d\n", cfg_file->out_matrix);
    printf("Wyjście listy sąsiedztwa: %d\n", cfg_file->out_list);
    if (cfg_file->file_name) {
        printf("Nazwa pliku wejściowego: %s\n", cfg_file->file_name);
    }
    printf(SECTION_LINE);
}

/* print_config_results
 * wyświetla wyniki algorytmu na podstawie typu algorytmu w strukturze Config
 */
void print_config_results(const Config *cfg) {
    if (cfg->alg_type == DIJKSTRA_LIST || 
        cfg->alg_type == DIJKSTRA_MATRIX || 
        cfg->alg_type == BELMAN_FORD_LIST || 
        cfg->alg_type == BELMAN_FORD_MATRIX_EDGE_LIST || 
        cfg->alg_type == BELMAN_FORD_MATRIX_NO_EDGE_LIST) {
        print_res_sp(cfg->res_sp, cfg->num_v, cfg->start_vertex);
    } else if (cfg->alg_type == PRIM_LIST || cfg->alg_type == PRIM_MATRIX) {
        print_res_prim(cfg->res_prim, cfg->num_v, cfg->start_vertex);
    } else if (cfg->alg_type == KRUSKAL_LIST || cfg->alg_type == KRUSKAL_MATRIX) {
        print_res_kruskal(cfg->res_kruskal, cfg->num_v - 1);
    }
}

/* print_config_graph_representation
 * wyświetla reprezentację grafu (macierz incydencji lub lista sąsiedztwa) na podstawie konfiguracji
 */
void print_config_graph_representation(const Config *cfg, const File_config *cfg_file) {
    if (cfg_file->out_matrix && cfg->inc_matrix_dir) {
        print_inc_dir_matrix(cfg->inc_matrix_dir, cfg->num_v, cfg->density);
        printf(BREAK_LINE);
    }
    else if (cfg_file->out_matrix && cfg->inc_matrix_undir) {
        print_inc_undir_matrix(cfg->inc_matrix_undir, cfg->num_v, cfg->density);
        printf(BREAK_LINE);
    }
    if (cfg_file->out_list && cfg->graph) {
        print_graph(cfg->graph);
        printf(BREAK_LINE);
    }
}

/* run_config_file_var
 * generuje losowy graf i uruchamia algorytm na podstawie konfiguracji z pliku
 */
void run_config_file_var(File_config *cfg_file, Config *cfg) {
    printf("1. Generowanie grafu losowego i uruchomienie algorytmu\n");
    printf(BREAK_LINE);
    create_config_random_weights(cfg, cfg_file->num_v, cfg_file->density, cfg_file->alg_type, cfg_file->start_vertex);
    print_config_graph_representation(cfg, cfg_file);
    free_unused_config(cfg, cfg->alg_type);
    run_alg(cfg);
    printf("Wyniki dla algorytmu %s:\n", alg_names[cfg_file->alg_type]);
    print_config_results(cfg); 
    printf(SECTION_LINE);
}

/* run_config_file_load
 * wczytuje graf z pliku i uruchamia algorytm na podstawie konfiguracji z pliku
 */
void run_config_file_load(File_config *cfg_file, Config *cfg) {
    printf("2. Wczytanie grafu z pliku i uruchomienie algorytmu\n");
    printf(BREAK_LINE);
    if (cfg_file->file_name == NULL || strlen(cfg_file->file_name) == 0) {
        fprintf(stderr, "Nie podano nazwy pliku wejściowego.\n");
        return;
    }
    load_graph_from_file(cfg_file->file_name, cfg_file, cfg);
    if (cfg->graph == NULL) {
        printf("Błąd wczytywania grafu z pliku %s\n", cfg_file->file_name);
        return;
    }
    U32f rand_start_vertex = rand() % cfg_file->num_v;
    create_config_from_graph(cfg, cfg_file->alg_type, cfg_file->num_v, cfg->density);
    print_config_graph_representation(cfg, cfg_file);
    free_unused_config(cfg, cfg->alg_type);
    run_alg(cfg);
    printf("Wyniki dla algorytmu %s:\n", alg_names[cfg_file->alg_type]);
    print_config_results(cfg);
    printf(SECTION_LINE);
}