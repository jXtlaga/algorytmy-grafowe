#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "sp.h"
#include "mst.h"

// Enum typów algorytmów
typedef enum Alg_type {
    DIJKSTRA_LIST,
    DIJKSTRA_MATRIX,
    BELMAN_FORD_LIST,
    BELMAN_FORD_MATRIX_EDGE_LIST,
    BELMAN_FORD_MATRIX_NO_EDGE_LIST,
    PRIM_LIST,
    PRIM_MATRIX,
    KRUSKAL_LIST,
    KRUSKAL_MATRIX,
    ALG_TYPE_COUNT
} Alg_type;

// Struktura konfiguracji algorytmu
typedef struct Config {
    Graph *graph;
    I32f* inc_matrix_dir;
    U32f* inc_matrix_undir;
    Res_sp *res_sp;
    Res_prim *res_prim;
    Res_kruskal *res_kruskal;
    U32f start_vertex;
    U32f num_v;
    U32f density;
    Alg_type alg_type;
} Config;

// Struktura konfiguracji wczytywanej z pliku
typedef struct File_config {
    Alg_type alg_type;
    U32f num_v;
    double density;
    U32f start_vertex;
    char out_matrix;
    char out_list;
    char *file_name;
} File_config;

// Enum sekcji konfiguracji
typedef enum Config_type {
    CONF_ALG_TYPE, CONF_NUM_V, CONF_DENSITY,
    CONF_START_VERTEX, CONF_OUT_MATRIX, CONF_OUT_LIST, CONF_FILE_IN, CONF_NONE
} Config_type;

// Funkcje konfiguracyjne
void run_alg(Config *cfg);
void create_config_random_weights(Config *cfg, U32f num_v, double density, enum Alg_type alg_type, U32f start_vertex);
void create_config_from_graph(Config *cfg, enum Alg_type alg_type, U32f num_v, U32f density);
void free_unused_config(Config *cfg, Alg_type alg_type);
void free_config(Config *cfg);
void free_config_file(File_config *cfg);
void read_config_file(const char* filename, File_config* config);

typedef int (*ParseFunc)(const char*, File_config*);
typedef struct {
    Config_type section;
    const char* sectionName;
    ParseFunc parseFunc;
} ConfigParserEntry;

extern char *alg_names[];

#endif // CONFIGURATION_H