
#ifndef _ESTRUCTURAGRAFO24_H
#define _ESTRUCTURAGRAFO24_H

#include "vertice.h"

typedef unsigned int u32;

typedef struct Grafo_St {
    u32 num_vertices;
    u32 num_lados;
    u32 delta;
    vertice * list_vertice;
} GrafoSt;

#endif