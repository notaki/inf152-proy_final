#ifndef CILES_H
	#define CILES_H
	
	#include <fstream>
	
	enum unidades {Gr, pz = 100};
	
	typedef struct
	{
		char clave[8];		/* 1a Llave de ordenamiento */
		char campus[2];		/* 2a Llave de ordenamiento */
		char almacen[2];	/* 3a Llave de ordenamiento */
		char nombre[30];
		unidades unidad;
		union quantity {
			int Z;
			double R;
		} cantidad;
		int reg;			/* Número del registro en el */
	} CILES;				/* archivo ENTSAL donde se */
							/* encuentran las llaves */
	
	unidades aUnidad(char[] unidad);
	
	char aCadena(unidades unidad);
	
	int cilescmp(void* pInfo1, void* pInfo2);

	std::ifstream& operator>>(std::ifstream& ifs, CILES& registro);
	
	std::ofstream& operator<<(std::ofstream& ofs, CILES& registro);
#endif
