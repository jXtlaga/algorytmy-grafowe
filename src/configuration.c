#include "configuration.h"
#define MAX_WEIGHT 4096

char *alg_names[] = {
    "dijkstra_list",
    "dijkstra_matrix",
    "belman_ford_list",
    "belman_ford_matrix_edge_list",
    "belman_ford_matrix_no_edge_list",
    "prim_list",
    "prim_matrix",
    "kruskal_list",
    "kruskal_matrix"
};


/* run_alg
 * uruchamia wybrany algorytm na podstawie typu algorytmu w strukturze Config
 */
void run_alg(Config *cfg) {
    switch (cfg->alg_type) {
        case DIJKSTRA_LIST:
            cfg->res_sp = dijkstra_list(cfg->graph, cfg->start_vertex);
            break;
        case DIJKSTRA_MATRIX:
            cfg->res_sp = dijkstra_matrix(cfg->inc_matrix_dir, cfg->num_v, cfg->density, cfg->start_vertex);
            break;
        case BELMAN_FORD_LIST:
            cfg->res_sp = belman_ford_list(cfg->graph, cfg->start_vertex);
            break;
        case BELMAN_FORD_MATRIX_EDGE_LIST:
            cfg->res_sp = belman_ford_matrix_edge_list(cfg->inc_matrix_dir, cfg->num_v, cfg->density, cfg->start_vertex);
            break;
        case BELMAN_FORD_MATRIX_NO_EDGE_LIST:
            cfg->res_sp = belman_ford_matrix_no_edge_list(cfg->inc_matrix_dir, cfg->num_v, cfg->density, cfg->start_vertex);
            break;
        case PRIM_LIST:
            cfg->res_prim = prim_list(cfg->graph, cfg->start_vertex);
            break;
        case PRIM_MATRIX:
            cfg->res_prim = prim_matrix(cfg->inc_matrix_undir, cfg->num_v, cfg->density, cfg->start_vertex);
            break;
        case KRUSKAL_LIST:
            cfg->res_kruskal = kruskal_list(cfg->graph, cfg->density);
            break;
        case KRUSKAL_MATRIX:
            cfg->res_kruskal = kruskal_matrix(cfg->inc_matrix_undir, cfg->num_v, cfg->density);
            break;
        case ALG_TYPE_COUNT:
            break;
    }
}

/* density_dir
 * oblicza gęstość grafu skierowanego na podstawie liczby wierzchołków i współczynnika gęstości
 */
U32f density_dir(double density, U32f num_v) {
    return (U32f)(num_v * (num_v - 1) * density);
}

/* density_undir
 * oblicza gęstość grafu nieskierowanego na podstawie liczby wierzchołków i współczynnika gęstości
 */
U32f density_undir(double density, U32f num_v) {
    return (U32f)(num_v * (num_v - 1) / 2 * density);
}

/* create_config
 * tworzy i konfiguruje strukturę Config na podstawie liczby wierzchołków, gęstości i typu algorytmu
 */
void create_config_random_weights(Config *cfg, U32f num_v, double density, enum Alg_type alg_type, U32f start_vertex) {
   if (start_vertex >= num_v) {
        fprintf(stderr, "startowy wierzcholek musi byc mniejszy od liczby wierzcholkow\n");
        exit(EXIT_FAILURE);
    }
    if (density < 0 || density > 1) {
        fprintf(stderr, "gęstość musi być w przedziale [0, 1]\n");
        exit(EXIT_FAILURE);
    }
    if (alg_type < 0 || alg_type >= ALG_TYPE_COUNT) {
        fprintf(stderr, "nieznany typ algorytmu\n");
        exit(EXIT_FAILURE);
    }
    cfg->graph = create_graph(num_v);
    cfg->inc_matrix_dir = NULL;
    cfg->inc_matrix_undir = NULL;
    cfg->num_v = num_v;
    cfg->start_vertex = start_vertex;
    cfg->res_sp = NULL;
    cfg->res_prim = NULL;
    cfg->res_kruskal = NULL;
    cfg->alg_type = alg_type;

    if (alg_type == DIJKSTRA_LIST || alg_type == BELMAN_FORD_LIST) {
        cfg->density = density_dir(density, num_v);
        create_rand_dir_graph(cfg->graph, cfg->density, cfg->start_vertex);
        set_rand_weights_dir(cfg->graph, 1, MAX_WEIGHT);
    }
    else if (alg_type == DIJKSTRA_MATRIX || alg_type == BELMAN_FORD_MATRIX_EDGE_LIST || alg_type == BELMAN_FORD_MATRIX_NO_EDGE_LIST) {
        
        cfg->density = density_dir(density, num_v);
        create_rand_dir_graph(cfg->graph, cfg->density, cfg->start_vertex);
        set_rand_weights_dir(cfg->graph, 1, MAX_WEIGHT);
    
        cfg->inc_matrix_dir = create_inc_dir_matrix(cfg->graph, cfg->density);        
    }
    else if (alg_type == PRIM_LIST || alg_type == KRUSKAL_LIST) {
          
        cfg->density = density_undir(density, num_v);
        create_rand_undir_graph(cfg->graph, cfg->density);
        
        set_rand_weights_undir(cfg->graph, 1, MAX_WEIGHT);
    }
    else if (alg_type == PRIM_MATRIX || alg_type == KRUSKAL_MATRIX) {
        cfg->density = density_undir(density, num_v);
        create_rand_undir_graph(cfg->graph, cfg->density);
        set_rand_weights_undir(cfg->graph, 1, MAX_WEIGHT);
        cfg->inc_matrix_undir = create_inc_undir_matrix(cfg->graph, cfg->density); 
    }
}
void create_config_from_graph(Config *cfg, enum Alg_type alg_type, U32f num_v, U32f density) {
   
    if (alg_type < 0 || alg_type >= ALG_TYPE_COUNT) {
        fprintf(stderr, "nieznany typ algorytmu\n");
        exit(EXIT_FAILURE);
    }
    cfg->inc_matrix_dir = NULL;
    cfg->inc_matrix_undir = NULL;
    cfg->num_v = num_v;
    cfg->density = density;
    cfg->start_vertex = rand() % num_v;
    cfg->res_sp = NULL;
    cfg->res_prim = NULL;
    cfg->res_kruskal = NULL;
    cfg->alg_type = alg_type;

   
    if (alg_type == DIJKSTRA_MATRIX || alg_type == BELMAN_FORD_MATRIX_EDGE_LIST || alg_type == BELMAN_FORD_MATRIX_NO_EDGE_LIST) {
        cfg->inc_matrix_dir = create_inc_dir_matrix(cfg->graph, cfg->density);        
    }
    else if (alg_type == PRIM_MATRIX || alg_type == KRUSKAL_MATRIX) {
        cfg->inc_matrix_undir = create_inc_undir_matrix(cfg->graph, cfg->density); 
    }
}
/* free_unused_config
 * zwalnia nieużywaną pamięć w strukturze Config w zależności od typu algorytmu
 */
