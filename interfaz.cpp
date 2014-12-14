/* Johel Guerrero
   Tercer cuatrimestre 2014 (Dic. 5)

Las impresiones a pantalla son horizontales y de izquierda a derecha al menos
que se especifique lo contrario.

Abreviaturas/convensión de identificadores:

impr - imprimir
hor - horizontal
ver - vertical
msj - mensaje
scr - pantalla (screen)
(y^x)(i^f^id)(d) - ordenada^abscisa inicial^final^identificador por_defecto
Ejemplo: yid - ordenada inicial por defecto, xf - abscisa final

NOTA: Donde se usan las coordenadas de nombre (yi,xi) y (yf,xf) se espera que
estas puedan formar un rectángulo con esquina superior izquierda (yi,xi) y
esquina inferior derecha (yf,xf). De no ser así, la función no hace nada.

El espacio usado por la interfaz es de (0,0) a (LINES-1,COLS-1). Este espacio es
bordado, por lo que el uso de una ordenada de valor yid o una abscisa de valor
xid es para evitar sobreescribir los bordes, igual que con las funciones que
reciben LINES-1 y/o COLS-1 (que imprimen hasta ese valor, exclusivo).

Posible bug de ncurses: Imprimir a pantalla caracteres no ASCII o chtype
declarados en ncurses.h causa pérdidas de posiciones en la interfaz.

TODO:
-Centralizar algunos campos de registros.
-Espacio entre los campos mediante un string global y no el hardcoded char ' '.
-Que el gui de registros de lista diga si es de entradas o salidas.
-Funciones con constantes mágicas por eliminar:
 -impr_lista()
-Mejorar las siguientes funciones en sentido general:
 -impr_lista() */
#include <string>
#include <vector>
#include "interfaz.hpp"

