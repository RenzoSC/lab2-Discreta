
#ifndef _APIG24_h
#define _APIG24_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "EstructuraGrafo24.h"

typedef struct Grafo_St *Grafo;

typedef unsigned int color;

typedef unsigned int u32;

Grafo ConstruirGrafo();

void DestruirGrafo(Grafo G);

//extraer información del grafo

u32 NumeroDeVertices(Grafo G);

u32 NumeroDeLados(Grafo G);

u32 Delta(Grafo G);

// Extraer informacion de los vertices

u32 Grado(u32 i,Grafo G);
color Color(u32 i,Grafo G);
u32 Vecino(u32 j,u32 i,Grafo G);

// Asignar colores

void AsignarColor(color x,u32 i,Grafo  G);
void ExtraerColores(Grafo G,color* Color);
void ImportarColores(color* Color,Grafo  G);

#endif