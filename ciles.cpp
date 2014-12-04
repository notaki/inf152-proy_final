/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En este archivo se definen los constructores de la clase CILES para guardar un
registro de artículo del inventario. Se definen las funciones no miembro para la
entrada y salida de registros por archivo y para definir el ordenamiento de
objetos CILES. También se definen funciones de apoyo. */
#include <unordered_map>
#include <stdexcept>
#include <cctype>
#include "ciles.hpp"
using namespace std;

namespace ITSON {

CILES::CILES()
     : cantidad_{0.0}, unidad_{"unid"}
{

}

CILES::CILES(const string& clave, const string& campus, const string& almacen,
             const string& nombre, double cantidad, const string& unidad)
     : clave_{clave}, campus_{campus}, almacen_{almacen},
       nombre_{nombre}, cantidad_{cantidad}, unidad_{unidad}
{

}

/* Se definen las unidades reconocidas por CILES. */
unordered_map<string,bool>& inicializar_m_unidad()
{
    unordered_map<string,bool>* m = new unordered_map<string,bool>;

    /* Si (*m)[unidad] == true  entonces su valor se representa con enteros.
       Si (*m)[unidad] == false entonces su valor se representa con reales. */
    (*m)["unid"] = true;    // unidades
    (*m)["mc"] = false;     // metros cúbicos
    (*m)["kg"] = false;     // kilogramos

    return *m;
}

unordered_map<string,bool>& m_unidad()
{
    static unordered_map<string,bool>& m = inicializar_m_unidad();

    return m;
}

void validar_cantidad(double cantidad, const string& unidad)
{
    /* Revisa si la cantidad de una unidad entera tiene un valor válido. */
    if (m_unidad()[unidad] && (cantidad-int(cantidad) != 0.0))
        throw invalid_argument {"cantidad real " + to_string(cantidad) +
                                " para unidad " + unidad};
}

/* Asigna una nueva cantidad. */
CILES& CILES::operator=(double cantidad)
{
    validar_cantidad(cantidad_,unidad_);

    cantidad_ = cantidad;

    return *this;
}

/* Avanza el buffer hasta llegar a un campo o a una nueva línea. El parámetro
   rechaza_newline indica si no debería encontrarse una nueva línea, y de
   encontrarse, arroja una excepción. */
void salta_espacio(ifstream& ifs, bool rechaza_newline)
{
    char ch;
    while (ifs.get(ch) && isspace(ch) && ch!='\n');

    if (ch=='\n' && rechaza_newline)
        throw invalid_argument {"insuficientes campos para un registro"};

    ifs.unget();
}

void lee_campo(ifstream& ifs, string& str, bool rechaza_newline = true)
{
    ifs >> str;
    salta_espacio(ifs,rechaza_newline);
}

/* Lee un registro de un archivo con una línea por registro. Los campos están
   separados por espacio(s) (como se define en isspace(char)) y se encuentran en
   el siguiente orden: clave campus almacen nombre cantidad unidad

   Ya que los registros se leen de archivo, la validación se hace aquí en lugar
   del constructor de CILES.

   La función falla al leer un registro si:
   -una línea no tiene suficientes campos.
   -se lee una unidad no definida.
   -la unidad se contabiliza con enteros pero se lee una cantidad real.

   La función arroja una excepción:
   invalid_argument si no hay suficientes campos para completar un registro. */
ifstream& operator>>(ifstream& ifs, CILES& registro)
{
    string clave, campus, almacen, nombre, str_cantidad, unidad;
    lee_campo(ifs,clave);
    lee_campo(ifs,campus);
    lee_campo(ifs,almacen);
    lee_campo(ifs,nombre);
    lee_campo(ifs,str_cantidad);
    lee_campo(ifs,unidad,false);
    if (!ifs) return ifs;

    /* Como un nombre puede tener espacios, se lee hasta la nueva línea. */
    for (string tmp; ifs.peek()!='\n'; unidad += " "+tmp) lee_campo(ifs,tmp,false);

    /* Busca el penúltimo campo (cantidad) en unidad. */
    auto pos = unidad.find_last_of(' ');
    /* Si existe en unidad... */
    if (pos!=string::npos) {
        /* agrega cantidad a nombre, */
        nombre += " "+str_cantidad;

        /* guarda en cantidad la cantidad real, */
        auto tmp = unidad.find_last_of(' ',pos-1);
        str_cantidad = unidad.substr(tmp+1,pos);

        /* agrega a nombre el resto del nombre, */
        if(tmp!=string::npos) nombre += " "+unidad.substr(0,tmp);

        /* y quita de unidad todo menos la unidad. */
        unidad = unidad.substr(pos+1);
    }

    try {
        double cantidad = stod(str_cantidad);

        registro = CILES {clave,campus,almacen,nombre,cantidad,unidad};
    }
    catch (const logic_error& e) {
        ifs.clear(ios_base::failbit);
    }

    return ifs;
}

ofstream& operator<<(ofstream& ofs, const CILES& registro)
{
    ofs << registro.clave() << ' ' << registro.campus() << ' '
        << registro.almacen() << ' ' << registro.nombre() << ' '
        << registro.cantidad() << ' ' << registro.unidad();

    return ofs;
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
