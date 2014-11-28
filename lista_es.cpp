#include <fstream>
#include "lista.h"
#include "lista_es.h"
using namespace std;

char archEntrada[] = "ENTRADAS.DAT";
char archSalida[] = "SALIDAS.DAT";

ifstream& operator>>(ifstream& ifs, CILES& registro) {
	ifs >> registro.clave;
	ifs >> registro.campus >> registro.almacen
		>> registro.nombre >> registro.cantidad >> registro.unidad;
	return ifs;
}

LISTA generaLista(char es) {
	if (es != 'E' && es != 'S') return NULL;
	
	ifstream ifs ((es == 'E') ? archEntrada : archSalida);
	
	LISTA lista = NULL;
	inicializarLista(&lista);
	
	NODO* anterior = NULL;
	CILES registro;
	int regNum = 1;
	while (ifs >> registro) {
		registro.reg = regNum++;
		CILES* tmpRegistro = new CILES;
		*tmpRegistro = registro;
		
		NODO* tmpNodo = creaNodo(tmpRegistro,sizeof(CILES));
		insertarLista(&lista,anterior,tmpNodo);
		
		anterior = tmpNodo;
	}
	
	return lista;
}
