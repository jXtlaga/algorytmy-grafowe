#ifndef IO_GRAPH_H
#define IO_GRAPH_H

#include <stdio.h>
#include <inttypes.h>
#include "graph.h"
#include "mst.h"
#include "sp.h"
#include "types.h"
#include "configuration.h"

/* print_inc_dir_matrix
 * Wypisuje macierz incydencji dla grafu skierowanego.
 */
void print_inc_dir_matrix(I32f *inc_matrix, U32f vertices, U32f density);

/* print_inc_undir_matrix
 * Wypisuje macierz incydencji dla grafu nieskierowanego.
 */
void print_inc_undir_matrix(U32f *inc_matrix, U32f vertices, U32f density);

/* print_graph
 * Wypisuje reprezentację grafu (np. listę sąsiedztwa).
 */
void print_graph(Graph* graph);

/* print_edges
 * Wypisuje wszystkie krawędzie grafu.
 */
void print_edges(Graph *graph);

/* print_res_prim
 * Wypisuje wynik działania algorytmu Prima.
 */
void print_res_prim(Res_prim *mst, U32f numVertices, U32f start_vertex);

/* print_res_kruskal
 * Wypisuje wynik działania algorytmu Kruskala.
 */
void print_res_kruskal(Res_kruskal *res, U32f num_edges);

/* print_res_sp
 * Wypisuje wynik działania algorytmów najkrótszej ścieżki.
 */
void print_res_sp(Res_sp* result, U32f num_vericies, U32f start_vertex);

/* load_graph_from_file
 * Wczytuje graf z pliku do struktury Graph.
 */
void load_graph_from_file(const char *file_name, File_config *cfg_file, Config *cfg);

/* save_edges
 * Zapisuje krawędzie grafu do pliku.
 */
int save_edges(Graph *graph, char* base_file_name, char *folder);

/* save_weight
 * Zapisuje wagę grafu do pliku.
 */
void save_weight(Graph *graph, char* base_file_name, char *folder, U32f weight, int file_index);

/* save_sp
 * Zapisuje wyniki algorytmu najkrótszej ścieżki do pliku.
 */
void save_sp(Res_sp *res, char* base_file_name, char *folder, U32f num_vert, U32f start_vertex, int file_index);

/* print_edges_to_file
 * Wypisuje krawędzie grafu do pliku.
 */
void print_edges_to_file(Graph *graph, char* base_file_name, char*folder, U32f weight);

/* print_edges_to_file_bitmask
 * Wypisuje krawędzie grafu do pliku na podstawie bitmaski.
 */
void print_edges_to_file_bitmask(Graph *graph, char* base_file_name, U64 bitmask);


#endif // IO_GRAPH_H
