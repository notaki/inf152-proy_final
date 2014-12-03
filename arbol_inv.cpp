/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 2)

En este archivo se define la función que lee de un archivo los registros del
inventario. */
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "itson.hpp"
using namespace std;

namespace ITSON {

/* Devuelve un árbol binario de objetos CILES. Los objetos se leen del archivo
   de nombre contenido en arch_inventario.
   El archivo tiene un objeto por línea, cada uno de formato:

   clave campus almacen nombre cantidad unidad */
Arbol_binario<CILES> genera_arbol()
{
    Arbol_binario<CILES> arbol;

    ifstream ifs {arch_inventario};
    if (!ifs) throw runtime_error {"Error abriendo archivo: " + arch_inventario};

    /* Mientras no se llege al final del archivo, lee entradas. */
    while (ifs)
    try {
        for (CILES tmp; ifs >> tmp; arbol.inserta(tmp,ciles_cmp));
    }
    catch (throwable::Unidad_desconocida& e) {
        cerr << e.what() << ". Registro descartado.\n";
    }

    return arbol;
}

}
