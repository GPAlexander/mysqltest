#include "Layout.h"
#include <time.h>

#define SERVER "localhost"
#define USER "root"
#define PASSWORD ""
#define DATABASE "inventario"

#define LongitudCadigoFactura 10

int q_estado, option;
int filter;

MYSQL* mysql_init(MYSQL* objConnection);
MYSQL* objConnection;
int sizeStruct = 0;
int sizeStructUnique = 0;

struct ListProducts {
    int id;
    int idIside;
    string codigo;
    string nombre;
    float precio;
    int cantidad;
};
ListProducts lsp[100];
struct ListProductsUnique {
    int id;
    int idIside;
    string codigo;
    string nombre;
    float precio;
    int cantidad;
};
ListProductsUnique lspu[100];




void connect()
{

    if ((objConnection = mysql_init(0))) {

        if (!mysql_real_connect(
            objConnection, SERVER, USER, PASSWORD, DATABASE, 3307, NULL, 0)) {
            cout << "Imposible conectar con servidor mysql en el puerto " << 3307
                << endl;
            mysql_close(objConnection);
        }

    }
    else {
        cout << "Object Conecction Fail" << endl;
    }
}


int aleatorio_en_rango(int minimo, int maximo) {
    return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}


void cadena_aleatoria(int longitud, char* destino) {
    char muestra[] = "1234567890";
    for (int x = 0; x < longitud; x++) {
        int indiceAleatorio = aleatorio_en_rango(0, (int)strlen(muestra) - 1);
        destino[x] = muestra[indiceAleatorio];
    }
}


void tableData(string codigo)
{

    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    unsigned int num_fields;

    string selectAll
        = "SELECT * from productos where productos.codigo='" + codigo + "'";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado) {

        resultado = mysql_store_result(objConnection);

        cout << endl;
        SetConsoleTextAttribute(c, 6);
        cout << right << setw(55) << "Productos Filtrados Por codigo : " << codigo
            << endl;
        SetConsoleTextAttribute(c, 7);
        cout << "\t________________________________________________________________"
            "___________________"
            << endl;
        cout << "\t" << left << setw(8) << "ID" << left << setw(16) << "Codigo"
            << left << setw(18) << "Nombre" << left << setw(15) << "Cantidad"
            << left << setw(18) << "Precio U." << left << setw(10) << "Total"

            << endl;
        cout << "\t________________________________________________________________"
            "___________________"
            << endl;

        num_fields = mysql_num_rows(resultado);

        if (num_fields > 0) {
            while ((fila = mysql_fetch_row(resultado)) != NULL) {

                float cantidad, precio, total;
                cantidad = atoi(fila[3]);
                precio = atof(fila[4]);
                total = cantidad * precio;

                cout << "\t" << left << setw(8) << fila[0] << left << setw(16)
                    << fila[1] << left << setw(18) << fila[2] << left << setw(15)
                    << fila[3] << left << setw(0) << "Q." << fila[4] << right
                    << setw(13) << "Q." << total << endl;
            }
        }
        else {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)No se an encontrado Productos asocciados a este codigo: "
                << codigo << endl;
            SetConsoleTextAttribute(c, 7);
        }

    }
    else {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
        SetConsoleTextAttribute(c, 7);
    }
}


