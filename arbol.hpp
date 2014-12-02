/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 1)

En esta cabecera se declara la plantilla de clase Arbol_binario. El usuario
opera sobre el árbol con el objeto que crea, la raíz. El manejo de las hojas se
realiza de forma interna. Es necesario que la clase usada para instanciar un
Arbol_binario tenga un constructor vacío. */
#ifndef ARBOL_HPP_INCLUDED
#define ARBOL_HPP_INCLUDED

namespace ITSON {

template <class T>
class Arbol_binario {
public:
    Arbol_binario();

    bool vacio();

    /* Las funciones inserta(), extrae() y busca() reciben como segundo
       parámetro un puntero a función int que recibe dos parámetros, arg1 y arg2
       respectivamente, del tipo de elemento del árbol. Esta función compara los
       elementos para poder definir su ordenamiento. La función debe devolver:
       > 0  si  arg1 > arg2
        0   si  arg1 == arg2
       < 0  si  arg1 < arg2 */

    void inserta(const T& dato, int (* fcmp)(const T&, const T&));
    // Para extrae() y busca():
    /* El parámetro llave se usa para comparar con los elementos del árbol y
       devolver el que sea igual a la llave. Si no se encuentra dicho elemento o
       el árbol está vacío, la función causa comportamiento indefinido. */
    T extrae(const T& llave, int (* fcmp)(const T&, const T&));
    T busca(const T& llave, int (* fcmp)(const T&, const T&)) const;

    /* Las funciones visitar_*_orden reciben como parámetro un puntero a
       función void que recibe un parámetro del tipo de elemento del árbol. */
    void visitar_pre_orden(void (* fvisita)(T&));
    void visitar_pre_orden(void (* fvisita)(const T&)) const;
    void visitar_en_orden(void (* fvisita)(T&));
    void visitar_en_orden(void (* fvisita)(const T&)) const;
    void visitar_post_orden(void (* fvisita)(T&));
    void visitar_post_orden(void (* fvisita)(const T&)) const;
private:
    Arbol_binario(const T& dato);

    T* dato;
    Arbol_binario* hoja_izq;
    Arbol_binario* hoja_der;
};

}

#include "arbol_impl.hpp"

#endif // ARBOL_HPP_INCLUDED