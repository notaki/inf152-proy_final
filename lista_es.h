#ifndef LISTA_ES_H
	#define LISTA_ES_H

	typedef struct
	{
		char clave[8];		/* 1a Llave de ordenamiento */
		char campus[2];		/* 2a Llave de ordenamiento */
		char almacen[2];	/* 3a Llave de ordenamiento */
		int reg;			/* Número del registro en el */
	} CILES;				/* archivo ENTSAL donde se */
							/* encuentran las llaves */
	
	LISTA generaLista(char es);
#endif
