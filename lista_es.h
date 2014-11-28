#ifndef LISTA_ES_H
	#define LISTA_ES_H

	typedef struct
	{
		char clave[8];		/* 1a Llave de ordenamiento */
		char campus[2];		/* 2a Llave de ordenamiento */
		char almacen[2];	/* 3a Llave de ordenamiento */
		int reg;			/* Número del registro en el */
		char nombre[30];	/* archivo ENTSAL donde se */
		int cantidad;		/* encuentran las llaves */
		char unidad[4];
	} CILES;
	
	LISTA generaLista(char es);
#endif
