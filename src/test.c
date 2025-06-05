#define _POSIX_C_SOURCE 199309L
#include "configuration.h"
#include "graph.h"
#include "sp.h"
#include "io_graph.h"
#include <inttypes.h>
#include <time.h>
#include "types.h"
#include "mst.h"
/*funkcje podgladowe, nie sa aktualnie wazne. byly uzywane do testowania i generowania wynikow do sprawozdania*/
void save_sp2(Config *cfg, FILE *file, int file_index) {
    fprintf(file, "%" PRIuFAST32 "\n", cfg->start_vertex);
    for (U32f i = 0; i < cfg->num_v; i++) {
        fprintf(file, "%" PRIuFAST32 " %" PRIuFAST32  " %" PRIuFAST32 "\n", i, cfg->res_sp->parents[i], cfg->res_sp->distances[i]);
    }
}
void save_prim(Config *cfg, FILE *file, int file_index) {
    fprintf(file, "%" PRIuFAST32 "\n", cfg->res_prim->sum_weights);
    for (U32f i = 0; i < cfg->num_v; i++) {
        fprintf(file, "%" PRIuFAST32 " %" PRIuFAST32 "\n", cfg->res_prim->parent_weight[i].parent, cfg->res_prim->parent_weight[i].weight);
    }
}
void save_kruskal(Config *cfg, FILE *file, int file_index) {
    fprintf(file, "%" PRIuFAST32 "\n", cfg->res_kruskal->sum_weights);
    for (U32f i = 0; i < cfg->num_v - 1; i++) {
        fprintf(file, "%" PRIuFAST32 " %" PRIuFAST32 " %" PRIuFAST32 "\n", 
                cfg->res_kruskal->edges[i].src, 
                cfg->res_kruskal->edges[i].dst, 
                cfg->res_kruskal->edges[i].weight);
    }
}
void save_config(Config *cfg, char *base_folder, char *alg_name, int file_index) {
    char file_name[256];
    snprintf(file_name, sizeof(file_name), "%s/data_conf/%s/%d.txt", base_folder, alg_name, file_index);
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_name);
        return;
    }
    
    fprintf(file, "%" PRIuFAST32 "\n", cfg->num_v);
    fprintf(file, "%" PRIuFAST32 "\n", cfg->density);
    fprintf(file, "%" PRIuFAST32 "\n", cfg->start_vertex);
    fclose(file);
    char file_name_res[256];
    snprintf(file_name_res, sizeof(file_name_res), "%s/data_res/%s/%d.txt", base_folder, alg_name, file_index);
    file = fopen(file_name_res, "w");
    if (cfg->alg_type == DIJKSTRA_LIST || cfg->alg_type == DIJKSTRA_MATRIX || 
        cfg->alg_type == BELMAN_FORD_LIST || cfg->alg_type == BELMAN_FORD_MATRIX_EDGE_LIST || 
        cfg->alg_type == BELMAN_FORD_MATRIX_NO_EDGE_LIST) {
        save_sp2(cfg, file, file_index);
        }
    else if (cfg->alg_type == PRIM_LIST || cfg->alg_type == PRIM_MATRIX) {
        save_prim(cfg, file, file_index);
    }
    else if (cfg->alg_type == KRUSKAL_LIST || cfg->alg_type == KRUSKAL_MATRIX) {
        save_kruskal(cfg, file, file_index);
    }
    fclose(file);
}

int full_test() {  
    char *file_exc = "../data/output1.csv";
    char *base_folder = "../data1";

    FILE *fileExcel = fopen(file_exc, "a");
    double density_d[] = {0.25, 0.5, 0.99};
    U32f sizes[] = {16,32, 64, 128,256, 512, 1024};
    U32f index = 0;
    U32f seed = (U32f) time(NULL);
    Config *cfg = (Config *)malloc(sizeof(Config));
    for (int type = 0; type < ALG_TYPE_COUNT; type++) {
        Alg_type alg_type = (Alg_type) type;
        srand(seed);

        
        for (int size = 0; size < 7; size ++){
            U32f s = sizes[size]; 
            for (int dens = 0; dens < 3; dens++){
                
                double mean = 0.0;
                 
                for (int rep = 0; rep < 50; rep ++){
                    create_config_random_weights(cfg, s, density_d[dens], alg_type, rand() % s);
                    int ind_f = save_edges(cfg->graph, alg_names[type], base_folder);
                    free_unused_config(cfg, alg_type);
                    struct timespec start, end;
                   
                    clock_gettime(CLOCK_MONOTONIC, &start);
                    run_alg(cfg); 
                    clock_gettime(CLOCK_MONOTONIC, &end);
                    mean += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e6;
                    save_config(cfg, base_folder, alg_names[type], ind_f);
                    free_config(cfg);
                }
                
                mean /= 50.0;
                if(fileExcel != NULL) {
                    fprintf(fileExcel, "%s,%"PRIuFAST32",%lf,%lf\n", alg_names[type], s, density_d[dens] , mean);
                } else {
                    fprintf(stderr, "errorr%s\n", file_exc);
                }
                
            }
        }
    }
    free(cfg);
    fclose(fileExcel);

    return 0;
}