void free_unused_config(Config *cfg, Alg_type alg_type) {
    if (alg_type == DIJKSTRA_MATRIX || alg_type == BELMAN_FORD_MATRIX_EDGE_LIST || alg_type == BELMAN_FORD_MATRIX_NO_EDGE_LIST || alg_type == PRIM_MATRIX || alg_type == KRUSKAL_MATRIX) {
        free_graph(cfg->graph);
        cfg->graph = NULL;
    }
}

/* free_config
 * zwalnia całą pamięć zajętą przez strukturę Config
 */
void free_config(Config *cfg) {
    if (cfg->graph) {
        free_graph(cfg->graph);
    }
    if (cfg->inc_matrix_dir) {
        free(cfg->inc_matrix_dir);
    }
    if (cfg->inc_matrix_undir) {
        free(cfg->inc_matrix_undir);
    }
    if (cfg->res_sp) {
        free_res_dijkstra(cfg->res_sp);
    }
    if (cfg->res_prim) {
        free_res_prim(cfg->res_prim);
    }
    if (cfg->res_kruskal) {
        free_res_kruskal(cfg->res_kruskal);
    }
}
/* free_config_file
* zwalnia pamięć zajętą przez strukturę File_config
 */
void free_config_file(File_config *cfg) {
    if (cfg->file_name) {
        free(cfg->file_name);
    }
    free(cfg);
}


/* Enum_entry
 * struktura pomocnicza: mapa stringów na wartości enum
 */
typedef struct {
    const char* name;
    int value;
} Enum_entry;

Enum_entry alg_type_entries[] = {
    {"DIJKSTRA_LIST", DIJKSTRA_LIST},
    {"DIJKSTRA_MATRIX", DIJKSTRA_MATRIX},
    {"BELMAN_FORD_LIST", BELMAN_FORD_LIST},
    {"BELMAN_FORD_MATRIX_EDGE_LIST", BELMAN_FORD_MATRIX_EDGE_LIST},
    {"BELMAN_FORD_MATRIX_NO_EDGE_LIST", BELMAN_FORD_MATRIX_NO_EDGE_LIST},
    {"PRIM_LIST", PRIM_LIST},
    {"PRIM_MATRIX", PRIM_MATRIX},
    {"KRUSKAL_LIST", KRUSKAL_LIST},
    {"KRUSKAL_MATRIX", KRUSKAL_MATRIX}
};

/* map_string_to_enum
 * zamienia tekst (np. z pliku konfiguracyjnego) na wartość enuma
 */
int map_string_to_enum(const char* line, const Enum_entry* map, int invalidValue) {
    for (int i = 0; map[i].name != NULL; i++) {
        if (strcmp(line, map[i].name) == 0) {
            return map[i].value;
        }
    }
    return invalidValue;
}

/* parse_alg_type
 * parsuje typ algorytmu z ciągu znaków i zapisuje go w strukturze File_config
 */
int parse_alg_type(const char* line, File_config* cfg) {
    int val = map_string_to_enum(line, alg_type_entries, ALG_TYPE_COUNT);
    if (val == ALG_TYPE_COUNT) {
        fprintf(stderr, "Nieznany typ algorytmu: %s\n", line);
        return 0;
    }
    cfg->alg_type = (Alg_type)val;
    return 1;
}

