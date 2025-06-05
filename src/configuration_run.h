#ifndef CONFIGURATION_RUN_H
#define CONFIGURATION_RUN_H

#include "configuration.h"
#include "graph.h"
#include "io_graph.h"
#include "types.h"
#include <stdint.h>

/* print_config_file
 * wyświetla zawartość struktury File_config
 */
void print_config_file(const File_config *cfg_file);

/* print_config_results
 * wyświetla wyniki algorytmu na podstawie typu algorytmu w strukturze Config
 */
void print_config_results(const Config *cfg);

/* print_config_graph_representation
 * wyświetla reprezentację grafu (macierz incydencji lub lista sąsiedztwa) na podstawie konfiguracji
 */
void print_config_graph_representation(const Config *cfg, const File_config *cfg_file);

/* run_config_file_var
 * generuje losowy graf i uruchamia algorytm na podstawie konfiguracji z pliku
 */
void run_config_file_var(File_config *cfg_file, Config *cfg);

/* run_config_file_load
 * wczytuje graf z pliku i uruchamia algorytm na podstawie konfiguracji z pliku
 */
void run_config_file_load(File_config *cfg_file, Config *cfg);

#endif /* CONFIGURATION_RUN_H */