#include "Header.h"
#define NombreTienda "Ferreteria Amanecer"
#pragma warning(disable : 4996) 



const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y/%m/%d", &tstruct);

    return buf;
}

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

void HeaderBilling(char *nombre, int nit, char *facturaNo)
{

    cout << endl;
    SetConsoleTextAttribute(c, 6);
    cout << right << setw(55) << NombreTienda << endl;
    SetConsoleTextAttribute(c, 7);
    cout << "\t" << left << setw(5) << "Nombre : "
        << left << setw(30) << nombre
        << left << setw(2) << "Fecha: "
        << currentDateTime() 
        << endl;
    cout << "\t" << left << setw(5) << "Nit : " << nit << endl;
    cout << "\t" << left << setw(5) << "No Factura : " << facturaNo << endl;


    cout << "\t___________________________________________________________________" << endl;
  
    cout << "\t"
        << left << setw(5) << "No"
        << left << setw(30) << "Nombre"
        << left << setw(10) << "Cantidad"
        << left << setw(10) << "Precio U."
        << left << setw(16) << "Total"

        << endl;
    cout << "\t___________________________________________________________________" << endl;


}

