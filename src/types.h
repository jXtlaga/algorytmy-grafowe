#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define U32f uint_fast32_t
#define U64f uint_fast64_t
#define I32f int_fast32_t
#define I64f int_fast64_t

#define U32 uint32_t 
#define U64 uint64_t
// Struktura przechowywujaca krawedz
typedef struct Edge {
    U32f src, dst, weight;
} Edge;
#endif // TYPES_H