=== konfiguracja ===
warto zobaczyc przykladowe konfiguracje
1. 
.alg_type

mozliwosci:
    DIJKSTRA_LIST,
    DIJKSTRA_MATRIX,
    BELMAN_FORD_LIST,
    BELMAN_FORD_MATRIX_EDGE_LIST,
    BELMAN_FORD_MATRIX_NO_EDGE_LIST,
    PRIM_LIST,
    PRIM_MATRIX,
    KRUSKAL_LIST,
    KRUSKAL_MATRIX
2. 
.num_v - liczba wierzcholkow

3. 
.start_vertex - wiorzcholek startowy

4. 
.density - gestosc grafu

5. wyswieltanie reprezentacji macierzowej jest tylko dla algorytmow ktorych argumentem jest macierz incydencji
np. DIJKSTRA_MATRIX, PRIM_MATRIX, KRUSKAL_MATRIX etc.
.out_matrix - (true/false) 
.out_list - (true/false)
6. .file_name - nazwa pliku (bez kropki na poczatku) 
