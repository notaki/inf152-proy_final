/*************************************************************
 * ARBOL.C
 *
 * Este módulo implementa las operaciones de un árbol binario
 * generalizado, esto es, que opera con cualquier tipo de dato.
 * Las funciones que implementan las operaciones del árbol
 * reciben como parámetros datos o apuntadores de tipo ARBOL y
 * HOJA definidos como:
 *
 *	struct hoja
 *	{
 *	void *pInfo;		Apuntador al campo de información
 *	struct hoja *pIzq;	Apuntador al subárbol izquierdo
 *	struct hoja *pIzq;	Apuntador al subárbol derecho
 *	};
 *
 *	typedef struct hoja HOJA;	Alias de la estructura hoja
 *	typedef struct hoja *ARBOL;	Alias del apuntador a la
 *								raíz del árbol
 *************************************************************/
 
#include <alloc.h>
#include <mem.h>
#include "arbol.h"

/*************************************************************
 * ARBOL inicializarArbol(ARBOL raiz)
 *
 * Esta función inicializa (vacía) un arbol. raíz es un
 * apuntador al árbol
 *************************************************************/
ARBOL inicializarArbol(ARBOL raiz)
{
	/* Si el árbol no está vacío */
	if(raiz)
	{
		/* Inicializa (vacía) el subárbol izquierdo */
		raiz->pIzq = inicializarArbol(raiz->pIzq);
		
		/* Inicializa (vacía) el subárbol izquierdo */
		raiz->pDer = inicializarArbol(raiz->pDer);
		
		/* Libera el nodo ocupado por la raíz */
		destruyeHoja(raiz);
	}
	
	/* Hace que el apuntador al árbol sea nulo */
	return NULL;
}

/*************************************************************
 *int arbolVacio(ARBOL raiz)
 *
 * Esta función regresa un 1 si el árbol está vacío, 0 en caso
 * contrario. raíz es un apuntador al árbol.
 *************************************************************/
int arbolVacio(ARBOL raiz)
{
	return raiz == NULL;
}

/*************************************************************
 * ARBOL insertarArbol(ARBOL raiz, HOJA *pHoja,
 *					  int (* fcmp)(void *pInfo1, void *pInfo2))
 *
 * Esta función inserta el nodo apuntado por pHoja en el árbol
 * apuntado por raíz. fcmp es un apuntador a la función
 * utilizada para comparar el nodo a insertar y la raíz del
 * árbol. La función para comparar es suministrada por el
 * usuario y es de tipo entero y recibe como parámetros
 * apuntadores a los campos de información comparar: pInfo1 e
 * pInfo2 que corresponden a los de la raíz y del nodo a
 * insertar, respectivamente. La función para comparar debe
 * regresar:
 *
 *	0	si info1 == info2
 *	(+)	si info1 > info2
 *	(-)	si info1 < info2
 *************************************************************/
ARBOL insertarArbol(ARBOL raiz, HOJA *pHoja,
					int (* fcmp)(void *pInfo1, void *pInfo2))
{
	/* Si el árbol no está vacío */
	if(raiz)
	{
		/* Si el nodo a insertar es menor que la raíz inserta en
		el subárbol izquierdo */
		if(fcmp(raiz->pInfo, pHoja->pInfo) > 0)
			raiz->pIzq = insertarArbol(raiz->pIzq, pHoja, fcmp);
			
		/* Si no inserta en el subárbol derecho */
		else raiz->pDer = insertarArbol(raiz->pDer, pHoja, fcmp);
		
		return(raiz);
	}
	return pHoja;
}

/*************************************************************
 * int extraerArbol(ARBOL *pPos, void *pDato, int tamDato)
 *
 * Esta función extrae un nodo del árbol si el nodo no está
 * vacío. pPos es un apuntador al nodo que se va extraer.
 * pDato es la localidad de memoria en la que se almacena el
 * campo de información del nodo extraído. La función no
 * verifica que pPos sea una dirección de un nodo del árbol.
 *************************************************************/
