#include "Header.h"

void HeaderTable(string codigo)
{

    cout << endl;
    SetConsoleTextAttribute(c, 6);
    cout << right << setw(55) << "Productos Filtrados Por codigo : " << codigo << endl;
    SetConsoleTextAttribute(c, 7);
    cout << "\t___________________________________________________________________" << endl;
    cout << "\t"
        << left << setw(8) << "ID"
        << left << setw(16) << "Codigo"
        << left << setw(18) << "Nombre"
        << left << setw(15) << "Cantidad"
        << left << setw(10) << "Precio U."
        << endl;
    cout << "\t___________________________________________________________________" << endl;

}

