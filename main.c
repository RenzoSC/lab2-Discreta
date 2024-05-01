#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "API2024parte2.h"
#include "APIG24.h"


typedef struct vertData {
    u32 indice;
    u32 grade;
}vertData;
typedef bool (*Comparador)(vertData, vertData);

unsigned int partition(vertData a[], unsigned int izq, unsigned int der, Comparador comp);
void quick_sort_rec(vertData a[], unsigned int izq, unsigned int der, Comparador comp);
void quick_sort(vertData a[], unsigned int length, Comparador comp);
bool goes_before_by_grade(vertData x, vertData y){
    return x.grade <= y.grade;

}

//Qsort 2 (Igual que el de APO2024parte 2 pero con vertData en lugar de set de colores)
void swap2(vertData a[], int x, int y){
    vertData z = a[x];
    a[x] = a[y];
    a[y]= z;
}

unsigned int partition2(vertData a[], unsigned int izq, unsigned int der, Comparador comp) {
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
            swap2(a,i,j);
            i+=1;
            j-=1;
        }
    }
    swap2(a,ppiv,j);
    ppiv =j;
    return ppiv;
}

void quick_sort_rec2(vertData a[], unsigned int izq, unsigned int der, Comparador comp) {
    unsigned int ppiv;
    if (der > izq){
        ppiv = partition2(a, izq, der, comp);
        if(ppiv!=0){
            quick_sort_rec2(a, izq, ppiv-1, comp);
        }
        quick_sort_rec2(a, ppiv+1, der, comp);
    }
}

void quick_sort2(vertData a[], unsigned int length, Comparador comp) {
    quick_sort_rec2(a, 0u, (length == 0u) ? 0u : length - 1u, comp);
}

////////////////////////Funciones de Orden Inicial///////////////////////////
void OrdenNatural(u32 n, u32 *Orden) {
    for (u32 i = 0; i < n; i++){ Orden[i] = i; }
}
void OrdenAntiNatural(u32 n, u32 *Orden) {
    for (u32 i = 0; i < n; i++){ Orden[i] = n - 1 - i; }
}

void OrdenParImpar(u32 n, u32 *Orden){
    u32 cota_sup = n%2==0? n/2: (n+1)/2;
    u32 start = n%2==0? n-2: n-1;
    for (u32 i = 0; i < cota_sup; i++)
    {
        Orden[i]=start;
        start-=2;
    }
    start = 1;
    for (u32 i = cota_sup; i < n; i++)
    {
        Orden[i]=start;
        start+=2;
    }
    
}

void OrdenByGrade(u32 n, u32 *Orden, Grafo G){
    vertData* vertices = (vertData*) malloc(n * sizeof(vertData));
    for (u32 i = 0; i < n; i++)
    {
        vertices[i].indice=i;
        vertices[i].grade = Grado(i,G);
    }
    quick_sort2(vertices, n, goes_before_by_grade);
    for (u32 i = 0; i < n; i++)
    {
        Orden[i] = vertices[i].indice;
    }  
}

void OrdenInvalido(u32 n, u32 *Orden){
    for (u32 i = 0; i < n; i++){Orden[i]=1;}
}

////////////////////////Funciones Iterativas///////////////////////////
u32 Iterar50Veces(color *colores, Grafo G, u32 *Orden, color *Arr)
{
    for (u32 i = 0; i < 50; i++)
    {
        if (GulDukat(G,Orden) == '1'){printf("Critical Gul Dukat Error\n"); return 1;}
        *colores = Greedy(G,Orden);
        printf("Gul Dukat N° %u: %u\n",(i+1), *colores);
        
        if (ElimGarak(G,Orden) == '1'){printf("Critical Elim Garak Error\n"); return 1;}
        *colores = Greedy(G,Orden);
        printf("Elim Garak N° %u: %u\n",(i+1), *colores);
    }
    //Ahora Arr tiene el Ultimo Coloreo y colores la cantidad de Colores.
    ExtraerColores(G,Arr);
    printf("\n");
    return 0;
}

u32 Iterar500Veces(Grafo G, u32 *Orden)
{
    color colores;
    for (u32 i = 0; i < 500; i++)
    {
        if(rand() % 2)
        {
            if (GulDukat(G,Orden) == '1'){printf("Critical Gul Dukat Error\n"); return 1;}
            colores = Greedy(G,Orden);
            printf("Random Gul Dukat N° %u: %u\n",(i+1), colores);
        }else{
            if (ElimGarak(G,Orden) == '1'){printf("Critical Elim Garak Error\n"); return 1;}
            colores = Greedy(G,Orden);
            printf("Random Elim Garak N° %u: %u\n",(i+1), colores);
        }
    }
    return 0;
}

