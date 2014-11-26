#ifndef ARBOL_H
	#define ARBOL_H
	
	struct hoja
	{
		void *pInfo;		/* Campo de (apuntador) información	*/
		struct hoja *pIzq;	/* Apuntador al subárbol izquierdo	*/
		struct hoja *pDer;	/* Apuntador al subárbol derecho	*/
	};
	
	typedef struct hoja HOJA;	/* Alias de la estructura hoja */
	typedef struct hoja *ARBOL;	/* Alias del apuntador a la raíz del árbol */
	
	ARBOL inicializarArbol(ARBOL raiz);
	int arbolVacio(ARBOL raiz);
	ARBOL insertarArbol(ARBOL raiz, HOJA *pHoja,
						int (* fcmp)(void *pInfo, void *pLlave));
	int extraerArbol(ARBOL *pPos, void *pDato, int tamDato);
	void visitarArbolPreOrden(ARBOL raiz, void (* fVisita)(HOJA *pHoja));
	void visitarArbolEnOrden(ARBOL raiz, void (* fVisita)(HOJA *pHoja));
	void visitarArbolPostOrden(ARBOL raiz, void (* fVisita)(HOJA *pHoja));
	HOJA **buscaArbol(ARBOL *pRaiz, void *pLlave,
					  int (* fcmp)(void *pInfo, void *pLlave));
	HOJA *creaHoja(void *pInfo, int tamDato);
	void destruyeHoja(HOJA *pHoja);
#endif
