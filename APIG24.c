#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "EstructuraGrafo24.h"
#include "APIG24.h"

#define MAXLINE 1024

//Construccion de grafos

Grafo ConstruirGrafo(){
    Grafo grafo = (Grafo)malloc(sizeof(struct Grafo_St));
    if (grafo == NULL) return NULL;
    grafo->delta = 0;
    u32 m,n;
    //bool error = false;
    char car[MAXLINE];
    while (fgets(car, MAXLINE, stdin ))
    {
        if (car[0] == 'c') continue;

        if (car[0]== 'e')
        {   
            printf("Se obtuvo un edge antes de indicar la cantidad de vértices y lados del grafo...\nSaliendo...");
            exit(-1);
        }else if (car[0] == 'p')
        {
            int ret = sscanf(car, "p edge %u %u\n",&n,&m);
            if (ret <2)
            {
                printf("Hubo un error al leer el numero de vertices y  lados %i", ret);
                exit(-1);
            }
            
            grafo->num_vertices = n;
            grafo->num_lados = m;
            grafo->list_vertice = malloc(sizeof(vertice)*n);
            break;
        }
    }
    bool* is_already = malloc(sizeof(bool)*grafo->num_vertices);
    for (size_t i = 0; i < grafo->num_vertices; i++)
    {
        is_already[i]= false;     //acordar de destruir el array
    }
    
    for (size_t i = 0; i < grafo->num_lados; i++)
    {
        char *is_eof = fgets(car, MAXLINE, stdin);
        if (is_eof==NULL && i < grafo->num_lados-1)
        {
            return NULL;
        }else if (is_eof==NULL)
        {
            printf("error al leer la linea con fgets....");
            exit(-1);
        }
        
        if (car[0]=='e')
        {   
            u32 vert1,vert2;
            int r = sscanf(car,"e %u %u", &vert1,&vert2);
            if (r<2)
            {
                printf("No se pasaron 2 vertices como correspondería: %i\n",r);
                exit(-1);
            }

            if (is_already[vert1]==false)
            {
                vertice vertice1= create_vertice(vert1);
                grafo->list_vertice[vert1]= vertice1;
                is_already[vert1]=true;
            }
            
            if (is_already[vert2]==false)
            {
                vertice vertice2= create_vertice(vert2);
                grafo->list_vertice[vert2]= vertice2;
                is_already[vert2]= true;
            }
            
            add_vecino(grafo->list_vertice[vert1], vert2);

            add_vecino(grafo->list_vertice[vert2], vert1);

            u32 gradov1= get_grado_vertice(grafo->list_vertice[vert1]);
            u32 gradov2= get_grado_vertice(grafo->list_vertice[vert2]);
            if (grafo->delta < gradov1|| grafo->delta < gradov2)
            {
                grafo->delta = gradov1 <= gradov2 ? gradov2 : gradov1;
            }
            
        }else{
            printf("Mal formato de el paso del edge");
            exit(-1);
        }        
    }
    free(is_already);
    return grafo;
}

void DestruirGrafo(Grafo G){
    assert(G!=NULL);
    for (size_t i = 0; i < G->num_vertices; i++)
    {
        destroy_vertice(G->list_vertice[i]);
    }
    free(G->list_vertice);
    free(G);
    G = NULL;
    assert(G==NULL);
}

// Extraer información del grafo

u32 NumeroDeVertices(Grafo G){
    assert(G != NULL);
    return G->num_vertices;
}

u32 NumeroDeLados(Grafo G){
    assert(G != NULL);
    return G->num_lados;
}

u32 Delta(Grafo G){
    assert(G != NULL);
    return G->delta;
}

// Extraer informacion de los vertices

u32 Grado(u32 i,Grafo G){
    assert(G!=NULL);
    if (i >= G->num_vertices){
        return 0;
    }else{
        return get_grado_vertice(G->list_vertice[i]);
    }
}

color Color(u32 i,Grafo G){
    assert(G!=NULL);
    if (i >= G->num_vertices){
        return UINT32_MAX;
    }else{
        return G->list_vertice[i]->col;
    }
}

u32 Vecino(u32 j,u32 i,Grafo G){
    assert(G!=NULL);
    if ((i>=NumeroDeVertices(G)) || (i<=NumeroDeVertices(G) && j>=Grado(i,G)) || j >= Grado(i,G))
    {
        return UINT32_MAX;
    }
    return get_vecino_index(G->list_vertice[i], j);
}

// Asignar colores

void AsignarColor(color x,u32 i,Grafo  G){
    assert(G!=NULL);
    if (i<NumeroDeVertices(G))
    {
        set_color(G->list_vertice[i], x);
    }
}

void ExtraerColores(Grafo G,color* Color){
    assert(G!=NULL);
    for (u32 i = 0; i < G->num_vertices; i++) {
        Color[i] = G->list_vertice[i]->col;
    }
}

void ImportarColores(color* Color,Grafo  G){
    assert(G!=NULL);
    for (u32 i = 0; i < G->num_vertices; i++) {
        AsignarColor(Color[i], i, G);
    }
}