int extraerArbol(ARBOL *pPos, void *pDato, int tamDato)
{
	HOJA *pTemp;
	
	/* Si el nodo está vacío */
	if(!(*pPos)) return 0;
	
	/* Si el subárbol derecho del nodo a extraer está vacío */
	if(!(*pPos)->pDer)
	{
		pTemp = *pPos;
		
		/* Conecta el subárbol izquierdo del nodo a extraer */
		*pPos = (*pPos)->pIzq;
	}
	
	/* Si el subárbol izquierdo del nodo a extraer está vacío */
	else if(!(*pPos)->pIzq)
	{
		pTemp = *pPos;
		
		/* Conecta el subárbol derecho del nodo a extraer */
		*pPos = (*pPos)->pDer;
	}
	
	/* Si ninguno de los subárboles del nodo a extraer está vacío */
	else
	{
		/* Encuentra la hoja más a la izquierda del subárbol derecho */
		for(pTemp = (*pPos)->pDer; pTemp->pIzq;
			pTemp = pTemp->pIzq);
			
		/* Conecta el subárbol izquierdo del nodo a extraer a la
		   hoja más a la izquierda del subárbol derecho */
		pTemp->pIzq = (*pPos)->pIzq;
		pTemp = *pPos;
		
		/* Conecta el subárbol derecho del nodo a extraer */
		*pPos = (*pPos)->pDer;
	}
	
	/* Extrae el dato */
	memcpy(pDato, pTemp->pInfo, tamDato);
	
	/* Libera el nodo */
	destruyeHoja(pTemp);
	return 1;
}

/*************************************************************
 * void visitarArbolPreOrden(ARBOL raiz,
 *							 void (* fVisita)(HOJA *pHoja))
 *
 * Esta función recorre el árbol binario dado por raíz en
 * preorden, esto es, primero la raíz, luego el subárbol
 * izquierdo y por último el subárbol derecho. En cada uno de
 * los nodos del árbol la función ejecuta la operación dada
 * por la función fVisita() proporcionada por el usuario.
 *************************************************************/
void visitarArbolPreOrden(ARBOL raiz, void (* fVisita)(HOJA *pHoja))
{
	/* Si el árbol no está vacío */
	if(raiz)
	{
		/* Visita la raíz */
		fVisita(raiz);
		
		/* Visita el subárbol izquierdo */
		visitarArbolPreOrden(raiz->pIzq, fVisita);
		
		/* Visita el subárbol derecho */
		visitarArbolPreOrden(raiz->pDer, fVisita);
	}
}

/*************************************************************
 * void visitarArbolEnOrden(ARBOL raiz,
 *						   void (* fVisita)(HOJA *pHoja))
 *
 * Esta función recorre el árbol binario dado por raíz en
 * orden, esto es, primero el subárbol izquierdo, luego la
 * raíz y por último el subárbol derecho. En cada uno de los
 * nodos del árbol la función ejecuta la operación dada por
 * función fVisita() proporcionada por el usuario.
 *************************************************************/
void visitarArbolEnOrden(ARBOL raiz, void (* fVisita)(HOJA *pHoja))
{
	/* Si el árbol no está vacío */
	if(raiz)
	{
		/* Visita el subárbol izquierdo */
		visitarArbolEnOrden(raiz->pIzq, fVisita);
		
		/* Visita la raíz */
		fVisita(raiz);
		
		/* Visita el subárbol derecho */
		visitarArbolEnOrden(raiz->pDer, fVisita);
	}
}

/*************************************************************
 * void visitarArbolPostOrden(ARBOL raiz,
 *							 void (* fVisita)(HOJA *pHoja))
 *
 * Esta función recorre el árbol binario dado por raíz en
 * orden, esto es, primero el subárbol izquierdo, luego el
 * subárbol derecho y por último la raíz. En cada uno de los
 * nodos del árbol la función ejecuta la operación dada por
 * la función fVisita() proporcionada por el usuario.
 *************************************************************/