void SaveData()
{

    if (mysql_ping(objConnection)) {
        cout << "Error: conexión imposible" << endl;
        mysql_close(objConnection);
    }
    else {

        int CaseSave;
        do
        {

            string codigo;
            char nombre[50];
            int cantidad, validateQ = 0, colorQ = 7, colorP = 7, validateP = 0;
            float precio;

            cout << "(+)Ingrese Codigo del Producto : ";
            cin >> codigo;
            cout << "(+)Ingrese Nombre del Producto : ";
            cin.ignore();
            cin.getline(nombre, 50);

            do {
                SetConsoleTextAttribute(c, colorQ);
                cout << "(+) Ingrese Cantidad del Producto : ";
                SetConsoleTextAttribute(c, 7);

                cin >> cantidad;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(22, '\n');
                    colorQ = 4;

                }
                else {
                    if (cantidad >= 0) {
                        validateQ = 1;

                    }
                    else {

                        SetConsoleTextAttribute(c, 4);
                        cout << endl;
                        cout << "\t (*) Ingrese Una cantidad mayor o igual a 0" << endl;
                        SetConsoleTextAttribute(c, 7);
                    }
                }

            } while (validateQ != 1);

            do {
                SetConsoleTextAttribute(c, colorP);
                cout << "(+) Ingrese Precio del Producto : ";
                SetConsoleTextAttribute(c, 7);

                cin >> precio;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(22, '\n');
                    colorP = 4;

                }
                else {
                    if (precio >= 0) {
                        validateP = 1;

                    }
                    else {

                        SetConsoleTextAttribute(c, 4);
                        cout << endl;
                        cout << "\t (*) Ingrese Un Precio mayor o igual a 0" << endl;
                        SetConsoleTextAttribute(c, 7);
                    }
                }

            } while (validateP != 1);

            string insert = "INSERT INTO productos(codigo, nombre, cantidad, precio) VALUES('" + codigo + "','" + nombre + "', '" + to_string(cantidad) + "',  '" + to_string(precio) + "')";
            const char* i = insert.c_str();
            q_estado = mysql_query(objConnection, i);

            if (!q_estado) {
                SetConsoleTextAttribute(c, 2);
                cout << "*****Datos Guardados Correctamente!*****" << endl;
                SetConsoleTextAttribute(c, 7);

            }
            else {
                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)A Ocurrido un error intentelo nuevamente." << endl;
                SetConsoleTextAttribute(c, 7);
            }

            cout << endl << endl;
            cout << "Desea Continuar? " << endl;
            cout << "1...Si" << endl;
            cout << "2...No" << endl;
            cin >> CaseSave;

            if (CaseSave == 2) {
                CaseSave = 0;
            }

        } while (CaseSave!=0);
    }
}



void SelectAll()
{
    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    unsigned int num_fields;
    float totalInv = 0;

    string selectAll = "SELECT * from productos";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado) {

        resultado = mysql_store_result(objConnection);
        num_fields = mysql_num_rows(resultado);

        cout << endl;
        SetConsoleTextAttribute(c, 6);
        cout << right << setw(55) << "Productos Registrados." << endl;
        SetConsoleTextAttribute(c, 7);
        cout << "\t___________________________________________________________________________________"<< endl;
        cout << "\t" << left << setw(8) << "ID" << left << setw(16) << "Codigo"
            << left << setw(18) << "Nombre" << left << setw(15) << "Cantidad"
            << left << setw(18) << "Precio U." << left << setw(10) << "Total"
            << endl;
        cout << "\t___________________________________________________________________________________"<< endl;

        if (num_fields > 0) {
            while (fila = mysql_fetch_row(resultado)) {
                float cantidad, precio, total;
                cantidad = atoi(fila[3]);
                precio = atof(fila[4]);
                total = cantidad * precio;
                totalInv += total;

                cout << "\t" << left << setw(8) << fila[0] << left << setw(16)
                    << fila[1] << left << setw(18) << fila[2] << left << setw(15)
                    << fila[3] << left << setw(0) << "Q." << fila[4] << right
                    << setw(13) << "Q." << total << endl;
            }
            cout << endl;
            SetConsoleTextAttribute(c, 1);
            cout << "\t" << right << setw(75) << "Total del Inventario: "
                << "Q" << totalInv << endl;
            SetConsoleTextAttribute(c, 7);

        }
        else {
            SetConsoleTextAttribute(c, 4);
            cout << right << setw(65)
                << "(*)No se an encontrado Productos asocciados." << endl;
            SetConsoleTextAttribute(c, 7);
        }

    }
    else {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
        SetConsoleTextAttribute(c, 7);
    }

    system("pause");
}

