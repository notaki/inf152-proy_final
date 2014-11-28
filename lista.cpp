/*************************************************************
 * LISTA.C
 *
 * Este módulo implementa las operaciones de una lista ligada
 * generalizada, esto es, que opera con cualquier tipo de dato.
 * Las funciones que implementan las operaciones de la lista
 * reciben como parámetros datos o apuntadores de tipo LISTA y
 * NODO definidos como:
 *
 *	struct nodo
 *	{
 *		void pInfo;			// Campo de información
 *		struct nodo *pSig;	// Campo siguiente
 *	};
 *
 *	typedef struct nodo NODO;
 *	typedef struct nodo *LISTA;
 *
 * Aunque una variable y un apuntador de tipo LISTA declaradas
 * como:
 *
 *	LISTA lista;
 *	LISTA *pLista;
 *
 * podrían haberse declarado como:
 *
 *	NODO *lista;
 *	NODO **pLista;
 *
 * sin necesidad de crear el tipo LISTA, el definir el tipo
 * LISTA permite utilizar a LISTA par hacer referencia a la
 * lista y a NODO para hacer referencia a un nodo de la lista.
 * El uso del tipo LISTA simplifica las declaraciones como
 * la de NODO **plista a LISTA *pLista.
 *************************************************************/
 
#include <alloc.h>
#include <mem.h>

#include "lista.h"

/*************************************************************
 *void inicializarLista(LISTA *pLista)
 *
 * Esta función inicializa (vacía) una lista. pLista es un
 * apuntador al apuntador a la lista.
 *************************************************************/
void inicializarLista(LISTA *pLista)
{
	LISTA pTemp;
	
	/* Mientras haya nodos en la lista */
	while(*pLista)
	{
		pTemp = *pLista;
		*pLista = (*pLista)->pSig;
		destruyeNodo(pTemp);
	}
}

/*************************************************************
 * int listaVacia(LISTA lista)
 *
 * Esta función regresa un 1 si la lista está vacía, 0 en
 * caso contrario. lista es un apuntador a la lista.
 *************************************************************/
int listaVacia(LISTA lista)
{
	return lista == NULL;
}

/*************************************************************
 * void insertarLista(LISTA *pLista, NODO *pPos, NODO *pNodo)
 *
 * Esta función inserta el nodo apuntado por pNodo en la lista
 * apuntada por el apuntador pLista. pPos es un apuntador al
 * nodo después del cual se va insertar el nodo. Si se desea
 * insertar el dato al inicio de la lista pPos debe valer NULL.
 * La función no verifica que pPos sea una dirección de un
 * nodo de la lista.
 *************************************************************/
void insertarLista(LISTA *pLista, NODO *pPos, NODO *pNodo)
{
	/* Si la lista está vacía */
	if(listaVacia(*pLista))
	{
		*pLista = pNodo;
		return;
	}
	
	/* Si se va a insertar al inicio de la lista */
	if(!pPos)
	{
		pNodo->pSig = *pLista;
		*pLista = pNodo;
		return;
	}
	
	/* Si se va a insertar después del nodo pPos */
		pNodo->pSig = pPos->pSig;
		pPos->pSig = pNodo;
}

/*************************************************************
 * int extraerLista(LISTA *pLista, NODO *pPos, void *pDato,
 *				   int tamDato)
 *
 * Esta función extrae un nodo de la lista si no está vacía.
 * pPos es un apuntador al nodo después del cual esta el nodo
 * que se va extraer. Si se desea extraer el dato al inicio de
 * la lista pPos debe valer NULL. pDato es la localidad de
 * memoria en la que se almacena el campo de información del
 * nodo extraído. tamDato es el tamaño en bytes del campo de
 * información del nodo. La función no verifica que nPos sea
 * una dirección de un nodo de la lista.
 *************************************************************/
