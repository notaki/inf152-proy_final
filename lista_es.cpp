/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En este archivo se define la función que lee de un archivo las entradas o
salidas del inventario. */
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "itson.hpp"
using namespace std;

namespace ITSON {

/* Devuelve una lista ligada de objetos CILES. Los objetos se leen del archivo
   indicado por el parámetro. Los posibles parámetros se definen en itson.hpp.
   Los archivos tienen un objeto por línea, cada uno de formato:

   clave campus almacen nombre cantidad unidad */
forward_list<CILES> genera_lista(const string& arch)
{
    if (arch != arch_entrada && arch != arch_salida)
        throw throwable::Archivo_desconocido {"Archivo desconocido:",arch};

    forward_list<CILES> lista;

    ifstream ifs {arch};
    if (!ifs) throw runtime_error {"Error abriendo archivo: " + arch};

    auto it = lista.before_begin();
    /* Mientras no se llege al final del archivo, lee entradas. */
    while (ifs)
    try {
        for (CILES tmp; ifs >> tmp; it = lista.insert_after(it,tmp));
    }
    catch (throwable::Unidad_desconocida& e) {
        cerr << e.what() << " - Registro descartado.\n";
    }

    return lista;
}

}
