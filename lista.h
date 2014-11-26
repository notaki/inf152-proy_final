#ifndef LISTA_H
	#define LISTA_H
	
	struct nodo
	{
		void *pInfo;
		struct nodo *pSig;
	};
	
	typedef struct nodo NODO;
	typedef struct nodo *LISTA;
	
	void inicializarLista(LISTA *pLista);
	int listaVacia(LISTA lista);
	void insertarLista(LISTA *pLista, NODO *pPos, NODO *pNodo);
	int extraerLista(LISTA *pLista, NODO *pPos, void *pDato, int tamDato);
	void visitarLista(LISTA lista, void (* fVisita)(NODO *pNodo));
	NODO *buscaLista(LISTA lista, LISTA *pAnt, void *pLlave,
					 int (* fcmp)(void *pInfo, void *pLlave));
	NODO *creaNodo(void *pDato, int tamDato);
	void destruyeNodo(NODO *pNodo);
#endif