int extraerLista(LISTA *pLista, NODO *pPos, void *pDato, int tamDato)
{
	LISTA pTemp;
	
	/* Si la lista esta vacía */
	if(listaVacia(*pLista)) return 0;
	
	/* Si se va a extraer el primer elemento */
	if(!pPos)
	{
		pTemp = *pLista;
		*pLista = (*pLista)->pSig;
	}
	
	/* Si se va a extraer un elemento intermedio */
	else
	{
		pTemp = pPos->pSig;
		pPos->pSig = pTemp->pSig;
	}
	
	/* Extrae el dato */
	memcpy(pDato, pTemp->pInfo, tamDato);
	
	/* Libera el nodo */
	destruyeNodo(pTemp);
	return 1;
}

/*************************************************************
 * void visitarLista(LISTA lista, void (* fVisitar)(NODO *pNodo))
 *
 * Esta función recorre la lista dada por lista. En cada uno
 * de los nodos de la lista la función ejecuta la operación
 * dada por la función fVisitar(). La función fVisitar() es
 * suministrada por el Usuario y es de tipo void y recibe como
 * parámetro un apuntador a NODO
 *************************************************************/
void visitarLista(LISTA lista, void (* fVisitar)(NODO *pNodo))
{
	/* Mientras no se llegue al final de la lista */
	while(lista)
	{
		/* Opera sobre el nodo */
		fVisitar(lista);
		
		/* Va al siguiente nodo */
		lista = lista->pSig;
	}
}

/*************************************************************
 * NODO *buscaLista(LISTA lista, LISTA *pAnt, void *pLlave,
 *				   int (* fcmp)(void *pInfo, void *pLlave))
 *
 * Esta función busca en la lista dada por lista la primera
 * ocurrencia de un nodo cuyo campo de información al
 * compararse con llave cumpla la condición establecida por la
 * función fcmp(). La función regresa la dirección del nodo
 * que contiene esa primera ocurrencia, NULL en caso de no
 * encontrar un nodo que cumpla con la condición. pAnt apunta
 * al nodo anterior al nodo con la primera ocurrencia. Si el
 * nodo con la primera ocurrencia es el primer nodo de la
 * lista, pAnt apunta a NULL. fcmp es un apuntador a la función
 * utilizada para comparar la llave con los nodos de la lista.
 * La función para comparar es suministrada por el usuario y es
 * de tipo entero y recibe como parámetros dos apuntadores void
 * a los datos a comparar: pInfo y pLlave que corresponden al
 * campo de información y a la llave. La función fcmp() debe
 * regresar 0 si el campo de información y la llave cumplen con
 * la condición establecida por la función, diferente de cero
 * en caso contrario.
 *************************************************************/
NODO *buscaLista(LISTA lista, LISTA *pAnt, void *pLlave,
				 int (* fcmp)(void *pInfo, void *pLlave))
{
	*pAnt = NULL;
	
	/* Mientras no se llegue al final de la lista */
	while(lista)
	{
		/* Si la encontró */
		if(!fcmp(lista->pInfo, pLlave)) break;
		
		/* avanza al siguiente nodo */
		*pAnt = lista;
		lista = lista->pSig;
	}
	return lista;
}

/*************************************************************
 * NODO *creaNodo(void *pDato, int tamDato)
 *
 * Esta función crea un nodo haciendo una petición dinámica de
 * memoria e inicializa su campo de información con el valor
 * de info y el campo siguiente con un apuntador nulo.
 *************************************************************/
NODO *creaNodo(void *pDato, int tamDato)
{
	NODO *pNodo;
	
	/* Pide un bloque de memoria para el nodo, en forma dinámica */
	if((pNodo = (NODO*)malloc(sizeof(NODO))))
	{
		/* Pide un bloque de memoria para el dato, en forma dinámica */
		if((pNodo->pInfo = malloc(tamDato)))
			/* Almacena en el campo de información el dato */
			memcpy(pNodo->pInfo, pDato, tamDato);
		else return NULL;
		
		/* Almacena en el campo siguiente un apuntador nulo */
		pNodo->pSig = NULL;
	}
	return pNodo;
}

/*************************************************************
 * void destruyeNodo(NODO *pNodo)
 *
 * Esta función libera el bloque de memoria ocupada por el
 * nodo.
 *************************************************************/
void destruyeNodo(NODO *pNodo)
{
	/* Libera el bloque de memoria ocupada por el dato */
	free(pNodo->pInfo);
	
	/* Libera el bloque de memoria ocupada por el nodo */
	free(pNodo);
}