int main()
{
    Grafo g = ConstruirGrafo();

    u32 nv = NumeroDeVertices(g);
    u32* orden = (u32*) malloc(nv * sizeof(u32));
    color colores[5]; //Colores de los 5 Ordenes
    color* arr[5];

    //Orden Natural Inicial
    arr[0] = (color*) malloc(nv * sizeof(color));
    OrdenNatural(nv,orden);
    colores[0] = Greedy(g,orden);
    printf("Orden Natural: %u\n\n", colores[0]);

    //Iterar GD -> Greedy -> EG -> Greedy 50 veces
    if(colores[0] == INT32_MAX){ printf("Invalid Order\n"); OrdenNatural(nv,orden);} //Usamos Orden Natural que Sabemos es un Orden Valido
    else if(Iterar50Veces(&colores[0],g,orden,arr[0])){return 1;}


    //Orden Anti Natural Inicial
    arr[1] = (color*) malloc(nv * sizeof(color));
    OrdenAntiNatural(nv,orden);
    colores[1] = Greedy(g,orden);
    printf("Orden Anti-Natural: %u\n\n", colores[1]);

    //Iterar GD -> Greedy -> EG -> Greedy 50 veces
    if(colores[1] == INT32_MAX){ printf("Invalid Order\n"); OrdenNatural(nv,orden);} //Usamos Orden Natural que Sabemos es un Orden Valido
    else if(Iterar50Veces(&colores[1],g,orden,arr[1])){return 1;}


    //Orden Par Impar Inicial
    arr[2] = (color*) malloc(nv * sizeof(color));
    OrdenParImpar(nv,orden);
    colores[2] = Greedy(g,orden);
    printf("Orden Par Impar: %u\n\n", colores[2]);

    //Iterar GD -> Greedy -> EG -> Greedy 50 veces
    if(colores[2] == INT32_MAX){ printf("Invalid Order\n"); OrdenNatural(nv,orden);} //Usamos Orden Natural que Sabemos es un Orden Valido
    else if(Iterar50Veces(&colores[2],g,orden,arr[2])){return 1;}


    //Orden Grado Inicial
    arr[3] = (color*) malloc(nv * sizeof(color));
    OrdenByGrade(nv,orden,g);
    colores[3] = Greedy(g,orden);
    printf("Orden Grado: %u\n\n", colores[3]);

    //Iterar GD -> Greedy -> EG -> Greedy 50 veces
    if(colores[3] == INT32_MAX){ printf("Invalid Order\n"); OrdenNatural(nv,orden);} //Usamos Orden Natural que Sabemos es un Orden Valido
    else if(Iterar50Veces(&colores[3],g,orden,arr[3])){return 1;}


    //Orden Invalido Inicial
    arr[4] = (color*) malloc(nv * sizeof(color));
    OrdenInvalido(nv,orden);
    colores[4] = Greedy(g,orden);
    printf("Orden Invalido: %u\n\n", colores[4]);

    //Iterar GD -> Greedy -> EG -> Greedy 50 veces
    if(colores[4] == INT32_MAX){printf("Invalid Order\n"); OrdenNatural(nv,orden);} //Usamos Orden Natural que Sabemos es un Orden Valido
    else if(Iterar50Veces(&colores[4],g,orden,arr[4])){return 1;}


    //Comparar Coloreos y Repintar
    u32 minC = INT32_MAX; //Minimo Color
    u32 minI = 0; //Minimo Indice
    for (u32 i = 0; i < 5; i++)
    {
        if(minC > colores[i])
        {
            minC = colores[i];
            minI = i;
        }
    }
    if(minC == INT32_MAX){
        printf("No Valid Orders\n");
        return 1;
    }
    ImportarColores(arr[minI],g);

    //Libero los Array de Coloreos
    for (u32 i = 0; i < 5; i++){free(arr[i]);}

    printf("Iteraciones Random Partiendo del Orden %u\n\n", (minI + 1));
    if(Iterar500Veces(g,orden)){return 1;}

    free(orden);
    DestruirGrafo(g);
    return 0;
}
