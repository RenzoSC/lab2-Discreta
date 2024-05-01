#include "API2024parte2.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//Greedy --> Complexity --> O( n * (d+d+d)) --> O(n * 3d)
// --> O(3n*d) --> O(3*2m) --> O(6m) --> O(m)
// Aclaración --> n*d = 2m por el lema de apreton de manos 
u32 Greedy(Grafo G, u32 * Orden){
    u32 nv = NumeroDeVertices(G);
    u32 vert_painted =0;
    u32 total_colors =0;
    u32 delta = Delta(G)+1;
    color * used_colors = malloc(delta * sizeof(color));
    memset(used_colors, 0, delta*sizeof(color));

    bool *already_colored = malloc(nv * sizeof(bool));    //esto lo hacemos para poder correr greedy sobre grafos ya pintados
    memset(already_colored,false,nv*sizeof(bool));   //asi vemos q vert fueron pintados en esta corrida de Greedy

    color color_topaint;
    for (u32 i = 0; i < nv; i++)
    {
        u32 vert_selected = Orden[i];
        if (already_colored[vert_selected])
        {
            free(already_colored);
            free(used_colors);
            return INT32_MAX;
        }
        if (i==0)
        {
            color_topaint=1;
            total_colors=1;
            AsignarColor(color_topaint,vert_selected, G);
            used_colors[0]=i;
            vert_painted++;
            already_colored[vert_selected]=true;
            continue;
        }

        u32 how_many_vecinos = Grado(vert_selected, G);
        for (u32 j = 0; j < how_many_vecinos; j++)
        {
            u32 vert_vecino = Vecino(j,vert_selected,G);
            if (already_colored[vert_vecino])                        //si el vertice ya fue pintado
            {   
                color color_vecino = Color(vert_vecino, G);
                used_colors[color_vecino-1]=i;            //lo marca como usado en esta iteración
            }
        }
        
        for (u32 j = 0; j < delta; j++)    
        {
            if (used_colors[j]!= i)       //dentro de todos los vertices usados busca aquel
            {                             //que no esté usado en esta iteración, ese será el color más chico
                color_topaint = j+1;
                break;
            }
        }
        total_colors = total_colors<color_topaint? color_topaint:total_colors;  //actualizo la cantidad de colores q usé
        AsignarColor(color_topaint,vert_selected, G);      //lo pinta
        already_colored[vert_selected]=true;                //lo marco como pintado
        vert_painted++;                                 //actualizo la cantidad de vertices q pinté
    }

    if (vert_painted != nv)          //reviso que haya pintado todos los vertices
    {
        free(already_colored);
        free(used_colors);
        return INT32_MAX;
    }
    free(already_colored);
    free(used_colors);
    return total_colors;

}

//Estructuras y Funciones Auxiliares para guardar la Informacion de los Grafos al Ordenarlos
//Estructura con una lista de Vertices de Determinado Color y su Tamaño y sus M y m.
typedef struct setSameColor {
    u32 *vertices;
    u32 lenVertices;
    u32 color;
    u32 mGrande;
    u32 mChica;
    bool añadido;
    u32 size; //Tamaño Allocado para el Array
} setSetColor;

void initArray(setSetColor *a, u32 initialSize);
void insertArray(setSetColor *a, u32 element);
void freeArray(setSetColor *a);

//Estructuras y Funciones Auxiliares para ordenar la lista de setSetColor
typedef int (*Comparador)(setSetColor, setSetColor);

int goes_before_by_len(const void *x, const void *y);
int goes_before_by_Mm(const void *x, const void *y);