void selectSingle()
{

    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    string codigo;
    unsigned int num_fields;

    cout << "ingrese Codigo del producto : ";
    cin >> codigo;

    string selectAll = "SELECT * from productos where productos.codigo='" + codigo + "'";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado) {

        resultado = mysql_store_result(objConnection);

        cout << endl;
        SetConsoleTextAttribute(c, 6);
        cout << right << setw(55) << "Productos Filtrados Por codigo : " << codigo
            << endl;
        SetConsoleTextAttribute(c, 7);
        cout << "\t___________________________________________________________________"
            << endl;
        cout << "\t" << left << setw(8) << "ID" << left << setw(16) << "Codigo"
            << left << setw(18) << "Nombre" << left << setw(15) << "Cantidad"
            << left << setw(10) << "Precio U." << endl;
        cout << "\t___________________________________________________________________"
            << endl;

        num_fields = mysql_num_rows(resultado);

        if (num_fields > 0) {
            while ((fila = mysql_fetch_row(resultado)) != NULL) {

                cout << "\t" << left << setw(8) << fila[0] << left << setw(16)
                    << fila[1] << left << setw(18) << fila[2] << left << setw(15)
                    << fila[3] << left << setw(0) << "Q." << fila[4] << endl;
            }
        }
        else {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)No se an encontrado Productos asocciados a este codigo: "
                << codigo << endl;
            SetConsoleTextAttribute(c, 7);
        }

    }
    else {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
        SetConsoleTextAttribute(c, 7);
    }
    system("pause");

}

void DeleteData()
{

    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    unsigned int num_fields;

    if (mysql_ping(objConnection)) {
        cout << "Error: conexión imposible" << endl;
        mysql_close(objConnection);
    }
    else {

        string codigo;

        cout << "(+)Ingrese Codigo del Producto : ";
        cin >> codigo;

        string SelectSingle
            = "SELECT * from productos where productos.codigo='" + codigo + "'";
        const char* s = SelectSingle.c_str();
        q_estado = mysql_query(objConnection, s);
        if (!q_estado) {
            resultado = mysql_store_result(objConnection);
            num_fields = mysql_num_rows(resultado);

            if (num_fields > 0) {
                int q_estado_delete;

                string deleteData = "DELETE FROM productos WHERE productos.codigo = '" + codigo + "'";
                const char* i = deleteData.c_str();
                q_estado_delete = mysql_query(objConnection, i);

                if (!q_estado_delete) {
                    SetConsoleTextAttribute(c, 2);
                    cout << "****Producto Eliminado Correctamente!*****" << endl;
                    SetConsoleTextAttribute(c, 7);

                }
                else {
                    SetConsoleTextAttribute(c, 4);
                    cout << "\t(*)A Ocurrido un error Al Eliminar nuevamente." << endl;
                    SetConsoleTextAttribute(c, 7);
                }

            }
            else {

                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)No se an encontrado Productos asocciados a este codigo: "
                    << codigo << endl;
                SetConsoleTextAttribute(c, 7);
            }

        }
        else {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
            SetConsoleTextAttribute(c, 7);
        }

        system("pause");

    }
}