namespace ITSON {

namespace GUI {

void impr_bordes();

void inicializar()
{
    initscr(); // ncurses se inicializa.

    curs_set(0);          // No dibujes el puntero de la terminal.
    leaveok(stdscr,TRUE); // No actualices el puntero de la terminal.
    keypad(stdscr,TRUE);  // Activa input desde el numpad.
    nodelay(stdscr,FALSE);// Espera por el input del usuario.
    noecho();             // No escribas a pantalla los caracteres introducidos.
    cbreak();             // Recibe input sin esperar por retorno de línea.

    impr_bordes();
}

void finalizar()
{
    endwin(); // ncurses libera sus recursos y finaliza.
}

/* Imprime en la línea y el caracter ch a las columnas [xi,xf). */
void limpiar(int y, int xi, int xf, chtype ch=' ')
{
    move(y,xi);
    for (int i=xi; i<xf; ++i) addch(ch);
}

void (* const impr_char)(int,int,int,chtype)=limpiar;

/* Representan el espacio disponible para imprimir los registros menús (excluye
   la cabecera, el título y el pie del GUI). */
const int yid=8;
const int xid=1;
int yfd() { return LINES-3; }
int xfd() { return COLS-1; }

void limpiar_scr(int yi=yid, int xi=xid, int yf=yfd(), int xf=xfd())
{
    for (int y=yi; y<yf; ++y) limpiar(y,xi,xf);
}

/* Imprime verticalmente y de arriba hacia abajo en las columnas x1 y x2 el
   caracter ch a las filas [yi,yf). */
void impr_char_ver(int yi, int x1, int x2, int yf, chtype ch)
{
    for (int i=yi; i<yf; ++i) {
        mvaddch(i,x1,ch);
        mvaddch(i,x2,ch);
    }
}

/* Imprime un string que comienza en (y,x). */
void impr(int y, int x, const std::string& str)
{
    move(y,x);
    printw("%s",str.c_str());
}

/* Imprime el título de la pantalla actual. */
void impr_titulo(const std::string& titulo)
{
    const int ytitulod=6; // Línea para el título.
    limpiar(ytitulod,1,COLS-2);
    impr(ytitulod,COLS/2-titulo.size()/2,titulo);
}

int yipied() { return LINES-3; } // Línea donde inicia el espacio para el pie.

const std::string msj_pie_menu="para cambiar opcion, [Enter] para seleccionar.";
const std::string msj_pie_registros="'q' para salir, [Enter] para continuar.";

/* Imprime el pie del GUI. */
void impr_pie(const std::string& msj=msj_pie_menu)
{
    const int ypied=yipied()+1; // Línea para el mensaje de pie.
    limpiar(ypied,xid,COLS-1);

    const int ancho_flechas=3; // Ancho de las flechas junto a msj_pie_menu.
    move(ypied,COLS/2-msj.size()/2-((msj==msj_pie_menu) ? ancho_flechas : 0));
    if (msj==msj_pie_menu) {
        addch(ACS_UARROW);
        addch(ACS_DARROW);
        addch(' ');
    }
    printw("%s",msj.c_str());
}

void reimpr_gui(const std::string& msj_pie, void (* impr_scr)()=nullptr)
{
    erase(); // Borra toda la pantalla
    impr_bordes();
    impr_pie(msj_pie);
    if (impr_scr!=nullptr) impr_scr();
}

/* Reimprime la interfaz y actualiza el puntero. Devuelve lo que devuelve
   impr_menu(). */
std::pair<int,int> reimpr_gui(int opc, std::pair<int,int> (* impr_menu)(),
                              const std::string& msj_pie=msj_pie_menu)
{
    reimpr_gui(msj_pie);
    auto p=impr_menu();
    mvaddch(p.first+1,p.second+1,' ');
    return p;
}

/* Controla el input de los menús, permitiendo desplazarse y escoger entre las
   opciones.
   Devuelve un objeto Opc en el rango min_opc<=Opc<=max_opc igual a la opción
   escogida.
   Las opciones disponibles se imprimen mediante el puntero a función gui_menu,
   que devuelve la esquina superior izquierda donde inicia la ventana del menú
   usado para manejar el puntero que destaca la opción seleccionada. */
Opc manejar_menu(std::pair<int,int> (* gui_menu)(), int min_opc, int max_opc)
{
    std::pair<int,int> p {gui_menu()};
    int y=p.first+1;
    int x=p.second+1;

    /* Se inicia el puntero en la primera opción (impreso por gui_menu). */
    int opc=min_opc;
    /* Se devuelve la opción escogida al presionar Enter. */
    for (auto in=getch(); in!='\n'; in=getch()) {
        /* Si se desplaza hacia arriba o hacia abajo (presionando la flecha
           correspondiente), actualiza la opción escogida y el puntero. */
        mvaddch(y,x,' '); // Borra el puntero.
        switch (in) {
        case KEY_UP:
            (opc!=min_opc) ? --opc, --y : (y+=max_opc-min_opc, opc=max_opc);
            break;
        case KEY_DOWN:
            (opc!=max_opc) ? ++opc, ++y : (y-=max_opc-min_opc, opc=min_opc);
            break;
        /* Si cambia el tamaño de la terminal... */
        case KEY_RESIZE:
            /* reimprime la interfaz y actualiza las coordenadas del puntero. */
            p=reimpr_gui(opc,gui_menu);
            y=p.first+1+opc;
            x=p.second+1;
            break;
        }
        mvaddch(y,x,ACS_RARROW); // Imprime el puntero.
    }

    return Opc(opc);
}

void gui_registros();
void impr_lista(const std::forward_list<ITSON::CILES>& lista);
void impr_inventario(const ITSON::Arbol_binario<ITSON::CILES>& inventario);

void manejar_registros(const ITSON::Arbol_binario<ITSON::CILES>& inventario)
{
    gui_registros();

    impr_inventario(inventario);
}

void manejar_registros(const std::forward_list<ITSON::CILES>& lista)
{
    gui_registros();

    impr_lista(lista);

    mvaddch(yid+1,0,ACS_VLINE);
    mvaddch(yid+1,COLS-1,ACS_VLINE);
}

/* Imprime las esquinas de los bordes. */
void impr_esquinas(int ysi, int xsi, int yii, int xsd)
{
    mvaddch(ysi,xsi,ACS_ULCORNER); mvaddch(ysi,xsd,ACS_URCORNER);
    mvaddch(yii,xsi,ACS_LLCORNER); mvaddch(yii,xsd,ACS_LRCORNER);
}

/* Imprime los bordes y las opciones de un menú. */
std::pair<int,int> impr_menu(const std::vector<std::string>& opciones)
{
    /* Consigue el ancho máximo del menú. */
    int max_ancho=0;
    for (auto s : opciones) if (s.size()>max_ancho) max_ancho=s.size();
    const int num_bordes=2; /* Número de bordes que rodea las opciones del menú
                               a lo ancho o a lo largo. */
    const int ancho_borde=1;
    const int ancho_puntero=1;
    max_ancho += num_bordes*ancho_borde+ancho_puntero;

    /* Esquina superior izquierda del menú. */
    int y=LINES/2-opciones.size()/2;
    int x=COLS/2-(max_ancho)/2;

    /* Imprime los bordes del menú. */
    std::vector<int> ordenadas {y,y+1+opciones.size()};
    for (int y : ordenadas) impr_char(y,x+1,x+1+max_ancho-num_bordes,ACS_HLINE);
    impr_char_ver(y+1,x,x+max_ancho-1,y+ancho_borde+opciones.size(),ACS_VLINE);
    for (int i=0; i<opciones.size(); ++i)
        impr(y+ancho_borde+i,x+ancho_borde+ancho_puntero,opciones[i]);
    impr_esquinas(y,x,y+ancho_borde+opciones.size(),x+max_ancho-1);

    mvaddch(y+1,x+1,ACS_RARROW); // Imprime el puntero.

    return std::make_pair(y,x);
}

/* Borra de num los ceros a la derecha hasta el punto decimal u otro dígito. */
std::string formatear(double num)
{
    const std::string str_num {std::to_string(num)};
    auto ultimo_punto=str_num.find('.');
    auto ultimo_no_cero=str_num.find_last_not_of(".0");
    auto ultimo=(ultimo_punto<ultimo_no_cero) ? ultimo_no_cero+1 : ultimo_punto;
    return str_num.substr(0,ultimo);
}

/* Guarda los nombres de los campos de los registros de inventario. Un campo se
   imprime con un máximo de caracteres igual a los caracteres del nombre de su
   campo, a excepción del nombre, que ocupa todo el espacio libre. */
const std::vector<std::string> nombre_de_campo {"Reg ","Clave  ","Nombre",
                                        "Campus","Almacen","Cantidad","Unidad"};
const std::string& campo_de_nombre=nombre_de_campo[2];
std::vector<int> xicampos; // Guarda las abscisas donde se imprime cada campo.

int tam_max_nombre()
{
    int espacio_usado=xid+1;
    for (auto str : nombre_de_campo) espacio_usado += str.size()+1;
    return COLS-1-espacio_usado; // Espacio libre, todo para el nombre.
}

int& num_registro() { static int num_reg; return num_reg; }//Rushed code
int& linea() { static int linea=0; return linea; }

void impr_registro(const ITSON::CILES& registro)
{
    std::vector<std::string> campo {registro.clave(),registro.campus(),
           registro.almacen(),formatear(registro.cantidad()),registro.unidad()};
    std::string nombre=registro.nombre();

    /* Agrega una elipsis a los campos más largos que su nombre de campo. */
    std::string sust="..."; // Lo que sustituirá el final de los campos largos.
    std::vector<int> indice_campo {1,3,4,5,6}; // campo en nombre_de_campo.
    for (int i=0; i<campo.size(); ++i) {
        int tam_max_campo=nombre_de_campo[indice_campo[i]].size();
        if (campo[i].size()>tam_max_campo)
            campo[i]=campo[i].substr(0,tam_max_campo-sust.size())+sust;
    }
    int tam_max_nombre_=campo_de_nombre.size()+tam_max_nombre();
    if (nombre.size()>tam_max_nombre_)
        nombre=nombre.substr(0,tam_max_nombre_-sust.size())+sust;

    /* Imprime los campos. */
    for (int i=0, j=0; i<xicampos.size(); ++i) {
        move(linea(),xicampos[i]);
        if (i==0) printw("%d ",num_registro()++); // Número del registro.//Rushed code
        else if (i==2) printw("%s ",nombre.c_str());
        else printw("%s ",campo[j++].c_str());
    }
}

/* Imprime y maneja la ventana con los registros. */
void impr_lista(const std::forward_list<ITSON::CILES>& lista)
{
    num_registro()=1;//Rushed code
    int primer_registro=num_registro();//Rushed code
    auto primero_impreso=lista.begin();
    for (auto it=primero_impreso; it!=lista.end(); ) {
        primero_impreso=it;
        primer_registro=num_registro();//Rushed code
        /* Ajusta la primera y última línea para imprimir los registros. */
        int y=linea()=yid+2;
        int max_y=yipied();
        /* Mientras haya elementos y no se llegue a la última línea, imprime. */
        for ( ; it!=lista.end() && linea()<max_y; ++it, ++linea())
            impr_registro(*it);

        chtype in;
        /* Si se recibe un Enter, imprime la siguiente página de elementos si la
           hay. Si se recibe 'q', vuelve al menú anterior. */
        for (bool in_esperado=false; !in_esperado ; ) {
            in=getch();
            switch (in) {
            case '\n':
                limpiar_scr(yid+2);
                in_esperado=true;
                break;
            case 'q': return;
            case KEY_RESIZE:
                reimpr_gui(msj_pie_registros,gui_registros);
                it=primero_impreso;
                num_registro()=primer_registro;//Rushed code
                in_esperado=true;
                break;
            }
        }
    }
}

//******************************************************************************
// Rushed code starts here
//******************************************************************************

std::forward_list<ITSON::CILES>& lista()
{
    static std::forward_list<ITSON::CILES> lista;
    return lista;
}

void a_lista(const ITSON::CILES& registro)
{
    lista().push_front(registro);
}

void impr_inventario(const ITSON::Arbol_binario<ITSON::CILES>& inventario)
{
    lista().clear();
    inventario.visitar_en_orden(a_lista);
    lista().reverse();
    impr_lista(lista());

    mvaddch(yid+1,0,ACS_VLINE);
    mvaddch(yid+1,COLS-1,ACS_VLINE);
}

void manejar_claves(const std::vector<std::string>& claves)
{
    limpiar_scr();
    impr_pie("Presione [Enter] para continuar.");
    int x=xid+1;
    int y=yid;
    for (auto str : claves) {
        if (x+str.size()>COLS-2) {
            x=xid+1;
            ++y;
        }
        if (y>yfd()) break;
        move(y,x);
        printw("%s",str.c_str());
        x += str.size()+1;
    }
    for (auto in=getch(); in!='\n'; in=getch()) {
        switch (in) {
        case KEY_RESIZE:
            reimpr_gui("Presione [Enter] para continuar.");
            manejar_claves(claves);
            return;
        }
    }
}

//******************************************************************************
// Rushed code ends here
//******************************************************************************

void impr_nombres_de_campo()
{
    xicampos.clear();
    int x=xid+1;
    for (auto str : nombre_de_campo) {
        xicampos.push_back(x);
        impr(yid,x,str+' ');
        x += str.size()+1;
        if (str==campo_de_nombre) {
            impr_char(yid,x,x+tam_max_nombre(),' ');
            x+=tam_max_nombre();
        }
    }
    impr_char(yid+1,xid,COLS-1,ACS_HLINE);
    mvaddch(yid+1,0,ACS_LTEE);
    mvaddch(yid+1,COLS-1,ACS_RTEE);
}

void impr_gui(const std::string& titulo, const std::string& msj_pie=msj_pie_menu)
{
    limpiar_scr();
    impr_titulo(titulo);
    impr_pie(msj_pie);
}

void gui_registros()
{
    static const std::string titulo="Lista de registros";
    impr_gui(titulo,msj_pie_registros);
    impr_nombres_de_campo();
}

std::pair<int,int> gui_actualizar()
{
    static const std::string titulo="Actualizar inventario";
    impr_gui(titulo);
    static const std::vector<std::string> opciones {"Entradas","Salidas",
                                                    "Salir"};
    return impr_menu(opciones);
}

std::pair<int,int> gui_inventario()
{
    static const std::string titulo="Inventario";
    impr_gui(titulo);
    static const std::vector<std::string> opciones {"Actualizar","Iniciar",
                                                    "Listar","Salir"};
    return impr_menu(opciones);
}

std::pair<int,int> gui_principal()
{
    static const std::string titulo="Menú principal";
    impr_gui(titulo);
    static const std::vector<std::string> opciones {"Claves","Inventario",
                                                    "Entradas","Salidas",
                                                    "Salir"};
    return impr_menu(opciones);
}

/* Guarda las abscisas de las líneas horizontales que separan el gui. */
const std::vector<int>& lineas_hor()
{
    static std::vector<int> lineas_hor {0,4,5,7,LINES-3,LINES-1};
    lineas_hor[lineas_hor.size()-2]=yipied();
    lineas_hor[lineas_hor.size()-1]=LINES-1;

    return lineas_hor;
}

/* Contiene los mensajes de encabezado del gui. */
const std::vector<std::string>& encabezado()
{
    static const std::vector<std::string> mensaje_por_linea {
               "Instituto Tecnológico de Sonora","Departamento de Laboratorios",
               "Sistema de Inventarios de Materiales y Reactivos"};
    return mensaje_por_linea;
}

void impr_bordes()
{
    /* Imprime los bordes que rodean y dividen el gui. */
    for (auto y : lineas_hor()) impr_char(y,1,COLS-1,ACS_HLINE);
    impr_char_ver(1,0,COLS-1,LINES-1,ACS_VLINE);
    for (int i=1; i<lineas_hor().size()-1; ++i) {
        mvaddch(lineas_hor()[i],0,ACS_LTEE);
        mvaddch(lineas_hor()[i],COLS-1,ACS_RTEE);
    }
    impr_esquinas(0,0,LINES-1,COLS-1);

    /* Imprime el encabezado usado durante todo el programa. */
    for (int i=0; i<encabezado().size(); ++i)
        impr(i+1,COLS/2-encabezado()[i].size()/2,encabezado()[i]);
}

} // Final del namespace GUI

} // Final del namespace ITSON