char GulDukat(Grafo G,u32* Orden){
    //Creamos la lista con la Maxima cantidad de Colores Posibles.
    u32 delta = Delta(G)+1;
    u32 nv = NumeroDeVertices(G);
    color maxColor = 0;
    setSetColor* lista = malloc(delta * sizeof(setSetColor));
    //O n
    //Seria mejor si ya supieramos "r" (Cuantos colores son)
    for (u32 i = 0; i < delta; i++) {
        initArray(&lista[i], 1);
    }

    //O n 
    //Llenar la Estructura
    for (u32 i = 0; i < nv; i++)
    {
        u32 vert_selected = Orden[i];
        color col = Color(vert_selected, G); 
        maxColor = col>maxColor?col:maxColor;
        if (col == 0)
        {
            //Grafo no Coloreado Previamente
            free(lista);
            return '1';
        }
        insertArray(&lista[col-1], vert_selected);
        
        lista[col-1].color = col;
        u32 grad = Grado(vert_selected, G);

        lista[col-1].mGrande =lista[col-1].mGrande < grad? grad:lista[col-1].mGrande;
        lista[col-1].mChica =lista[col-1].mChica > grad? grad:lista[col-1].mChica;
    }

    //O nlogn
    //Ordeno la lista por disibile por 4(M) -> Par(M+m) -> Impar(m)
    qsort(lista,delta, sizeof(setSetColor), goes_before_by_Mm);
    
    //Si te pones a pensar solo pasa por las partes de añadir 1 vez por vertice asi que queda
    //O n
    //No hay necesidad de ver el arreglo pasado el maximo de colores utilizado
    //Iteramos la Lista ya Ordenada y añadimos
    u32 va = 0; //Cantidad de Vertices Agregados a Orden
    for (u32 i = 0; i < maxColor; i++)
    {
        if(lista[i].añadido == false)
        {
            u32 bva = va; //Base desde donde se añaden los nuevos Vertices
            for (u32 j = bva; j < (bva + lista[i].lenVertices); j++)
            {
                Orden[j] = lista[i].vertices[j - bva];
                va++;
            }
            lista[i].añadido = true;
        }
    }

    //O n
    //Liberamos la memoria de las Estructuras Auxiliares
    for (u32 i = 0; i < delta; i++) {
        freeArray(&lista[i]);
    }
    free(lista);

    //Chequeamos Errores y Finalizamos
    if(va != nv){
        printf("no agregados todos pa va:%d , nv:%d\n", va,nv);
        return '1';}
    return '0';
}

char ElimGarak(Grafo G, u32 *Orden){
    //Creamos la lista con la Maxima cantidad de Colores Posibles.
    u32 delta = Delta(G)+1;
    u32 nv = NumeroDeVertices(G);
    color maxColor = 0;
    setSetColor* lista = malloc(delta * sizeof(setSetColor));
    //O n
    //Seria mejor si ya supieramos "r" (Cuantos colores son)
    for (u32 i = 0; i < delta; i++) {
        initArray(&lista[i], 1);
    }

    //O n 
    //Llenar la Estructura
    for (u32 i = 0; i < nv; i++)
    {
        u32 vert_selected = Orden[i];
        color col = Color(vert_selected, G); 
        maxColor = col>maxColor?col:maxColor;
        if (col == 0)
        {
            //Grafo no Coloreado Previamente
            free(lista);
            return '1';
        }
        insertArray(&lista[col-1], vert_selected);
        lista[col-1].color = col;
    }

    //O nlogn
    //Ordenar Lista por Lista[i].lenvertices de menor a mayor
    qsort(lista,delta, sizeof(setSetColor), goes_before_by_len);
    //Si te pones a pensar solo pasa por las partes de añadir 1 vez por vertice asi que queda
    //O n
    //No hay necesidad de ver el arreglo pasado el maximo de colores utilizado
    //Los colores 1 y 2 se agregan aparte (Indices 0 y 1)
    //Iteramos la Lista ya Ordenada y añadimos
    u32 va = 0; //Cantidad de Vertices Agregados a Orden
    u32 c1 = 0; //Indice del Color 1
    u32 c2 = 0; //Indice del Color 2
    for (u32 i = 0; i < maxColor; i++)
    {
        if(lista[i].color == 1){c1 = i; continue;}
        if(lista[i].color == 2){c2 = i; continue;}
        if(lista[i].añadido == false)
        {
            u32 bva = va; //Base desde donde se añaden los nuevos Vertices
            for (u32 j = bva; j < (bva + lista[i].lenVertices); j++)
            {
                Orden[j] = lista[i].vertices[j - bva];
                va++;
            }
            lista[i].añadido = true;
        }
    }

    //Añadir c2
    if(lista[c2].añadido == false && maxColor > 1)
    {
        u32 bva = va;
        for (u32 j = bva; j < (bva + lista[c2].lenVertices); j++)
        {
            Orden[j] = lista[c2].vertices[j - bva];
            va++;
        }
        lista[c2].añadido = true;
    }

    //Añadir c1
    if(lista[c1].añadido == false)
    {
        u32 bva = va;
        for (u32 j = bva; j < (bva + lista[c1].lenVertices); j++)
        {
            Orden[j] = lista[c1].vertices[j - bva];
            va++;
        }
        lista[c1].añadido = true;
    }

    //O n
    //Liberamos la memoria de las Estructuras Auxiliares
    for (u32 i = 0; i < delta; i++) {
        freeArray(&lista[i]);
    }
    free(lista);

    //Chequeamos Errores y Finalizamos
    if(va != nv){return '1';}
    return '0';
}