void UpdateQuantity()
{

    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    string codigo;
    unsigned int num_fields;
    int cantidad, cantidadBefore, cantidadAfter, validateQ = 0, colorQ = 7;

    cout << "ingrese Codigo del producto : ";
    cin >> codigo;

    string selectAll
        = "SELECT * from productos where productos.codigo='" + codigo + "'";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado) {

        resultado = mysql_store_result(objConnection);

        cout << endl;
        SetConsoleTextAttribute(c, 6);
        cout << right << setw(55) << "Productos Filtrados Por codigo : " << codigo
            << endl;
        SetConsoleTextAttribute(c, 7);
        cout << "\t________________________________________________________________"
            "___________________"
            << endl;
        cout << "\t" << left << setw(8) << "ID" << left << setw(16) << "Codigo"
            << left << setw(18) << "Nombre" << left << setw(15) << "Cantidad"
            << left << setw(18) << "Precio U." << left << setw(10) << "Total"
            << endl;
        cout << "\t________________________________________________________________"
            "___________________"
            << endl;

        num_fields = mysql_num_rows(resultado);

        if (num_fields > 0) {
            while ((fila = mysql_fetch_row(resultado)) != NULL) {

                cantidadBefore = atoi(fila[3]);

                float cantidad, precio, total;
                cantidad = atoi(fila[3]);
                precio = atof(fila[4]);
                total = cantidad * precio;

                cout << "\t" << left << setw(8) << fila[0] << left << setw(16)
                    << fila[1] << left << setw(18) << fila[2] << left << setw(15)
                    << fila[3] << left << setw(0) << "Q." << fila[4] << right
                    << setw(13) << "Q." << total << endl;
            }
            cout << endl << endl;
            do {
                SetConsoleTextAttribute(c, colorQ);
                cout << "(+) Ingrese cantidad Nueva del Producto : ";
                SetConsoleTextAttribute(c, 7);

                cin >> cantidad;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(22, '\n');
                    colorQ = 4;

                }
                else {
                    if (cantidad >= 0) {
                        validateQ = 1;

                    }
                    else {

                        SetConsoleTextAttribute(c, 4);
                        cout << endl;
                        cout << "\t (*) Ingrese Una cantidad mayor o igual a 0" << endl;
                        SetConsoleTextAttribute(c, 7);
                    }
                }

            } while (validateQ != 1);

            cantidadAfter = cantidad + cantidadBefore;

            int statusUp;
            string updatedata = "UPDATE productos SET cantidad='"
                + to_string(cantidadAfter) + "' WHERE productos.codigo='" + codigo
                + "'";
            const char* i = updatedata.c_str();
            statusUp = mysql_query(objConnection, i);

            if (!statusUp) {
                SetConsoleTextAttribute(c, 2);
                cout << "*****Datos Actualizados Correctamente!*****" << endl;
                SetConsoleTextAttribute(c, 7);
                tableData(codigo);

            }
            else {
                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)A Ocurrido un error intentelo nuevamente." << endl;
                SetConsoleTextAttribute(c, 7);
            }

        }
        else {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)No se an encontrado Productos asocciados a este codigo: "
                << codigo << endl;
            SetConsoleTextAttribute(c, 7);
        }

    }
    else {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
        SetConsoleTextAttribute(c, 7);
    }
    system("pause");

}

void SubtractQuantity()
{

    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    string codigo;
    unsigned int num_fields;
    int cantidad, cantidadBefore = 0, cantidadAfter, validateQ = 0, colorQ = 7;

    cout << "ingrese Codigo del producto : ";
    cin >> codigo;

    string selectAll
        = "SELECT * from productos where productos.codigo='" + codigo + "'";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado) {

        resultado = mysql_store_result(objConnection);

        cout << endl;
        SetConsoleTextAttribute(c, 6);
        cout << right << setw(55) << "Productos Filtrados Por codigo : " << codigo
            << endl;
        SetConsoleTextAttribute(c, 7);
        cout << "\t________________________________________________________________"
            "___________________"
            << endl;
        cout << "\t" << left << setw(8) << "ID" << left << setw(16) << "Codigo"
            << left << setw(18) << "Nombre" << left << setw(15) << "Cantidad"
            << left << setw(18) << "Precio U." << left << setw(10) << "Total"
            << endl;
        cout << "\t________________________________________________________________"
            "___________________"
            << endl;

        num_fields = mysql_num_rows(resultado);

        if (num_fields > 0) {
            while ((fila = mysql_fetch_row(resultado)) != NULL) {

                cantidadBefore = atoi(fila[3]);

                float cantidad, precio, total;
                cantidad = atoi(fila[3]);
                precio = atof(fila[4]);
                total = cantidad * precio;

                cout << "\t" << left << setw(8) << fila[0] << left << setw(16)
                    << fila[1] << left << setw(18) << fila[2] << left << setw(15)
                    << fila[3] << left << setw(0) << "Q." << fila[4] << right
                    << setw(13) << "Q." << total << endl;
            }
            cout << endl << endl;
            do {
                SetConsoleTextAttribute(c, colorQ);
                cout << "(+) Ingrese cantidad a extraer del Producto : ";
                SetConsoleTextAttribute(c, 7);

                cin >> cantidad;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(22, '\n');
                    colorQ = 4;

                }
                else {
                    if (cantidad >= 0) {
                        validateQ = 1;

                    }
                    else {

                        SetConsoleTextAttribute(c, 4);
                        cout << endl;
                        cout << "\t (*) Ingrese Una cantidad mayor o igual a 0" << endl;
                        SetConsoleTextAttribute(c, 7);
                    }
                }

            } while (validateQ != 1);

            if (cantidadBefore > cantidad) {

                cantidadAfter = cantidadBefore - cantidad;
                int statusUp;
                string updatedata = "UPDATE productos SET cantidad='"
                    + to_string(cantidadAfter) + "' WHERE productos.codigo='" + codigo
                    + "'";
                const char* i = updatedata.c_str();
                statusUp = mysql_query(objConnection, i);

                if (!statusUp) {
                    SetConsoleTextAttribute(c, 2);
                    cout << "*****Datos Actualizados Correctamente!*****" << endl;
                    SetConsoleTextAttribute(c, 7);

                    tableData(codigo);

                }
                else {
                    SetConsoleTextAttribute(c, 4);
                    cout << "\t(*)A Ocurrido un error intentelo nuevamente." << endl;
                    SetConsoleTextAttribute(c, 7);
                }
            }
            else {
                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)Existencia Insuficiente." << endl;
                SetConsoleTextAttribute(c, 7);
            }

        }
        else {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)No se an encontrado Productos asocciados a este codigo: "
                << codigo << endl;
            SetConsoleTextAttribute(c, 7);
        }

    }
    else {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
        SetConsoleTextAttribute(c, 7);
    }
    system("pause");

}

