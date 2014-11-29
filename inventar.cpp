#include <fstream>
#include "inventar.h"
#include "ciles.h"
#include "lista.h"
#include "lista_es.h"
#include "arbol.h"
#include "arbolinv.h"
using namespace std;

char archivo[] = "INVENTAR.DAT";

void actualizaArchivo(HOJA *pHoja) {
	static ofstream ofs (archivo);
	
	ofs << (CILES*)pHoja->pInfo;
}

void actualizaInventario(char es) {
	if (es != 'E' && es != 'S') return;
	
	LISTA lista = generaLista(es);
	ARBOL inventario = generaArbol();
	
	CILES* registro;
	while (extraerLista(&lista,NULL,registro,sizeof(CILES))) {
		HOJA** hoja = buscaArbol(&inventario,registro,cilescmp);
		
		if (!hoja && es == 'E') {
			HOJA* tmpRegistro = creaHoja(registro,sizeof(CILES));
			
			inventario = insertarArbol(inventario,tmpRegistro,cilescmp);
			
			continue;
		}
		
		int* cantidadInv = &((CILES*)((*hoja)->pInfo))->cantidad;
		*cantidadInv += (es == 'E') ? registro->cantidad : -registro->cantidad; 
	}
	
	visitarArbolPreOrden(inventario,actualizaArchivo);
}
