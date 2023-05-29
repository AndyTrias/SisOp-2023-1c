/*
int base = 0;
// son espacios fijos o varibles ??
// el vector_direcciones deben ser bytes
// mmu debe devolver la direccion_fisica

tabla de segmentos
limt | base

segmento_codigo
segmento_dato
segmento_pila
pero con 1 basta
*/

/*
Proceso 1 . lista de dos cosas, tamaño y base
Proceso 2 lo mismo
Huecos libres . lista de dos cosas tamaño y base

tamaño_hueco - tamaño proceso = nuevo tamaño
base hueco + tamaño proceso = nueva base
 
como averiguar que hay que compactar
2 formas
-1
int segmento más grande y si no es


//base y limite de cada segmento 
//permisos R - W - X

int mmu(direccion_logica){
    if (direccion_logica < limite){
        if (direccion_logica < limite - base){
            base = base + direccion_logica
        }
    }
    return base 
}*/