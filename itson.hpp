/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En esta cabecera se reunen las declaraciones de las clases y funciones
necesarias para manejar el inventario. */
#ifndef ITSON_HPP_INCLUDED
#define ITSON_HPP_INCLUDED

#include <forward_list>
#include "ciles.hpp"
#include "arbol.hpp"

namespace ITSON {

/* Devuelve una lista ligada de los registros de entradas o salidas del
   inventario del archivo asociado con el parámetro. */
std::forward_list<CILES> generar_lista(std::ifstream& ifs);

/* Devuelve un árbol binario de objetos CILES de los registros del inventario
   del archivo asociado con el parámetro. */
Arbol_binario<CILES> generar_arbol(std::ifstream& ifs);

/* Actualiza el inventario según las entradas o salidas de la lista. El
   parámetro ofstream es el stream asociado al archivo del inventario. El
   parámetro entrada indica si la lista es de entrada. */
void actualizar_inventario(Arbol_binario<CILES>& inventario,
                          const std::forward_list<CILES>& lista, bool entrada);

/* Borra el contenido del archivo del inventario y de su árbol binario. */
void vaciar_inventario(Arbol_binario<CILES>& inventario);

/* Lista los registros del inventario hacia la pantalla (implementado por el
   parámetro imprimir).*/
void listar_inventario(Arbol_binario<CILES>& inventario,
                       void (* imprimir)(CILES& registro));

}

#endif // ITSON_HPP_INCLUDED
