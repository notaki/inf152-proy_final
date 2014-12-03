/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En este archivo se definen las funciones miembro de la estructura CILES para
guardar un registro del inventario. También se definen funciones de apoyo. */
#include "itson.hpp"
using namespace std;

namespace ITSON {

CILES::CILES()
{

}

/* Arroja la excepción Unidad_desconocida definida en itson.hpp si la cadena
   unidad no contiene el nombre de una Unidad como se define en el tipo. */
CILES::CILES(const string& clave, const string& campus, const string& almacen,
             const string& nombre, const string& cantidad, const string& unidad)
    : clave_{clave}, campus_{campus}, almacen_{almacen},
      nombre_{nombre}, cantidad_{stod(cantidad)}, unidad_{a_unidad(unidad)}
{

}

istream& operator>>(istream& ifs, CILES& var)
{
    string clave, campus, almacen, nombre, str_cantidad, str_unidad;
    ifs >> clave >> campus >> almacen;
    if (!ifs) return ifs;

    /* Para poder leer un nombre con espacios, se lee el resto de la línea.
       Se espera que en el resto de la línea tenga el formato:
       nombre cantidad unidad */
    getline(ifs,nombre);
    if (!ifs) return ifs;
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

    var = CILES {clave,campus,almacen,nombre,str_cantidad,str_unidad};

    return ifs;
}

std::ostream& operator<<(std::ostream& ofs, const CILES& var)
{
    return ofs << var.clave() << ' ' << var.campus() << ' '
               << var.almacen() << ' ' << var.nombre() << ' '
               << var.cantidad() << ' ' << a_cadena(var.unidad());
}

/* Arroja la excepción Unidad_fuera_de_rango definida en itson.hpp si el
   parámetro no es una Unidad definida en el tipo. */
string a_cadena(const Unidad unidad)
{
    switch (unidad) {
    case unid: return "unid";
    case mc  : return "mc";
    case kg  : return "kg";
    }
    throw throwable::Unidad_fuera_de_rango {"Valor de Unidad indefinido:",unidad};
}

/* Arroja la excepción Unidad_desconocida definida en itson.hpp si la cadena
   unidad no contiene el nombre de una Unidad como se define en el tipo. */
Unidad a_unidad(const string& unidad)
{
    if (unidad == "unid") return unid;
    if (unidad == "mc")   return mc;
    if (unidad == "kg")   return kg;
    throw throwable::Unidad_desconocida {"Unidad desconocida:",unidad};
}

/* Devuelve true si la cantidad de la unidad se guarda como número real. */
bool cantidad_real(const Unidad unidad)
{
    if (unidad < Unidad(1)) return true;
    return false;
}

/* Función que define el ordenamiento de los objetos tipo CILES.
   Devuelve         Si
    < 0         reg1 < reg2
     0          reg1 == reg2
    > 0         reg1 > reg2 */
int ciles_cmp(const CILES& reg1, const CILES& reg2) {
    int dif;

    if ((dif = reg1.clave().compare(reg2.clave()))) return dif;
    if ((dif = reg1.campus().compare(reg2.campus()))) return dif;
    return reg1.almacen().compare(reg2.almacen());
}

}
