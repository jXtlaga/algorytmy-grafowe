
#include "configuration_run.h"
int main(){
    char *file = "./configuration/config2.txt";
    File_config *file_config = (File_config *)malloc(sizeof(File_config));
    Config *cfg = (Config *)malloc(sizeof(Config));
    //1. generowanie grafu losowego i uruchomienie algorytmu
    read_config_file(file, file_config);
    print_config_file(file_config);
    run_config_file_var(file_config, cfg);
    free_graph(cfg->graph);
    
    //2. wczytanie grafu z pliku i uruchomienie algorytmu
    
    run_config_file_load(file_config, cfg);
    free_config_file(file_config);
    free_config(cfg); 
}
