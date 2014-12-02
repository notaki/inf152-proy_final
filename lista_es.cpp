/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En este archivo se define la función que lee de archivo las entradas y salidas
del inventario. */
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
        throw throwable::Archivo_desconocido {arch};

    forward_list<CILES> lista;

    ifstream ifs{arch};
    if (!ifs) throw runtime_error {"Error abriendo archivo: " + arch};

    auto it = lista.before_begin();
    for (string clave; ifs >> clave; ) {
        string campus, almacen, nombre, str_cantidad, str_unidad;
        ifs >> campus >> almacen;

        /* Para leer un nombre con espacios, guarda el resto de la línea a
           nombre. Formato de nombre: nombre cantidad unidad */
        getline(ifs,nombre);
        /* Elimina el espacio que en el archivo separa a almacen de nombre. */
        nombre.erase(nombre.begin());

        /* Guarda la unidad en nombre a str_unidad y elimínala de nombre. */
        auto pos = nombre.find_last_of(' ');
        str_unidad = nombre.substr(pos+1);
        nombre = nombre.substr(0,pos);

        /* Guarda la cantidad en nombre a str_cantidad y elimínala de nombre. */
        pos = nombre.find_last_of(' ');
        str_cantidad = nombre.substr(pos+1);
        nombre = nombre.substr(0,pos);

        try {
            it = lista.emplace_after(it,clave,campus,almacen,
                                     nombre,str_cantidad,str_unidad);
        }
        catch (throwable::Unidad_desconocida& e) {
            std::cerr << e.what() << ". Registro descartado.\n";
        }
    }

    return lista;
}

}
