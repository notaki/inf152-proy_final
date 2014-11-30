#include <fstream>
#include "ciles.h"
using namespace std;

ifstream& operator>>(ifstream& ifs, CILES& registro) {
	ifs >> registro.clave >> registro.campus >> registro.almacen
		>> registro.nombre >> registro.unidad;
	
	if (registro.unidad >= 100)
		ifs >> registro.cantidad.Z;
	else
		ifs >> registro.cantidad.R;
	
	return ifs;
}

ofstream& operator<<(ofstream& ofs, CILES& registro) {
	ofs << registro.clave << registro.campus << registro.almacen
		<< registro.nombre << registro.unidad;
	
	if (registro.unidad >= 100)
		ofs << registro.cantidad.Z;
	else
		ofs << registro.cantidad.R;
	
	return ofs << '\n';
}

int cilescmp(void* pInfo1, void* pInfo2) {
	CILES* reg1 = (CILES*)pInfo1;
	CILES* reg2 = (CILES*)pInfo2;

	if (reg1->clave != reg2->clave)
		return strcmp(reg1->clave,reg2->clave);

	if (reg1->campus != reg2->campus)
		return strcmp(reg1->campus,reg2->campus);

	return strcmp(reg1->almacen,reg2->almacen);
}
