/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 1)

En esta cabecera se definen las funciones miembro de la clase Arbol_binario. */
#ifndef ARBOL_IMPL_HPP_INCLUDED
#define ARBOL_IMPL_HPP_INCLUDED

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
bool Arbol_binario<T>::vacio()
{
    return dato == nullptr;
}

template <class T>
void Arbol_binario<T>::inserta(const T& dato, int (* fcmp)(const T&, const T&))
{
    /* Si la raíz no tiene dato, guárdalo. */
    if (this->dato == nullptr) {
        this->dato = new T {dato};
        return;
    }

    /* Si el dato es menor que el de esta hoja... */
    if ((fcmp(*(this->dato),dato)) > 0) {
        /* y no hay hoja izquierda, crea una con el dato. */
        if (hoja_izq == nullptr) {
            hoja_izq = new Arbol_binario {dato};
            return;
        }
        /* Sino, vuelve a intentar con la hoja izquierda. */
        hoja_izq->inserta(dato,fcmp);
        return;
    }
    /* Si el dato es mayor que el de esta hoja... */
    else {
        /* y no hay hoja derecha, crea una con el dato. */
        if (hoja_der == nullptr) {
            hoja_der = new Arbol_binario {dato};
            return;
        }
        /* Sino, vuelve a intentar con la hoja derecha. */
        hoja_der->inserta(dato,fcmp);
        return;
    }
}

template <class T>
T& Arbol_binario<T>::extrae(const T& llave, int (* fcmp)(const T&, const T&))
{
    /* Si la raíz no tiene dato, no hay nada que extraer. */
    if (dato == nullptr) throw throwable::Operacion_fallida { };

    int dif;
    /* Si se encontró el dato... */
    if (!(dif = fcmp(*dato,llave))) {
        Arbol_binario* tmp;
        T dato = *(this->dato);

        /* y hay hoja derecha... */
        if (hoja_der != nullptr) {
            /* y hay hoja izquierda... */
            if (hoja_izq != nullptr) {
                /* busca la hoja más a la izquierda de la hoja derecha... */
                for (tmp = hoja_der; tmp->hoja_izq != nullptr; tmp = tmp->hoja_izq);

                /* y conéctale la hoja izquierda de esta hoja. */
                tmp->hoja_izq = hoja_izq;
            }

            /* Sustituye esta hoja con su hoja derecha. */
            this->dato = hoja_der->dato;
            this->hoja_izq = hoja_der->hoja_izq;
            this->hoja_der = hoja_der->hoja_der;
        }
        /* Si no hay hoja por la derecha... */
        else {
            /* y hay hoja izquierda... */
            if (hoja_izq != nullptr) {
                /* Sustituye esta hoja con su hoja izquierda. */
                this->dato = hoja_izq->dato;
                this->hoja_der = hoja_izq->hoja_der;
                this->hoja_izq = hoja_izq->hoja_izq;
            }
            /* Sino hay hojas, hazla nula. */
            else {
                this->dato = nullptr;
                this->hoja_izq = this->hoja_der = nullptr;
            }
        }

        return dato;
    }

    /* Si la llave es menor que el dato de esta hoja, busca por la izquierda. */
    if (dif > 0 && hoja_izq != nullptr) return hoja_izq->extrae(llave,fcmp);
    /* Si la llave es mayor que el dato de esta hoja, busca por la derecha. */
    else if (dif < 0 && hoja_der != nullptr) return hoja_der->extrae(llave,fcmp);
    /* Si no hay más hojas, la llave no existe en el árbol. */
    throw throwable::Operacion_fallida { };
}

template <class T>
T& Arbol_binario<T>::busca(const T& llave, int (* fcmp)(const T&, const T&)) const
{
    /* Si la raíz no tiene dato, no hay nada que buscar. */
    if (dato == nullptr) throw throwable::Operacion_fallida { };

    int dif;
    /* Si se encontró el dato, devuélvelo. */
    if (!(dif = fcmp(*dato,llave))) return *dato;

    /* Si la llave es menor que el dato de esta hoja, busca por la izquierda. */
    if (dif > 0 && hoja_izq != nullptr) return hoja_izq->busca(llave,fcmp);
    /* Si la llave es mayor que el dato de esta hoja, busca por la derecha. */
    else if (dif < 0 && hoja_der != nullptr) return hoja_der->busca(llave,fcmp);
    /* Si no hay más hojas, la llave no existe en el árbol. */
    throw throwable::Operacion_fallida { };
}

template <class T>
void Arbol_binario<T>::visitar_pre_orden(void (* faccion)(T&))
{
    faccion(*dato);
    if (hoja_izq != nullptr) hoja_izq->visitar_pre_orden(faccion);
    if (hoja_der != nullptr) hoja_der->visitar_pre_orden(faccion);
}

template <class T>
void Arbol_binario<T>::visitar_pre_orden(void (* faccion)(const T&)) const
{
    faccion(*dato);
    if (hoja_izq != nullptr) hoja_izq->visitar_pre_orden(faccion);
    if (hoja_der != nullptr) hoja_der->visitar_pre_orden(faccion);
}

template <class T>
void Arbol_binario<T>::visitar_en_orden(void (* faccion)(T&))
{
    if (hoja_izq != nullptr) hoja_izq->visitar_en_orden(faccion);
    faccion(*dato);
    if (hoja_der != nullptr) hoja_der->visitar_en_orden(faccion);
}

template <class T>
void Arbol_binario<T>::visitar_en_orden(void (* faccion)(const T&)) const
{
    if (hoja_izq != nullptr) hoja_izq->visitar_en_orden(faccion);
    faccion(*dato);
    if (hoja_der != nullptr) hoja_der->visitar_en_orden(faccion);
}

template <class T>
void Arbol_binario<T>::visitar_post_orden(void (* faccion)(T&))
{
    if (hoja_izq != nullptr) hoja_izq->visitar_post_orden(faccion);
    if (hoja_der != nullptr) hoja_der->visitar_post_orden(faccion);
    faccion(*dato);
}

template <class T>
void Arbol_binario<T>::visitar_post_orden(void (* faccion)(const T&)) const
{
    if (hoja_izq != nullptr) hoja_izq->visitar_post_orden(faccion);
    if (hoja_der != nullptr) hoja_der->visitar_post_orden(faccion);
    faccion(*dato);
}

}

#endif // ARBOL_IMPL_HPP_INCLUDED
