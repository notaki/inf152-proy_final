#include "arbol.h"
#include "arbolinv.h"
#include "ciles.h"
using namespace std;

char archivo[] = "INVENTAR.DAT";

ARBOL generaArbol(void) {
	ifstream ifs (archivo);
	
	ARBOL arbol = NULL;
	inicializarArbol(arbol);
	
	CILES registro;
	int regNum = 0;
	while (ifs >> registro) {
		registro.reg = ++regNum;
		CILES* tmpRegistro = new CILES;
		*tmpRegistro = registro;
		
		HOJA* tmpHoja = creaHoja(tmpRegistro,sizeof(CILES));
		arbol = insertarArbol(arbol,tmpHoja,cilescmp);
	}
	
	return arbol;
}
