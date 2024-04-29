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
    color used_colors[delta];
    memset(used_colors, -1, sizeof(used_colors));
    color color_topaint;
    for (u32 i = 0; i < nv; i++)
    {
        u32 vert_selected = Orden[i];
        
        if (Color(vert_selected, G)!= 0)
        {
            return INT32_MAX;
        }

        u32 how_many_vecinos = Grado(vert_selected, G);
        for (u32 j = 0; j < how_many_vecinos; j++)
        {
            u32 vert_vecino = Vecino(j,vert_selected,G);
            color color_vecino = Color(vert_vecino, G);
            if (color_vecino!=0)                        //si es un color valido
            {
                used_colors[color_vecino]=i;            //lo marca como usado en esta iteración
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
        vert_painted++;                                 //actualizo la cantidad de vertices q pinté
    }

    if (vert_painted != nv)          //reviso que haya pintado todos los vertices
    {
        return INT32_MAX;
    }

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
typedef bool (*Comparador)(setSetColor, setSetColor);

bool goes_before_by_len(setSetColor x, setSetColor y);
unsigned int partition(setSetColor a[], unsigned int izq, unsigned int der, Comparador comp);
void quick_sort_rec(setSetColor a[], unsigned int izq, unsigned int der, Comparador comp);
void quick_sort(setSetColor a[], unsigned int length, Comparador comp);


char GulDukat(Grafo G,u32* Orden){
    //Creamos la lista con la Maxima cantidad de Colores Posibles.
    u32 delta = Delta(G)+1;
    u32 nv = NumeroDeVertices(G);
    color maxColor = 0;
    setSetColor lista [delta];
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
    quick_sort(lista, delta, goes_before_by_Mm);

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

    //Chequeamos Errores y Finalizamos
    if(va != nv){return '1';}
    return '0';
}

char ElimGarak(Grafo G, u32 *Orden){
    //Creamos la lista con la Maxima cantidad de Colores Posibles.
    u32 delta = Delta(G)+1;
    u32 nv = NumeroDeVertices(G);
    color maxColor = 0;
    setSetColor lista [delta];
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
            return '1';
        }
        insertArray(&lista[col-1], vert_selected);
        lista[col-1].color = col;
    }

    //O nlogn
    //Ordenar Lista por Lista[i].lenvertices de menor a mayor
    quick_sort(lista, delta, goes_before_by_len);

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
        for (u32 j = va; j < (va + lista[c2].lenVertices); j++)
        {
            va++;
            Orden[j] = lista[c2].vertices[j - va];
        }
        lista[c2].añadido = true;
    }

    //Añadir c1
    if(lista[c1].añadido == false)
    {
        for (u32 j = va; j < (va + lista[c1].lenVertices); j++)
        {
            va++;
            Orden[j] = lista[c1].vertices[j - va];
        }
        lista[c1].añadido = true;
    }

    //O n
    //Liberamos la memoria de las Estructuras Auxiliares
    for (u32 i = 0; i < delta; i++) {
        freeArray(&lista[i]);
    }

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
  a->vertices[a->lenVertices++] = element;
}

//Funcion para Liberar el Arreglo de Vertices
void freeArray(setSetColor *a) {
  free(a->vertices);
  a->vertices = NULL;
  a->lenVertices = a->size = 0;
}

//Funciones de ordenación

bool goes_before_by_len(setSetColor x, setSetColor y){
    return x.lenVertices <= y.lenVertices;
}

bool goes_before_by_Mm(setSetColor x, setSetColor y){
    if(x.color % 4 == 0)
    {
        if(y.color % 4 == 0)
        {
            return x.mGrande >= y.mGrande;
        }
        return true;
    }
    if(x.color % 2 == 0)
    {
        if(y.color % 2 == 0)
        {
            return (x.mGrande + x.mChica) >= (y.mGrande + y.mChica);
        }
        return true;
    }
    return x.mChica >= y.mChica;
}