void SearchAndAddBilling()
{
    int optionIside = 1, optionQInside = 1, QueryResult, QuantityTable,
        Quantity, Qlater, totalInS = 0, totalQIside, totalForDB = 0;
    string codigoPInside;
    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    unsigned int num_fields;

    do {
        int  total1 = 0, total2 = 0;
        system("cls");

        cout << "Ingrese Codigo de producto : ";
        cin >> codigoPInside;



        for (int i = 0; i < sizeStruct; i++) {

            if (lsp[i].codigo == codigoPInside)
            {
                total1 = lsp[i].cantidad;
                total2 = total2 + total1;
            }
        }




        string search = "SELECT * from productos where productos.codigo='"
            + codigoPInside + "'";
        const char* s = search.c_str();

        QueryResult = mysql_query(objConnection, s);

        if (!QueryResult) {

            resultado = mysql_store_result(objConnection);
            num_fields = mysql_num_rows(resultado);
            HeaderTable(codigoPInside);
            if (num_fields > 0) {

                string nombreR, codigoR;
                float precioR, total;
                int idR, totalForRow;

                while ((fila = mysql_fetch_row(resultado)) != NULL) {
                    idR = atof(fila[0]);
                    QuantityTable = atoi(fila[3]);

                    nombreR = fila[2];
                    precioR = atof(fila[4]);
                    codigoR = fila[1];
                    totalForRow = atof(fila[3]) - total2;


                    cout << "\t" << left << setw(8) << fila[0] << left << setw(16)
                        << fila[1] << left << setw(18) << fila[2] << left << setw(15)
                        << totalForRow << left << setw(0) << "Q" << fila[4] << endl;
                }
                cout << endl;

                do {
                    cout << "Cantidad a Comprar : ";
                    cin >> Quantity;
                    totalQIside = Quantity + totalInS;
                    if (QuantityTable >= totalQIside) {
                        lsp[sizeStruct].id = idR;
                        lsp[sizeStruct].codigo = codigoR;
                        lsp[sizeStruct].idIside = sizeStruct;
                        lsp[sizeStruct].nombre = nombreR;
                        lsp[sizeStruct].cantidad = Quantity;
                        lsp[sizeStruct].precio = precioR;
                        sizeStruct += 1;
                        optionQInside = 0;
                    }
                    else

                    {
                        SetConsoleTextAttribute(c, 4);
                        cout << "\t(*)Cantidad Insuficiente, Intentelo nuevamente" << endl;
                        SetConsoleTextAttribute(c, 7);
                    }
                } while (optionQInside != 0);

            }

            else {
                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)No se an encontrado Productos asocciados a este codigo: "
                    << codigoPInside << endl;
                SetConsoleTextAttribute(c, 7);
            }
        }

        cout << endl << endl;
        cout << "Desea Continuar? " << endl;
        cout << "1...Si" << endl;
        cout << "2...No" << endl;
        cin >> optionIside;

        if (optionIside == 2) {
            optionIside = 0;
        }

    } while (optionIside != 0);



    for (int org = 0; org < sizeStruct; org++)
    {
        bool repeat = false;
        for (int unq = 0; unq < sizeStructUnique; unq++)
        {
            cout << "aquii : " << lspu[unq].codigo << endl;
            if (lspu[unq].codigo==lsp[org].codigo)
            {
                lspu[unq].cantidad += lsp[org].cantidad;
                repeat = true;
                break;
            }
        

        }
        if (repeat==false)
        {
            lspu[sizeStructUnique].id = lsp[org].id;
            lspu[sizeStructUnique].idIside = lsp[org].idIside;
            lspu[sizeStructUnique].codigo = lsp[org].codigo;
            lspu[sizeStructUnique].nombre = lsp[org].nombre;
            lspu[sizeStructUnique].precio = lsp[org].precio;
            lspu[sizeStructUnique].cantidad = lsp[org].cantidad;

            sizeStructUnique += 1;
        }


    }

}
void InsertClientAndBilling(char* nombreCliente, int nit, char* codigoFactura)
{
    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    int statusQuery, user_id, statusQueryInsert, SavedTotal=0;

    string insert = "INSERT INTO clientes(nit, nombre) VALUES('" + to_string(nit) + "','" + nombreCliente + "')";
    const char* i = insert.c_str();
    statusQuery = mysql_query(objConnection, i);


    if (!statusQuery)
    {
        if ((resultado = mysql_store_result(objConnection)) == 0 && mysql_field_count(objConnection) == 0 && mysql_insert_id(objConnection) != 0)
        {
            user_id = mysql_insert_id(objConnection);


            for (int sqlKey = 0; sqlKey < sizeStructUnique; sqlKey++)
            {

                int product_id = lspu[sqlKey].id;
                int cantidad = lspu[sqlKey].cantidad;

                string insertBilling = "INSERT INTO factura(cliente_id, producto_id, codigo,cantidad) VALUES ('" + to_string(user_id) + "','" + to_string(product_id) + "', '" + codigoFactura + "', '" + to_string(cantidad) + "')";
             
                const char* qq = insertBilling.c_str();
                statusQueryInsert = mysql_query(objConnection, qq);

                if (!statusQueryInsert) {
                    SavedTotal += 1;

                }
                


            }











        }
    }
    else
    {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error intentelo nuevamente." << endl;
        SetConsoleTextAttribute(c, 7);

    }





}


