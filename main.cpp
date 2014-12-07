/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30) */
#include <iostream>
#include <vector>
#include "itson.hpp"
using namespace ITSON;

namespace interfaz {

typedef const std::forward_list<CILES> Lista;
typedef Arbol_binario<CILES> Arbol;

/* Opciones para escoger en los diferentes menús de la interfaz. */
enum class Opc {claves,entradas,salidas,inventario,salir};
const Opc actualizar {Opc::claves};
const Opc iniciar {Opc::entradas};
const Opc listar {Opc::salidas};

/* Interfaz de la figura 16-24. */
Opc menu_actualizar(Lista& lista_ent, Lista& lista_sal, Arbol& inventario)
{
    Opc opcion=Opc::salir;

    // Consigue la opción que escoge el usuario en la interfaz.

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

/* Interfaz de la figura 16-25. */
void imprimir(const CILES& registro)
{
    // Imprimir el registro a pantalla.
}

/* Interfaz de la figura 16-23. */
Opc menu_inventario(Lista& lista_ent, Lista& lista_sal, Arbol& inventario)
{
    Opc opcion=Opc::salir;

    // Consigue la opción que escoge el usuario en la interfaz.

    switch (opcion) {
    case actualizar:
        while (menu_actualizar(lista_ent,lista_sal,inventario)!=Opc::salir);
        break;
    case iniciar: vaciar_inventario(inventario); break;
    case listar: listar_inventario(inventario,imprimir); break;
    case Opc::salir: break;
    case Opc::inventario:
        throw std::invalid_argument {"menu_inventario() opción inválida"};
    default: throw std::out_of_range {"menu_inventario() opción no definida"};
    }

    return opcion;
}

void imprime_lista(Lista& lista)
{
    for (auto it=lista.begin(); it!=lista.end(); ++it) imprimir(*it);
}

/* Guarda las claves. */
std::vector<std::string>& v_claves()
{
    static std::vector<std::string> v_c;
    return v_c;
}

void actualiza_v_claves(const CILES& registro)
{
    bool found=false;
    for (auto s : v_claves())
        if (s==registro.clave()) {
            found=true;
            break;
        }

    if (!found) v_claves().push_back(registro.clave());
}

void actualiza_v_claves(Lista& lista_ent, const Arbol& inventario)
{
    /* Agrega a v_claves() las claves nuevas en la lista y el inventario. */
    for (auto it=lista_ent.begin(); it!=lista_ent.end(); ++it) {
        bool found=false;
        for (auto s : v_claves())
            if (s==it->clave()) {
                found=true;
                break;
            }

        if (!found) v_claves().push_back(it->clave());
    }
    inventario.visitar_en_orden(actualiza_v_claves);
}

/* Interfaz similar a la figura 16-25. */
void imprimir_claves()
{
    // Imprime las claves en v_claves() a pantalla.
}

/* Se asume que esta opción de menú debe imprimir las claves a pantalla. */
void claves(Lista& lista_ent, const Arbol& inventario)
{
    actualiza_v_claves(lista_ent,inventario);

    imprimir_claves();
}

/* Interfaz de la figura 16-22. */
Opc menu_principal(Lista& lista_ent, Lista& lista_sal, Arbol& inventario)
{
    Opc opcion=Opc::salir;

    // Consigue la opción que escoge el usuario en la interfaz.

    switch (opcion) {
    case Opc::claves: claves(lista_ent,inventario); break;
    case Opc::entradas: imprime_lista(lista_ent); break;
    case Opc::salidas: imprime_lista(lista_sal); break;
    case Opc::inventario:
        while (menu_inventario(lista_ent,lista_sal,inventario)!=Opc::salir);
        break;
    case Opc::salir: break;
    default: throw std::out_of_range {"menu_principal() opción no definida"};
    }

    return opcion;
}

} // Final de namespace interfaz

/* Devuelve el ifstream asociado con el archivo de nombre del parámetro. */
std::ifstream& genera_ifstream(const std::string& arch="")
{
    static std::ifstream ifs;
    if (ifs.is_open()) ifs.close();
    if (!arch.empty()) {
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
    auto lista_ent=generar_lista(genera_ifstream(arch_ent));

    const std::string arch_sal {"SALIDAS.DAT"};
    auto lista_sal=generar_lista(genera_ifstream(arch_sal));

    const std::string arch_inventario {"INVENTARIO.DAT"};
    auto inventario=generar_arbol(genera_ifstream(arch_inventario));

    genera_ifstream();

    while (interfaz::menu_principal(lista_ent,lista_sal,inventario)
           !=interfaz::Opc::salir);
}
catch (const std::exception& e) {
    std::cerr<<e.what()<<'\n';
    return -2;
}
