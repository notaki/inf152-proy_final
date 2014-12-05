/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 2)

En este archivo se define la función genera_arbol() que lee de un archivo los
registros del inventario. */
#include "ciles.hpp"
#include "arbol.hpp"
using namespace std;

namespace ITSON {

/* Devuelve un árbol binario de objetos CILES de los registros del inventario
   del archivo asociado con el parámetro. */
Arbol_binario<CILES> genera_arbol(ifstream& ifs)
{
    if (!ifs.is_open())
        throw invalid_argument {"genera_arbol() de ifstream::is_open()==false"};
    ifs.seekg(0);

    Arbol_binario<CILES> arbol;

    /* Mientras no se llege al final del archivo, lee registros. */
    while (ifs) {
        for (CILES tmp; ifs>>tmp; arbol.insertar(tmp,ciles_cmp));
        if (ifs.fail() && !ifs.eof()) ifs.clear();
    }

    return arbol;
}

}