void ShowBilling(char *nombreCliente, int nitCliente)
{
    float TotalFinal=0.00;
    system("cls");
    srand(time(NULL));
    char facturaNo[LongitudCadigoFactura + 1] = "";
    cadena_aleatoria(LongitudCadigoFactura, facturaNo);
    InsertClientAndBilling(nombreCliente, nitCliente, facturaNo);


    HeaderBilling(nombreCliente, nitCliente, facturaNo);

    for (int i = 0; i < sizeStructUnique; i++)
    {
        float totalIside = 0;
        totalIside = (lspu[i].cantidad * lspu[i].precio);
        TotalFinal += totalIside;
        cout << "\t"
            << left << setw(5) << i
            << left << setw(30) << lspu[i].nombre
            << left << setw(10) << lspu[i].cantidad
            << left << setw(1) << "Q"
            << left << setw(10) << lspu[i].precio
            << left << setw(1) << "Q"
            << left << setw(16) << totalIside
            << endl;

    }
    cout << endl;
    cout << "\t" << left << setw(45)<<""
        << left << setw(3) << "Total : "
        << left << setw(1) << "Q"
        << TotalFinal << endl;

}


void createBilling()
{

   

    char nombreCliente[100];
    int nitCliente=0, statusQuery, statusquerySelect;

     cout << "Ingrese Nombre Cliente : ";
     cin.ignore();
     cin.getline(nombreCliente, 100);
     cout << "Ingrese nit Cliente : ";
     cin >> nitCliente;

     cout << nombreCliente<<endl;
     cout << nitCliente << endl;

    SearchAndAddBilling();
    ShowBilling(nombreCliente, nitCliente);

     system("pause");

   
}

