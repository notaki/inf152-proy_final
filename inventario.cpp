/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 2)

En este archivo se definen las funciones para manejar el inventario según las
las opciones de menú en la interfaz gráfica (Actualizar, Iniciar y Listar).
También se definen funciones de apoyo. */
#include <forward_list>
#include <iostream>
#include "ciles.hpp"
#include "arbol.hpp"
using namespace std;

namespace ITSON {

/* Nombre de archivo del inventario. */
const string arch_inventario="INVENTARIO.DAT";

/* Devuelve el ofstream asociado con el archivo del inventario. */
ofstream& ofs_inv()
{
    static ofstream ofs {arch_inventario};
    if (!ofs.is_open()) ofs.open(arch_inventario);
    if (!ofs) throw runtime_error {"error abriendo/creando archivo " +
                                    arch_inventario};
    return ofs;
}

void actualiza_registro(const CILES& registro)
{
    ofs_inv()<<registro<<'\n';
}

void actualiza_archivo(const Arbol_binario<CILES>& inventario)
{
    if (inventario.vacio()) return;

    inventario.visitar_en_orden(actualiza_registro);
    ofs_inv().close();
}

/* Actualiza el inventario según las entradas o salidas de la lista. El
   parámetro ofstream es el stream asociado al archivo del inventario. El
   parámetro entrada indica si la lista es de entrada. */
void actualizar_inventario(Arbol_binario<CILES>& inventario,
                          const forward_list<CILES>& lista, bool entrada)
{
    /* Por cada artículo en la lista de entrada/salida... */
    for (auto it=lista.begin(); it!=lista.end(); ++it)
    try {
        /* busca su registro en el inventario... */
        CILES& registro=inventario.buscar(*it,ciles_cmp);

        /* y actualiza su cantidad. */
        registro+=it->cantidad()*((entrada) ? 1 : -1);
    }
    /* Si no existe en el inventario... */
    catch (const range_error& e) {
        /* y el archivo es de entrada, agrégalo al inventario. */
        if (entrada) inventario.insertar(*it,ciles_cmp);
        else cerr<<e.what()<<"\nllaves: "<<it->clave()<<' '<<it->campus()<<' '
                 <<it->almacen()<<'\n';
    }

    actualiza_archivo(inventario);
}

/* Borra el contenido del inventario en su archivo y su árbol binario. */
void vaciar_inventario(Arbol_binario<CILES>& inventario)
{
    /* Vacía el archivo. */
    ofs_inv().close();
    /* Vacía el árbol. */
    if (!inventario.vacio()) inventario=Arbol_binario<CILES> { };
}

/* Lista los registros del inventario hacia la pantalla (implementado por el
   parámetro imprimir).*/
void listar_inventario(Arbol_binario<CILES>& inventario,
                       void (* imprimir)(const CILES& registro))
{
    if (inventario.vacio()) return;

    inventario.visitar_en_orden(imprimir);
}

}
