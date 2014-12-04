/* Johel Guerrero
   Tercer cuatrimestre 2014 (Nov. 30)

En esta cabecera se declara la clase CILES para guardar un registro de artículo
del inventario. Se declaran funciones no miembro para la entrada y salida de
registros por archivo y para definir el ordenamiento de objetos CILES. */
#ifndef CILES_HPP_INCLUDED
#define CILES_HPP_INCLUDED

#include <string>
#include <fstream>

namespace ITSON {

/* Un objeto CILES representa un registro de artículo. */
class CILES {
public:
    CILES();
    CILES(const std::string& clave, const std::string& campus,
          const std::string& almacen, const std::string& nombre,
          double cantidad, const std::string& unidad);

    /* Asigna una nueva cantidad.
       Arroja la excepción:
       invalid_argument - Si se intenta asignar una cantidad real a un registro
                          de unidad entera. */
    CILES& operator=(double cantidad);

    // Llaves de ordenamiento de mayor a menor.
    const std::string& clave() const { return clave_; }
    const std::string& campus() const { return campus_; }
    const std::string& almacen() const { return almacen_; }

    const std::string& nombre() const { return nombre_; }
    double cantidad() const { return cantidad_; }
    const std::string& unidad() const { return unidad_; }
private:
    std::string clave_;
    std::string campus_;
    std::string almacen_;

    std::string nombre_;
    double cantidad_;
    std::string unidad_;
};

/* Lee un registro de un archivo con una línea por registro. Los campos están
   separados por espacio(s) (como se define en isspace(char)) y se encuentran en
   el siguiente orden: clave campus almacen nombre cantidad unidad

   Ya que los registros se leen de archivo, la validación se hace aquí en lugar
   del constructor de CILES.

   La función falla al leer un registro si:
   -una línea no tiene suficientes campos.
   -se lee una unidad no definida.
   -se lee una cantidad real para una unidad que se contabiliza con enteros. */
std::ifstream& operator>>(std::ifstream& ifs, CILES& var);

std::ofstream& operator<<(std::ofstream& ofs, const CILES& var);

/* Función que define el ordenamiento de los objetos tipo CILES.
   Devuelve         si
    > 0         reg1 > reg2
     0          reg1 == reg2
    < 0         reg1 < reg2 */
int ciles_cmp(const CILES& reg1, const CILES& reg2);

}

#endif // CILES_HPP_INCLUDED
