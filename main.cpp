/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30) */
#include <iostream>
#include <vector>
#include "itson.hpp"
#include "interfaz.hpp"
using namespace ITSON;

using Lista=const std::forward_list<CILES>;
using Arbol=Arbol_binario<CILES>;

namespace ITSON {

namespace GUI {

Opc menu_actualizar(Lista& lista_ent, Lista& lista_sal, Arbol& inventario)
{
    Opc opcion {manejar_menu(gui_actualizar,int(Opc::entradas),int(Opc::salir))};

    switch (opcion) {
    case Opc::entradas: actualizar_inventario(inventario,lista_ent,true); break;
    case Opc::salidas: actualizar_inventario(inventario,lista_sal,false); break;
    case Opc::salir: break;
    case Opc::claves: case Opc::inventario:
        throw std::invalid_argument {"menu_actualizar() opción inválida"};
    default: throw std::out_of_range {"menu_actualizar() opción no definida"};
    }

    return opcion;
}

Opc menu_inventario(Lista& lista_ent, Lista& lista_sal, Arbol& inventario)
{
    Opc opcion {manejar_menu(gui_inventario,int(Opc_actualizar),int(Opc::salir))};

    switch (opcion) {
    case Opc_actualizar:
        while (menu_actualizar(lista_ent,lista_sal,inventario)!=Opc::salir);
        break;
    case Opc_iniciar: vaciar_inventario(inventario); break;
    case Opc_listar: manejar_registros(inventario); break;
    case Opc::salir: break;
    case Opc::claves:
        throw std::invalid_argument {"menu_inventario() opción inválida"};
    default: throw std::out_of_range {"menu_inventario() opción no definida"};
    }

    return opcion;
}

/* Guarda las claves. */
std::vector<std::string>& claves()
{
    static std::vector<std::string> claves;
    return claves;
}

void actualiza_v_claves(const CILES& registro)
{
    for (auto s : claves()) if (s==registro.clave()) return;
    claves().push_back(registro.clave());
}

/* Agrega a v_claves() las claves nuevas en la lista y el inventario. */
void actualiza_v_claves(Lista& lista_ent, const Arbol& inventario)
{
    for (auto it=lista_ent.begin(); it!=lista_ent.end(); ++it) {
        bool found=false;
        for (auto s : claves())
            if (s==it->clave()) {
                found=true;
                break;
            }

        if (!found) claves().push_back(it->clave());
    }

    inventario.visitar_en_orden(actualiza_v_claves);
}

/* Se asume que esta opción de menú debe imprimir las claves a pantalla. */
void claves(Lista& lista_ent, const Arbol& inventario)
{
    claves().clear();
    actualiza_v_claves(lista_ent,inventario);
    manejar_claves(claves());
}

Opc menu_principal(Lista& lista_ent, Lista& lista_sal, Arbol& inventario)
{
    Opc opcion {manejar_menu(gui_principal,int(Opc::claves),int(Opc::salir))};

    switch (opcion) {
    case Opc::claves: claves(lista_ent,inventario); break;
    case Opc::inventario:
        while (menu_inventario(lista_ent,lista_sal,inventario)!=Opc::salir);
        break;
    case Opc::entradas: manejar_registros(lista_ent); break;
    case Opc::salidas: manejar_registros(lista_sal); break;
    case Opc::salir: break;
    default: throw std::out_of_range {"menu_principal() opción no definida"};
    }

    return opcion;
}

} // Final del namespace GUI

} // Final del namespace ITSON

/* Devuelve el ifstream asociado con el archivo de nombre del parámetro, o el
   anteriormente abierto. */
std::ifstream& obtener_ifs(const std::string& arch="")
{
    static std::ifstream ifs;
    if (!arch.empty()) {
        if (ifs.is_open()) ifs.close();
        ifs.open(arch);
        if (!ifs) throw std::runtime_error {"error abriendo/creando archivo " +
                                             arch};
        if (ifs.exceptions()!=std::ios_base::badbit)
            ifs.exceptions(ifs.exceptions()|std::ios_base::badbit);
    }
    return ifs;
}

int main()
try {
    const std::string arch_ent {"ENTRADAS.DAT"};
    Lista lista_ent {generar_lista(obtener_ifs(arch_ent))};

    const std::string arch_sal {"SALIDAS.DAT"};
    Lista lista_sal {generar_lista(obtener_ifs(arch_sal))};

    const std::string arch_inventario {"INVENTARIO.DAT"};
    Arbol inventario {generar_arbol(obtener_ifs(arch_inventario))};

    obtener_ifs().close();

    GUI::inicializar();
    while (GUI::menu_principal(lista_ent,lista_sal,inventario)!=GUI::Opc::salir);
    GUI::finalizar();
}
catch (const std::exception& e) {
    std::cerr<<e.what()<<'\n';
    return -2;
}
