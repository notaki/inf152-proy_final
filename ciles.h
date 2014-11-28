#ifndef LISTA_ES_H
	#define LISTA_ES_H

	#include <fstream>

	typedef struct
	{
		char clave[8];		/* 1a Llave de ordenamiento */
		char campus[2];		/* 2a Llave de ordenamiento */
		char almacen[2];	/* 3a Llave de ordenamiento */
		char nombre[30];
		int cantidad;
		char unidad[4];
		int reg;			/* Número del registro en el */
	} CILES;				/* archivo ENTSAL donde se */
							/* encuentran las llaves */
	
	int cilescmp(void* pInfo1, void* pInfo2);

	std::ifstream& operator>>(std::ifstream& ifs, CILES& registro);
#endif
