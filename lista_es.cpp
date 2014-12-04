/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En este archivo se define la función genera_lista() que lee de un archivo las
entradas o salidas del inventario. */
#include <forward_list>
#include <stdexcept>
#include "ciles.hpp"
using namespace std;

namespace ITSON {

/* Devuelve una lista ligada de los registros de entradas o salidas del
   inventario del archivo asociado con el parámetro. */
forward_list<CILES> genera_lista(ifstream& ifs)
{
    if (!ifs.is_open())
        throw invalid_argument {"genera_lista() de archivo no abierto"};
    ifs.seekg(0);

    forward_list<CILES> lista;

    auto it=lista.before_begin();
    /* Mientras no se llege al final del archivo, lee registros. */
    while (ifs) {
        for (CILES reg; ifs>>reg; it=lista.insert_after(it,reg));
        if (ifs.fail() && !ifs.eof()) ifs.clear();
    }

    return lista;
}

}
