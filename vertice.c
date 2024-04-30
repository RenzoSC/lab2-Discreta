#include <stdlib.h>
#include <assert.h>

#include "vertice.h"

vertice create_vertice(u32 id){
    vertice v = malloc(sizeof(struct vertice_st));
    v->vecinos = NULL;
    v->col = 0;
    v->id = id;
    v->last_max = 0;
    v->grado =0;
    assert(v!=NULL);
    return v;
}

u32 get_vertice_id(vertice v){
    assert(v!=NULL);
    return v->id;
}

u32 get_grado_vertice(vertice v){
    assert(v!=NULL);
    return v->grado;
}

u32 get_color_vertice(vertice v){
    assert(v!=NULL);
    return v->col;
}

u32 get_first_vecino(vertice v){
    assert(v!=NULL);
    assert(v->grado>0);
    return v->vecinos[0];
}

u32 get_last_vecino(vertice v){
    assert(v!=NULL);
    assert(v->grado>0);
    return v->vecinos[v->grado-1];
}

u32 get_vecino_index(vertice v, u32 index){
    assert(v!=NULL);
    return v->vecinos[index];
}

void set_color(vertice v, color colorv){
    assert(v!=NULL);
    v->col = colorv;
}


void add_vecino(vertice v, u32 vec){
    assert(v!=NULL);
    if (v->last_max == v->grado)
    {   
        u32 new_max = v->grado == 0 ? 1 : 2*v->grado;
        v->vecinos = realloc(v->vecinos, sizeof(u32)*new_max);
        v->last_max = new_max;
    }
    v->vecinos[v->grado]= vec;
    v->grado++;
}

void pop_vecino(vertice v){
    assert(v!=NULL);
    assert(v->grado>0);
    v->grado --;
}

vertice destroy_vertice(vertice v){
    assert(v!=NULL);
    v->col=0;
    v->id=0;
    v->grado =0;
    v->last_max = 0;
    free(v->vecinos);
    v->vecinos = NULL;
    free(v);
    v=NULL;
    assert(v==NULL);
    return v;
}