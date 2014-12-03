/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 2)

En este archivo se definen las funciones para manejar el inventario según las
entradas o salidas y las opciones de menú en la interfaz gráfica. También se
definen funciones de apoyo. */
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "itson.hpp"
using namespace std;

namespace ITSON {

ofstream& objeto_ofs()
{
    static ofstream ofs {arch_inventario};
    if (!ofs.is_open()) ofs.open(arch_inventario);
    if (!ofs) throw runtime_error {"Error abriendo/creando archivo: " + arch_inventario};

    return ofs;
}

void actualiza_registro(CILES& registro)
{
    objeto_ofs() << registro << '\n';
}

void actualiza_archivo(Arbol_binario<CILES>& inventario)
{
    inventario.visitar_en_orden(actualiza_registro);
    objeto_ofs().close();
}

/* Actualiza el inventario según las entradas o salidas. El parámetro arch es
   para llamar a la función genera_lista(). */
void actualiza_inventario(Arbol_binario<CILES>& inventario, const std::string& arch)
{
    forward_list<CILES> lista = genera_lista(arch);
    if (lista.empty()) return;

    /* Por cada artículo en la lista de entrada/salida... */
    for (auto it = lista.begin(); it != lista.end(); ++it)
    try {
        /* busca su registro en el inventario... */
        CILES& registro = inventario.busca(*it,ciles_cmp);

        /* y actualiza su cantidad. */
        double aumento = it->cantidad();
        registro.cantidad() += (arch == arch_entrada) ? aumento : -aumento;
    }
    /* Si no existe en el inventario... */
    catch (throwable::Operacion_fallida) {
        /* y el archivo es de entrada, agrégalo al inventario. */
        if (arch == arch_entrada) inventario.inserta(*it,ciles_cmp);
        else cerr << "Salida de artículo inexistente:"
                  << "\nclave: " << it->clave()
                  << "\ncampus: " << it->campus()
                  << "\nalmacen: " << it->almacen() << '\n';
    }

    actualiza_archivo(inventario);
}

/* Borra el contenido del archivo de nombre arch_inventario. */
void vaciar_inventario(Arbol_binario<CILES>& inventario)
{
    /* Vacía el archivo. */
    objeto_ofs().close();
    /* Vacía el árbol. */
    inventario = genera_arbol();
}

/* Lista los registros del inventario hacia la pantalla. */
void listar_inventario(Arbol_binario<CILES>& inventario,
                       void (* imprimir)(CILES& registro))
{
    if (inventario.vacio()) return;

    inventario.visitar_en_orden(imprimir);
}

}
