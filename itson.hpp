/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En esta cabecera se reunen las declaraciones de las funciones, constantes y las
definiciones excepciones necesarias para el manejo del inventario. */
#ifndef ITSON_HPP_INCLUDED
#define ITSON_HPP_INCLUDED

#include <forward_list>
#include "ciles.hpp"

namespace ITSON {

// Únicos parámetros para la función genera_lista(string)
const std::string arch_entrada = "ENTRADAS.DAT";
const std::string arch_salida = "SALIDAS.DAT";

std::forward_list<CILES> genera_lista(const std::string& arch);

const std::string arch_inventario = "INVENTAR.DAT";

/* Contiene las clases para arrojar como excepciones. */
namespace throwable {
    class Unidad_fuera_de_rango {
    public:
        Unidad_fuera_de_rango(const Unidad unidad)
                          : valor{std::to_string(int(unidad))} { }
        std::string what() { return "Valor de Unidad fuera de rango: " + valor; }
    private:
        std::string valor;
    };

    class Unidad_desconocida {
    public:
        Unidad_desconocida(const std::string& unidad) : unidad{unidad} { }
        std::string what() { return "Unidad desconocida: " + unidad; }
    private:
        std::string unidad;
    };

    class Archivo_desconocido {
    public:
        Archivo_desconocido(const std::string& archivo) : archivo{archivo} { }
        std::string what() { return "Arcihivo desconocido: " + archivo; }
    private:
        std::string archivo;
    };
}

}

#endif // ITSON_HPP_INCLUDED