/* parse_num_v
 * parsuje liczbę wierzchołków z ciągu znaków i zapisuje ją w strukturze File_config
 */
int parse_num_v(const char* line, File_config* cfg) {
    int num_v = atoi(line);
    if (num_v <= 0) {
        fprintf(stderr, "Nieprawidłowa liczba wierzchołków: %s\n", line);
        return 0;
    }
    cfg->num_v = (U32f)num_v;
    return 1;
}

/* parse_density
 * parsuje gęstość grafu z ciągu znaków i zapisuje ją w strukturze File_config
 */
int parse_density(const char* line, File_config* cfg) {
    double density = atof(line);
    if (density <= 0.0 || density > 1.0) {
        fprintf(stderr, "Nieprawidłowa gęstość: %s\n", line);
        return 0;
    }
    cfg->density = density;
    return 1;
}

/* parse_start_vertex
 * parsuje wierzchołek startowy z ciągu znaków i zapisuje go w strukturze File_config
 */
int parse_start_vertex(const char* line, File_config* cfg) {
    int start_vertex = atoi(line);
    if (start_vertex < 0) {
        fprintf(stderr, "Nieprawidłowy wierzchołek startowy: %s\n", line);
        return 0;
    }
    cfg->start_vertex = (U32f)start_vertex;
    return 1;
}


/* parse_out_matrix
 * parsuje wartość logiczną dla out_matrix z ciągu znaków i zapisuje ją w strukturze File_config
 */
int parse_out_matrix(const char* line, File_config* cfg) {
    if (strcmp(line, "true") == 0) {
        cfg->out_matrix = 1;
    } else if (strcmp(line, "false") == 0) {
        cfg->out_matrix = 0;
    } else {
        fprintf(stderr, "Nieprawidłowa wartość dla out_matrix: %s\n", line);
        return 0;
    }
    return 1;
}

/* parse_out_list
 * parsuje wartość logiczną dla out_list z ciągu znaków i zapisuje ją w strukturze File_config
 */
int parse_out_list(const char* line, File_config* cfg) {
    if (strcmp(line, "true") == 0) {
        cfg->out_list = 1;
    } else if (strcmp(line, "false") == 0) {
        cfg->out_list = 0;
    } else {
        fprintf(stderr, "Nieprawidłowa wartość dla out_list: %s\n", line);
        return 0;
    }
    return 1;
}

/* parse_file_name
 * parsuje nazwę pliku z ciągu znaków i zapisuje ją w strukturze File_config
 */
int parse_file_name(const char* line, File_config* cfg) {
    if (line == NULL || strlen(line) == 0) {
        fprintf(stderr, "Nieprawidłowa nazwa pliku wejściowego: %s\n", line);
        return 0;
    }
    cfg->file_name = strdup(line);
    return 1;
}

typedef int (*ParseFunc)(const char*, File_config*);



/* configParsers
 * tablica parserów przypisanych do sekcji pliku konfiguracyjnego
 */
ConfigParserEntry configParsers[] = {
    { CONF_ALG_TYPE,     ".alg_type",     parse_alg_type },
    { CONF_NUM_V,        ".num_v",        parse_num_v },
    { CONF_DENSITY,      ".density",      parse_density },
    { CONF_START_VERTEX, ".start_vertex", parse_start_vertex },
    { CONF_OUT_MATRIX,   ".out_matrix",   parse_out_matrix },
    { CONF_OUT_LIST,     ".out_list",     parse_out_list },
    { CONF_FILE_IN,      ".file_name",    parse_file_name },
    { CONF_NONE,         NULL,            NULL }
};

/* get_config_section
 * zwraca typ sekcji konfiguracyjnej na podstawie nazwy sekcji
 */
Config_type get_config_section(const char* buffor) {
    Config_type currentSection = CONF_NONE;
    for (int i = 0; configParsers[i].sectionName != NULL; i++) {
        if (strcmp(buffor, configParsers[i].sectionName) == 0) {
            return configParsers[i].section;
        }
    }
    return CONF_NONE;
}

/* read_config
 * czyta plik konfiguracyjny i uzupełnia strukturę File_config
 */
void read_config_file(const char* filename, File_config* config) {
    FILE* file = fopen(filename, "r"); 
    if (!file) {
        fprintf(stderr, "Nie można otworzyć pliku konfiguracyjnego: %s\n", filename);
        exit(1);
    }
    char buffor[512];
    Config_type actConf = CONF_NONE;
    while (fscanf(file, "%511s", buffor) == 1) {
        if (buffor[0] == '\0') continue;
        if (buffor[0] == '.' || buffor[0] == '#') {
            actConf = get_config_section(buffor);
        } else if (actConf != CONF_NONE) {
            for (int i = 0; configParsers[i].sectionName != NULL; i++) {
                if (configParsers[i].section == actConf) {
                    configParsers[i].parseFunc(buffor, config);
                    break;
                }
            }
        }
    }
    fclose(file);
}
