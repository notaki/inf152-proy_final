/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 5) */
#ifndef INTERFAZ_HPP_INCLUDED
#define INTERFAZ_HPP_INCLUDED

#include <ncurses.h>
#include <forward_list>
#include <utility>
#include "ciles.hpp"
#include "arbol.hpp"

namespace ITSON {

namespace GUI {

/* Opciones para escoger en los diferentes menús. */
enum class Opc {claves,inventario,entradas,salidas,salir};
// Intentar using
const Opc Opc_actualizar {Opc::inventario};
const Opc Opc_iniciar {Opc::entradas};
const Opc Opc_listar {Opc::salidas};

void inicializar();
void finalizar();

void manejar_claves(const std::vector<std::string>& claves);
void manejar_registros(const ITSON::Arbol_binario<ITSON::CILES>& inventario);
void manejar_registros(const std::forward_list<ITSON::CILES>& lista);
Opc manejar_menu(std::pair<int,int> (* gui_menu)(), int min_opc, int max_opc);
std::pair<int,int> gui_actualizar();
std::pair<int,int> gui_inventario();
std::pair<int,int> gui_principal();

} // Final del namespace GUI

} // Final del namespace ITSON

#endif // INTERFAZ_HPP_INCLUDED