//Funcion para Inicializar el Arreglo de Vertices
void initArray(setSetColor *a, u32 initialSize) {
  a->vertices = malloc(initialSize * sizeof(u32));
  a->lenVertices = 0;
  a->color = 0;
  a->añadido = false;
  a->mGrande = 0;
  a->mChica = 0;
  a->size = initialSize;
}

//Funcion para Insertar en el Arreglo de Vertices
void insertArray(setSetColor *a, u32 element) {
   if (a->lenVertices == a->size) {
    a->size *= 2;
    a->vertices = realloc(a->vertices, a->size * sizeof(u32));
  }
  a->vertices[a->lenVertices] = element;
  a->lenVertices++;
}

//Funcion para Liberar el Arreglo de Vertices
void freeArray(setSetColor *a) {
  free(a->vertices);
  a->vertices = NULL;
  a->lenVertices = a->size = 0;
}

//Funciones de ordenación
int goes_before_by_len(const void *x, const void *y){
    const setSetColor *dataA = (const setSetColor *)x;
    const setSetColor *dataB = (const setSetColor *)y;

    if (dataA->lenVertices ==0)
    {
        return 1;
    }if (dataB->lenVertices==0)
    {
        return -1;
    }else{
        if (dataA->lenVertices < dataB->lenVertices)
        {
            return -1;
        }else if (dataB->lenVertices > dataA->lenVertices)
        {
            return 1;
        }else{
            return 0;
        }
    }
}

int goes_before_by_Mm(const void *x, const void *y){
    const setSetColor *dataA = (const setSetColor *)x;
    const setSetColor *dataB = (const setSetColor *)y;
    if (dataA->lenVertices ==0)
    {
        return 1;
    }if (dataB->lenVertices==0)
    {
        return -1;
    }else{
        if(dataA->color % 4 == 0)
        {
            if(dataB->color % 4 == 0)
            {
                if (dataA->mGrande > dataB->mGrande)
                {
                    return -1;
                }else if(dataA->mGrande < dataB->mGrande){
                    return 1;
                }else{
                    return 0;
                }
            }
            return -1;
        }
        if(dataA->color % 2 == 0)
        {   
            if (dataB->color %4 ==0)
            {
                return 1;
            }
            
            if(dataB->color % 2 == 0)
            {
                if ((dataA->mGrande + dataA->mChica) > (dataB->mGrande + dataB->mChica))
                {
                    return -1;
                }else if((dataA->mGrande + dataA->mChica) < (dataB->mGrande + dataB->mChica)){
                    return 1;
                }else{
                    return 0;
                }
            }
            return -1;
        }
        if (dataB->color %2 !=0)
        {
            if (dataA->mChica > dataB->mChica)
            {
                return -1;
            }else if (dataA->mChica < dataB->mChica){
                return 1;
            }else{
                return 0;
            }
        }
        return 1;
    }
    
}