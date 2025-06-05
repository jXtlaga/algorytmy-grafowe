#ifndef HEAP_MIN_H
#define HEAP_MIN_H

#include <stdint.h>
#include <stddef.h>
#include "types.h"

// Struktura reprezentująca węzeł kopca minimalnego
typedef struct Min_heap_node {
    U32f vertex;
    U32f weight;
} Min_heap_node;

// Struktura reprezentująca kopiec minimalny
typedef struct Min_heap {
    Min_heap_node *arr;
    U32f size;
    U32f capacity;
} Min_heap;

// Funkcja tworząca nowy kopiec minimalny o podanej pojemności
Min_heap* create_min_heap(U32f capacity);

// Funkcja zwalniająca pamięć zajmowaną przez kopiec minimalny
void free_min_heap(Min_heap* heap);

// Funkcja wstawiająca nowy węzeł do kopca minimalnego
void min_heap_insert(Min_heap* heap, U32f vertex, U32f weight);

// Funkcja przywracająca własność kopca minimalnego dla węzła o podanym indeksie
void min_heapify(Min_heap* heap, U32f index);

// Funkcja usuwająca i zwracająca węzeł o minimalnej wartości z kopca
Min_heap_node min_heap_extract_min(Min_heap* heap);

// Funkcja zmniejszająca wartość klucza węzła o podanym wierzchołku
void min_heap_decrease_key(Min_heap* heap, U32f vertex, U32f new_weight);

U32f min_heap_find(Min_heap* heap, U32f vertex);

#endif // HEAP_MIN_H