void ShowBilling()
{

    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    string codigo;
    unsigned int num_fields;
    int QueryResult;


    string selectBilling = "SELECT f.codigo, f.fecha, c.nombre, p.nombre, f.cantidad, p.precio FROM factura as f INNER JOIN clientes as c on f.cliente_id = c.id INNER JOIN productos as p on f.producto_id = p.id";
    const char* s = selectBilling.c_str();
    QueryResult = mysql_query(objConnection, s);

    if (!QueryResult) {
        resultado = mysql_store_result(objConnection);
        num_fields = mysql_num_rows(resultado);

        if (num_fields > 0) {
            cout << "\t____________________________________________________________________________________________________________________________" << endl;
            cout << "\t"
                << left << setw(15) << "No Factura"
                << left << setw(20) << "Fecha"
                << left << setw(20) << "Cliente"
                << left << setw(20) << "Producto"
                << left << setw(20) << "Cantidad P."
                << left << setw(20) << "Precio U."
                << left << setw(16) << "Total"
                << endl;
            cout << "\t____________________________________________________________________________________________________________________________" << endl;
            while ((fila = mysql_fetch_row(resultado)) != NULL) {

                int b;
                float a, total;
                a = atof(fila[5]);
                b = atoi(fila[4]);
               
                    total = (a*b);
                cout << "\t"
                    << left << setw(15) << fila[0]
                    << left << setw(20) << fila[1]
                    << left << setw(20) << fila[2]
                    << left << setw(20) << fila[3]
                    << left << setw(20) << fila[4]
                    << left << setw(1) << "Q"
                    << left << setw(20) << fila[5]
                    << left << setw(1) << "Q"
                    << left << setw(16) << total
                    << endl;
               
            }
            cout<< endl << endl;
        }
        else
        {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)No se an encontrado Facturas asocciados. " << endl;
            SetConsoleTextAttribute(c, 7);
        }



    }
    else
    {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)No se an encontrado Facturas asocciados. " << endl;
        SetConsoleTextAttribute(c, 7);

    }
    system("pause");
   
}

int main()
{


  connect();
    string StringSelect="Ingrese Una Opcion : ";
    int ColorString = 1;
    do
    {

        system("cls");
        cout << "\t Seleccione Una Opcion:" << endl;
        cout << "___________________________________________"<<endl;
        cout << "1...Registrar producto en el inventario" << endl;
        cout << "2...Mostrar productos del inventario" << endl;
        cout << "3...Ingresar producto al inventario" << endl;
        cout << "4...Extraer  producto del inventario" << endl;
        cout << "5...Buscar Producto" << endl;
        cout << "6...Eliminar producto" << endl;
        cout << "7...Crear Factura" << endl;
        cout << "8...Ver Historial De  Compras" << endl;
        cout << "9...Salir" << endl;
        cout << "___________________________________________" << endl<< endl;

        SetConsoleTextAttribute(c, ColorString);
        cout << StringSelect;
        SetConsoleTextAttribute(c, 7);

        while (!(cin>> option))
        {
            ColorString = 4;
            StringSelect = "(*) Solo Se Permiten Valores [1-9], intentelo nuevamente : "; 
            cin.clear(); 
            cin.ignore(22, '\n');
        }


        switch (option)
       {
        case 1:
            SaveData();
            break;
        case 2:
            SelectAll();
            break;
        case 3:
            UpdateQuantity();
            break;
        case 4:
            SubtractQuantity();
            break;
        case 5:
            selectSingle();
           break;

        case 6:
            DeleteData();
            break;
       case 7:
           createBilling();
           break;
       case 8:
           ShowBilling();
           break;
    
        case 9:
           

           return 0;
            break;
        default:
            system("cls");
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)Ingrese Una Opcion Valida, intente nuevamente." <<
             endl<< endl; SetConsoleTextAttribute(c, 7);

            break;
        }
   } while (option!=9);
    
    return 0;
}
