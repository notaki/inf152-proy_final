/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En esta cabecera se reunen las declaraciones de las funciones, constantes y las
definiciones excepciones necesarias para el manejo del inventario. */
#ifndef ITSON_HPP_INCLUDED
#define ITSON_HPP_INCLUDED

#include <forward_list>
#include "ciles.hpp"
#include "arbol.hpp"

namespace ITSON {

// Parámetros para la función genera_lista(string)
const std::string arch_entrada = "ENTRADAS.DAT";
const std::string arch_salida = "SALIDAS.DAT";

/* Devuelve una lista ligada de objetos CILES. Los objetos se leen del archivo
   de nombre contenido en el parámetro. */
std::forward_list<CILES> genera_lista(const std::string& arch);

// Nombre del archivo con los registros del inventario
const std::string arch_inventario = "INVENTARIO.DAT";

/* Devuelve un árbol binario de objetos CILES. Los objetos se leen del archivo
   de nombre contenido en arch_inventario. */
Arbol_binario<CILES> genera_arbol();

/* Actualiza el inventario según las entradas o salidas. El parámetro arch es
   para llamar a la función genera_lista(). */
void actualiza_inventario(Arbol_binario<CILES>& inventario, const std::string& arch);

/* Borra el contenido del archivo de nombre arch_inventario. */
void vaciar_inventario(Arbol_binario<CILES>& inventario);

/* Lista los registros del inventario hacia la pantalla. */
void listar_inventario(Arbol_binario<CILES>& inventario);

/* Contiene las clases para arrojar como excepciones. */
namespace throwable {
    class Unidad_fuera_de_rango {
    public:
        Unidad_fuera_de_rango(const std::string err_msg, const Unidad unidad)
                      : err_msg{err_msg + " " + std::to_string(int(unidad))} { }
        std::string what() { return err_msg; }
    private:
        std::string err_msg;
    };

    class Unidad_desconocida {
    public:
        Unidad_desconocida(const std::string err_msg, const std::string& unidad)
                          : err_msg{err_msg + " " + unidad} { }
        std::string what() { return err_msg; }
    private:
        std::string err_msg;
    };

    class Archivo_desconocido {
    public:
        Archivo_desconocido(const std::string err_msg, const std::string& archivo)
                           : err_msg{err_msg + " " + archivo} { }
        std::string what() { return err_msg; }
    private:
        std::string err_msg;
    };
}

}

#endif // ITSON_HPP_INCLUDED
