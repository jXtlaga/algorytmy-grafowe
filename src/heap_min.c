#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "heap_min.h"

/* create_min_heap
 * Tworzy nową kopiec minimum o zadanej pojemności.
 * Zwraca wskaźnik na zaalokowaną strukturę Min_heap.
 */
Min_heap* create_min_heap(U32f capacity) {
    Min_heap* heap = (Min_heap*)malloc(sizeof(Min_heap));
    if (!heap) {
        return NULL; 
    }
    heap->arr = (Min_heap_node*)malloc(sizeof(Min_heap_node) * capacity);
    if (!heap->arr) {
        free(heap);
        return NULL; 
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

/* free_min_heap
 * Zwalnia pamięć zaalokowaną przez kopiec minimum.
 */
void free_min_heap(Min_heap* heap) {
    if (heap) {
        free(heap->arr);
        free(heap);
    }
}

/* min_heapify
 * Przywraca własność kopca minimum od zadanego indeksu w dół.
 */
void min_heapify(Min_heap* heap, U32f index) {
    U32f smallest = index;
    U32f left = 2 * index + 1;
    U32f right = 2 * index + 2;

    if (left < heap->size && heap->arr[left].weight < heap->arr[smallest].weight) {
        smallest = left;
    }
    if (right < heap->size && heap->arr[right].weight < heap->arr[smallest].weight) {
        smallest = right;
    }
    if (smallest != index) {
        Min_heap_node temp = heap->arr[index];
        heap->arr[index] = heap->arr[smallest];
        heap->arr[smallest] = temp;
        min_heapify(heap, smallest);
    }
}

/* min_heap_insert
 * Dodaje nowy wierzchołek o zadanej wadze do kopca minimum.
 */
void min_heap_insert(Min_heap* heap, U32f vertex, U32f weight) {
    if (heap->size >= heap->capacity) {
        return; // Heap is full
    }
    Min_heap_node new_node = {vertex, weight};
    heap->arr[heap->size] = new_node;
    heap->size++;
    min_heapify(heap, (heap->size - 1) / 2);
}

/* min_heap_extract_min
 * Usuwa i zwraca wierzchołek o najmniejszej wadze z kopca minimum.
 */
Min_heap_node min_heap_extract_min(Min_heap* heap) {
    if (heap->size == 0) {
        return (Min_heap_node){UINT32_MAX, UINT32_MAX}; // Return an invalid node if heap is empty
    }
    Min_heap_node min_node = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    min_heapify(heap, 0);
    return min_node;
}

/* min_heap_decrease_key
 * Zmniejsza wagę wierzchołka w kopcu minimum.
 */
void min_heap_decrease_key(Min_heap* heap, U32f vertex, U32f new_weight) {
    for (U32f i = 0; i < heap->size; i++) {
        if (heap->arr[i].vertex == vertex) {
            if (new_weight < heap->arr[i].weight) {
                heap->arr[i].weight = new_weight;
                while (i > 0 && heap->arr[(i - 1) / 2].weight > heap->arr[i].weight) {
                    Min_heap_node temp = heap->arr[i];
                    heap->arr[i] = heap->arr[(i - 1) / 2];
                    heap->arr[(i - 1) / 2] = temp;
                    i = (i - 1) / 2;
                }
            }
            return;
        }
    }
}

/* min_heap_find
 * Zwraca indeks wierzchołka w kopcu minimum lub UINT_FAST32_MAX jeśli nie znaleziono.
 */
U32f min_heap_find(Min_heap* heap, U32f vertex) {
    for (U32f i = 0; i < heap->size; i++) {
        if (heap->arr[i].vertex == vertex) {
            return i;
        }
    }
    return UINT_FAST32_MAX;
}