void swap(setSetColor a[], int x, int y){
    setSetColor z = a[x];
    a[x] = a[y];
    a[y]= z;
}

unsigned int partition(setSetColor a[], unsigned int izq, unsigned int der, Comparador comp) {
    unsigned int i, j,ppiv;
    ppiv = izq;
    i= izq +1;
    j= der;
    while (i<=j)
    {
        if (comp(a[i],a[ppiv]))
        {
            i+=1;
        }else if(comp(a[ppiv], a[j])){
            j-=1;
        }else if(comp(a[ppiv], a[i]) && comp(a[j], a[ppiv])){
            swap(a,i,j);
            i+=1;
            j-=1;
        }
    }
    swap(a,ppiv,j);
    ppiv =j;
    return ppiv;
}

void quick_sort_rec(setSetColor a[], unsigned int izq, unsigned int der, Comparador comp) {
    unsigned int ppiv;
    if (der > izq){
        ppiv = partition(a, izq, der, comp);
        if(ppiv!=0){
            quick_sort_rec(a, izq, ppiv-1, comp);
        }
        quick_sort_rec(a, ppiv+1, der, comp);
    }
}

void quick_sort(setSetColor a[], unsigned int length, Comparador comp) {
    quick_sort_rec(a, 0u, (length == 0u) ? 0u : length - 1u, comp);
}


    /*
    To do List para ElimGarak (* significa hecho; - significa no hecho)
   * Creamos un Array de SetsDeVerticesSameColor de Delta + 1 de tamaño. O 1
   * Iteramos en los Vertices del Array y los agregamos donde corresponda en la Estructura Anterior (Index = Color - 1)
   * Ademas llevamos la cuenta de cual es el Color mas Alto usado hasta entonces. O n
   * SetsDeVerticesSameColor: con Color, Lista de Vertices, Cardinal de esa Lista y un Bool de Añadido

   * Ordeno la lista por lenvertices. O nlogn

   * añado a los que no sean el 1 o el 2 llevando la cuenta de cuantos ya añadi. O n
   * cuando no queden mas añado el 2 y el 1 y termina. O n

   * Libero la memoria de las estructuras auxiliares O n

   de momento va O 3n creo, pero falta el sort por lo qeu quiezas queda nlogn
    */

    /*
    To do List para GulDukat (* significa hecho; - significa no hecho)
   * Creamos un Array de SetsDeVerticesSameColor de Delta + 1 de tamaño. O 1
   * Iteramos en los Vertices del Array y los agregamos donde corresponda en la Estructura Anterior (Index = Color - 1)
   * Ademas llevamos la cuenta de cual es el Color mas Alto usado hasta entonces. O n
   * SetsDeVerticesSameColor: con Color, Lista de Vertices, Cardinal de esa Lista y un Bool de Añadido y M y n 

   * Ordeno la lista por disibile por 4(M) -> Par(M+m) -> Impar(m) MILANESA

   * Añado a los colores divisibles por 4, ordenados por M, luego a los pares por M + m y finalmente a los impares por m. O nlogn

   * Libero la memoria de las estructuras auxiliares O n

   de momento va O 3n creo, pero falta el sort por lo qeu quiezas queda nlogn
    */

    /*
    el ">":

    para ElimGarak ">" seria "lista[i].lenvertices > lista[j].lenvertices"

    en cambio para GulDukat es mas complicado, ">" seria algo como
    "  if lista[i].color div 4 y lista[j].color no, entonces la primera va primero"
    "elif lista[i].color div 4 y lista[j].color tambien, se ordenan por lista[x].mGrande"
    "elif lista[i].color div 2 y lista[j].color no, entonces la primera va primero"
    "elif lista[i].color div 2 y lista[j].color tambien, se ordenan por lista[x].mGrande + lista[x].mChica"
    "elif lista[i].color no div  y lista[j].color tampoco, se ordenan por lista[x].mChica"

    Sabemos que QuickSort es O nlogn
    */