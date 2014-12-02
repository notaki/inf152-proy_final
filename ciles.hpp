/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En esta cabecera se declara la clase CILES para guardar un registro de artículo
del inventario. También se declaran tipos y funciones de apoyo. */
#ifndef CILES_HPP_INCLUDED
#define CILES_HPP_INCLUDED

#include <string>
#include <istream>

namespace ITSON {

/* Un objeto Numero guarda la cantidad de un artículo. */
union Numero {
    int entero;
    double real;
};

/* Un objeto Unidad guarda la unidad de un artículo. Un valor positivo
   indica que su miembro cantidad es un Numero::entero. Un valor negativo
   indica que su miembro cantidad es un Numero::real. */
enum Unidad {unid,   // unidades
             mc = -9,// metros cúbicos
             kg};    // kilogramos

/* Un objeto CILES representa un registro de artículo. */
class CILES {
public:
    CILES();
    /* Arroja la excepción Unidad_desconocida definida en itson.hpp si la cadena
       unidad no contiene el nombre de una Unidad como se define en el tipo. */
    CILES(const std::string& clave, const std::string& campus,
          const std::string& almacen, const std::string& nombre,
          const std::string& cantidad, const std::string& unidad);

    std::string clave() const { return clave_; }
    std::string campus() const { return campus_; }
    std::string almacen() const { return almacen_; }

    std::string nombre() const { return nombre_; }
    Numero cantidad() const { return cantidad_; }
    Unidad unidad() const { return unidad_; }

    void asignar_cantidad(const Numero cantidad);
private:
    std::string clave_;   // 1ra llave de ordenamiento
    std::string campus_;  // 2da llave de ordenamiento
    std::string almacen_; // 3ra llave de ordenamiento

    std::string nombre_;  // Nombre del artículo
    Numero cantidad_;     // Cantidad del artículo
    Unidad unidad_;       // Unidad del artículo
};

std::istream& operator>>(std::istream& ifs, CILES& var);

/* Arroja la excepción Unidad_fuera_de_rango definida en itson.hpp si el
   parámetro no es una Unidad definida en el tipo. */
std::string a_cadena(const Unidad unidad);

/* Arroja la excepción Unidad_desconocida definida en itson.hpp si la cadena
   unidad no contiene el nombre de una Unidad como se define en el tipo. */
Unidad a_unidad(const std::string& unidad);

/* Devuelve true si la cantidad de la unidad se guarda como número real. */
bool cantidad_real(const Unidad unidad);

/* Función que define el ordenamiento de los objetos tipo CILES.
   Devuelve         si
    > 0         reg1 > reg2
     0          reg1 == reg2
    < 0         reg1 < reg2 */
int ciles_cmp(const CILES& reg1, const CILES& reg2);

}

#endif // CILES_HPP_INCLUDED
