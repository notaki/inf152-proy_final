/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 1)

En esta cabecera se definen las funciones miembro de clase Arbol_binario<T>. */
#ifndef ARBOL_IMPL_HPP_INCLUDED
#define ARBOL_IMPL_HPP_INCLUDED

#include <stdexcept>
#include <utility>

namespace ITSON {

template <class T>
Arbol_binario<T>::Arbol_binario()
                : dato{nullptr}, hoja_izq{nullptr}, hoja_der{nullptr}
{

}

template <class T>
Arbol_binario<T>::Arbol_binario(const T& dato)
                : dato{new T {dato}}, hoja_izq{nullptr}, hoja_der{nullptr}
{

}

template <class T>
Arbol_binario<T>::Arbol_binario(Arbol_binario&& tmp)
                : dato{tmp.dato}, hoja_izq{tmp.hoja_izq}, hoja_der{tmp.hoja_der}
{
    tmp.dato=nullptr;
    tmp.hoja_izq=tmp.hoja_der=nullptr;
}

template <class T>
Arbol_binario<T>::~Arbol_binario()
{
    // TBD
}

template <class T>
bool Arbol_binario<T>::vacio() const
{
    return dato==nullptr;
}

template <class T>
void Arbol_binario<T>::insertar(const T& dato, int (* fcmp)(const T&, const T&))
{
    /* Si la raíz no tiene dato, guárdalo. */
    if (this->dato==nullptr) {
        this->dato=new T {dato};
        return;
    }

    /* Si el dato es menor que el de esta hoja... */
    if ((fcmp(*(this->dato),dato))>0) {
        /* y no hay hoja izquierda, crea una con el dato. */
        if (hoja_izq==nullptr) hoja_izq=new Arbol_binario {dato};
        /* Sino, inserta en la hoja izquierda. */
        else hoja_izq->insertar(dato,fcmp);
        return;
    }
    /* Si el dato es mayor que el de esta hoja... */
    else {
        /* y no hay hoja derecha, crea una con el dato. */
        if (hoja_der==nullptr) hoja_der=new Arbol_binario {dato};
        /* Sino, vuelve a intentar con la hoja derecha. */
        else hoja_der->insertar(dato,fcmp);
        return;
    }
}

template <class T>
T& Arbol_binario<T>::extraer(const T& llave, int (* fcmp)(const T&, const T&))
{
    /* Si la raíz no tiene dato, no hay nada que extraer. */
    if (dato==nullptr)
        throw std::range_error {"extraer() de Arbol_binario::vacio()==true"};

    int dif;
    /* Si se encontró el dato... */
    if (!(dif = fcmp(*dato,llave))) {
        T dato {std::move(*(this->dato))};
        delete this->dato;
        this->dato=nullptr;

        /* y hay hoja derecha... */
        if (hoja_der!=nullptr) {
            /* y hay hoja izquierda... */
            if (hoja_izq!=nullptr) {
                Arbol_binario* tmp;
                /* busca la hoja más a la izquierda de la hoja derecha... */
                for (tmp=hoja_der; tmp->hoja_izq!=nullptr; tmp=tmp->hoja_izq);

                /* y conéctale la hoja izquierda de esta hoja... */
                tmp->hoja_izq=hoja_izq;
            }

            /* y haz esta hoja su hoja derecha. */
            this->dato=hoja_der->dato;
            this->hoja_izq=hoja_der->hoja_izq;
            this->hoja_der=hoja_der->hoja_der;
        }
        /* Si no hay hoja derecha y hay hoja izquierda... */
        else if (hoja_izq!=nullptr) {
            /* haz esta hoja su hoja izquierda. */
            this->dato=hoja_izq->dato;
            this->hoja_der=hoja_izq->hoja_der;
            this->hoja_izq=hoja_izq->hoja_izq;
        }
        /* Si no hay hoja izquierda ni derecha, haz esta nula. */
        else {
            this->dato=nullptr;
            this->hoja_der=this->hoja_izq=nullptr;
        }

        return dato;
    }

    /* Si la llave es menor que el dato de esta hoja, extrae por la izquierda. */
    if (dif>0 && hoja_izq!=nullptr) return hoja_izq->extraer(llave,fcmp);
    /* Si la llave es mayor que el dato de esta hoja, extrae por la derecha. */
    else if (dif<0 && hoja_der!=nullptr) return hoja_der->extraer(llave,fcmp);
    /* Si no hay más hojas, la llave no existe en el árbol. */
    throw std::range_error {"extraer(): llave no existe en Arbol_binario"};
}

template <class T>
T& Arbol_binario<T>::buscar(const T& llave, int (* fcmp)(const T&, const T&)) const
{
    /* Si la raíz no tiene dato, no hay nada que buscar. */
    if (dato == nullptr)
        throw std::range_error {"buscar() de Arbol_binario::vacio()==true"};

    int dif;
    /* Si se encontró el dato, devuélvelo. */
    if (!(dif = fcmp(*dato,llave))) return *dato;

    /* Si la llave es menor que el dato de esta hoja, busca por la izquierda. */
    if (dif>0 && hoja_izq!=nullptr) return hoja_izq->buscar(llave,fcmp);
    /* Si la llave es mayor que el dato de esta hoja, busca por la derecha. */
    else if (dif<0 && hoja_der!=nullptr) return hoja_der->buscar(llave,fcmp);
    /* Si no hay más hojas, la llave no existe en el árbol. */
    throw std::range_error {"buscar(): llave no existe en Arbol_binario"};
}

template <class T>
void Arbol_binario<T>::visitar_pre_orden(void (* faccion)(T&))
{
    if (dato!=nullptr) faccion(*dato);
    if (hoja_izq!=nullptr) hoja_izq->visitar_pre_orden(faccion);
    if (hoja_der!=nullptr) hoja_der->visitar_pre_orden(faccion);
}

template <class T>
void Arbol_binario<T>::visitar_pre_orden(void (* faccion)(const T&)) const
{
    if (dato!=nullptr) faccion(*dato);
    if (hoja_izq!=nullptr) hoja_izq->visitar_pre_orden(faccion);
    if (hoja_der!=nullptr) hoja_der->visitar_pre_orden(faccion);
}

template <class T>
void Arbol_binario<T>::visitar_en_orden(void (* faccion)(T&))
{
    if (hoja_izq!=nullptr) hoja_izq->visitar_en_orden(faccion);
    if (dato!=nullptr) faccion(*dato);
    if (hoja_der!=nullptr) hoja_der->visitar_en_orden(faccion);
}

template <class T>
void Arbol_binario<T>::visitar_en_orden(void (* faccion)(const T&)) const
{
    if (hoja_izq!=nullptr) hoja_izq->visitar_en_orden(faccion);
    if (dato!=nullptr) faccion(*dato);
    if (hoja_der!=nullptr) hoja_der->visitar_en_orden(faccion);
}

template <class T>
void Arbol_binario<T>::visitar_post_orden(void (* faccion)(T&))
{
    if (hoja_izq!=nullptr) hoja_izq->visitar_post_orden(faccion);
    if (hoja_der!=nullptr) hoja_der->visitar_post_orden(faccion);
    if (dato!=nullptr) faccion(*dato);
}

template <class T>
void Arbol_binario<T>::visitar_post_orden(void (* faccion)(const T&)) const
{
    if (hoja_izq!=nullptr) hoja_izq->visitar_post_orden(faccion);
    if (hoja_der!=nullptr) hoja_der->visitar_post_orden(faccion);
    if (dato!=nullptr) faccion(*dato);
}

}

#endif // ARBOL_IMPL_HPP_INCLUDED
