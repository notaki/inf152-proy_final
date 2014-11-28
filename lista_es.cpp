#include <fstream>
#include "lista.h"
#include "lista_es.h"
using namespace std;

char archEntrada[] = "ENTRADAS.DAT";
char archSalida[] = "SALIDAS.DAT";

LISTA generaLista(char es) {
	if (es != 'E' && es != 'S') return NULL;
	
	ifstream ifs ((es == 'E') ? archEntrada : archSalida);
	
	LISTA lista = NULL;
	inicializarLista(&lista);
	
	NODO* anterior = NULL;
	CILES registro;
	while (ifs >> registro.clave >> registro.campus >> registro.almacen) {
		CILES* tmpRegistro = new CILES;
		*tmpRegistro = registro;
		
		NODO* tmpNodo = creaNodo(tmpRegistro,sizeof(CILES));
		insertarLista(&lista,anterior,tmpNodo);
		
		anterior = tmpNodo;
	}
	
	return lista;
}