void visitarArbolPostOrden(ARBOL raiz, void (* fVisita)(HOJA *pHoja))
{
	/* Si el árbol está vacío */
	if(raiz)
	{
		/* Visita el subárbol izquierdo */
		visitarArbolPostOrden(raiz->pIzq, fVisita);
		
		/* Visita el subárbol derecho */
		visitarArbolPostOrden(raiz->pDer, fVisita);
		
		/* Visita la raíz */
		fVisita(raiz);
	}
}

/*************************************************************
 * HOJA **buscaArbol(ARBOL *pRaiz, void *pLlave,
 *					int (* fcmp)(void *pInfo, void *pLlave))
 *
 * Esta función busca en el árbol cuya dirección está
 * almacenada en la localidad de memoria dada por pRaiz el nodo
 * cuyo campo de información tenga una llave igual a la llave
 * apuntada por pLlave. La función regresa la dirección de la
 * localidad de memoria donde está almacenada la dirección del
 * nodo buscado, NULL en caso de no encontrar un nodo. fcmp()
 * es un apuntador a la función utilizada para comparar la
 * llave con los nodos de la lista. La función para comparar es
 * suministrada por el usuario y es de tipo entero y recibe
 * como parámetros apuntadores a los datos a comparar: pinfo y
 * pLlave que corresponden al campo de información y a la llave.
 * La función fcmp() debe regresar:
 *
 *	0	si campo de información == llave
 *	(+)	si campo de información > llave
 *	(-)	si campo de información < llave
 *************************************************************/
HOJA **buscaArbol(ARBOL *pRaiz, void *pLlave,
				  int (* fcmp)(void *pInfo, void *pLlave))
{
	int rc;
	HOJA **ppPos = pRaiz;
	
	/* Si el árbol no está vacío */
	if(*pRaiz)
	{
		/* Compara llave con nodo */
		rc = fcmp((*pRaiz)->pInfo, pLlave);
		
		/* Si no es igual, busca en el subárbol izquierdo */
		if(rc > 0) ppPos = buscaArbol(&((*pRaiz)->pIzq), pLlave, fcmp);
		
		/* si no está, busca el subárbol derecho */
		else if(rc < 0) ppPos = buscaArbol(&((*pRaiz)->pDer),
										   pLlave, fcmp);
	}
	return ppPos;
}

/*************************************************************
 * HOJA *creaHoja(void *pDato, int tamDato)
 *
 * Esta función crea un nodo de un árbol haciendo una petición
 * dinámica de memoria e inicializa su campo de información
 * con el valor de info y los apuntadores a los subárboles
 * izquierdo y derecho con apuntadores nulos.
 *************************************************************/
HOJA *creaHoja(void *pDato, int tamDato)
{
	HOJA *pHoja;
	
	/* Pide un bloque de memoria para el nodo, en forma dinámica */
	if(pHoja = malloc(sizeof(HOJA)))
	{
		/* Pide un bloque de memoria para el dato, en forma dinámica */
		if((pHoja->pInfo = malloc(tamDato)))
			/* Almacena en el campo de información el dato */
			memcpy(pHoja->pInfo, pDato, tamDato);
		else return NULL;
		
		/* Almacena en los apuntadores a los subárboles
		   izquierdo y derecho apuntadores nulos. */
		pHoja->pIzq = pHoja->pDer = NULL;
	}
	return pHoja;
}

/*************************************************************
 * void destruyeHoja(HOJA *pHoja)
 *
 * Esta función libera el bloque de memoria ocupada por el
 * nodo de un árbol.
 *************************************************************/
void destruyeHoja(HOJA *pHoja)
{
	free(pHoja->pInfo);
	free(pHoja);
}
