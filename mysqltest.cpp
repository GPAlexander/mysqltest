#include "Layout.h"


#define SERVER "localhost"
#define USER "root"
#define PASSWORD ""
#define DATABASE "inventario"

int  q_estado, option;
int filter;
MYSQL* mysql_init(MYSQL* objConnection);
MYSQL* objConnection;



struct ListProducts
{
    int id;
    int idIside;
    string codigo;
    string nombre;
    float precio;
    int cantidad;


};
ListProducts lsp[5];

void connect()
{

    if ((objConnection = mysql_init(0))) {

        if (!mysql_real_connect(objConnection, SERVER, USER, PASSWORD, DATABASE, 3307, NULL, 0)) {
            cout << "Imposible conectar con servidor mysql en el puerto " << 3307 << endl;
            mysql_close(objConnection);
        }
       
    }
    else
    {
        cout << "Object Conecction Fail" << endl;

    }



 }


void tableData(string codigo)
{

    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    unsigned int num_fields;



    string selectAll = "SELECT * from productos where productos.codigo='" + codigo + "'";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado)
    {

        resultado = mysql_store_result(objConnection);


        cout << endl;
        SetConsoleTextAttribute(c, 6);
        cout << right << setw(55) << "Productos Filtrados Por codigo : " << codigo << endl;
        SetConsoleTextAttribute(c, 7);
        cout << "\t___________________________________________________________________________________" << endl;
        cout << "\t"
            << left << setw(8) << "ID"
            << left << setw(16) << "Codigo"
            << left << setw(18) << "Nombre"
            << left << setw(15) << "Cantidad"
            << left << setw(18) << "Precio U."
            << left << setw(10) << "Total"

            << endl;
        cout << "\t___________________________________________________________________________________" << endl;



        num_fields = mysql_num_rows(resultado);

        if (num_fields > 0) {
            while ((fila = mysql_fetch_row(resultado)) != NULL)
            {


                float cantidad, precio, total;
                cantidad = atoi(fila[3]);
                precio = atof(fila[4]);
                total = cantidad * precio;

                cout << "\t"
                    << left << setw(8) << fila[0]
                    << left << setw(16) << fila[1]
                    << left << setw(18) << fila[2]
                    << left << setw(15) << fila[3]
                    << left << setw(0) << "Q." << fila[4]
                    << right << setw(13) << "Q." << total
                    << endl;


            }
        }
        else
        {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)No se an encontrado Productos asocciados a este codigo: " << codigo << endl;
            SetConsoleTextAttribute(c, 7);
        }

    }
    else
    {
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
    else
    {
        
        string codigo;
        char nombre[50];
        int cantidad, validateQ = 0, colorQ = 7, colorP=7, validateP=0;
        float precio;
        
        cout << "(+)Ingrese Codigo del Producto : ";
        cin >> codigo;
        cout << "(+)Ingrese Codigo del Producto : ";
        cin.ignore();
        cin.getline(nombre, 50);  
        
        do
        {
            SetConsoleTextAttribute(c, colorQ);
            cout << "(+) Ingrese Cantidad del Producto : ";
            SetConsoleTextAttribute(c,7);

            cin >> cantidad;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(22, '\n');
                colorQ = 4;

             }
            else
            {
                if (cantidad >= 0)
                {
                    validateQ = 1;

                }
                else
                {

                    SetConsoleTextAttribute(c, 4);
                    cout << endl;
                    cout << "\t (*) Ingrese Una cantidad mayor o igual a 0"<<endl;
                    SetConsoleTextAttribute(c, 7);
                }
            }

          
        
        } while (validateQ != 1);
       

        do
        {
            SetConsoleTextAttribute(c, colorP);
            cout << "(+) Ingrese Precio del Producto : ";
            SetConsoleTextAttribute(c, 7);

            cin >> precio;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(22, '\n');
                colorP = 4;

            }
            else
            {
                if (precio >= 0)
                {
                    validateP = 1;

                }
                else
                {

                    SetConsoleTextAttribute(c, 4);
                    cout << endl;
                    cout << "\t (*) Ingrese Un Precio mayor o igual a 0" << endl;
                    SetConsoleTextAttribute(c, 7);
                }
            }



        } while (validateP != 1);

        string insert = "INSERT INTO productos(codigo, nombre, cantidad, precio) VALUES('" + codigo + "','" + nombre + "', '"+to_string(cantidad)+ "',  '" + to_string(precio) + "')";
        const char* i = insert.c_str();
        q_estado = mysql_query(objConnection, i);

        if (!q_estado)
        {
            SetConsoleTextAttribute(c, 2);
            cout << "*****Datos Guardados Correctamente!*****"<<endl;
            SetConsoleTextAttribute(c, 7);


        }
        else
        {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)A Ocurrido un error intentelo nuevamente." << endl;
            SetConsoleTextAttribute(c, 7);

        }

    }
}

void SelectAll() {
    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    unsigned int num_fields;
    float totalInv=0;
        
    string selectAll = "SELECT * from productos";
        const char* s = selectAll.c_str();

        q_estado = mysql_query(objConnection, s);

        if (!q_estado)
        {

            resultado = mysql_store_result(objConnection);
            num_fields = mysql_num_rows(resultado);

            cout <<endl;
            SetConsoleTextAttribute(c, 6);
            cout <<right << setw(55)<<"Productos Registrados."<<endl;
            SetConsoleTextAttribute(c, 7);
            cout << "\t___________________________________________________________________________________" << endl;
            cout << "\t"
                 << left << setw(8) << "ID"
                 << left << setw(16) << "Codigo"
                 << left << setw(18) << "Nombre"
                 << left << setw(15) << "Cantidad"
                 << left << setw(18) << "Precio U."
                 << left << setw(10) << "Total"

                 << endl;
            cout << "\t___________________________________________________________________________________" << endl;


             if (num_fields > 0) {

                 while (fila = mysql_fetch_row(resultado))
                 {
                     float cantidad, precio, total;
                     cantidad = atoi(fila[3]);
                     precio = atof(fila[4]);
                     total = cantidad * precio;
                     totalInv += total;

                     cout << "\t"
                         << left << setw(8) << fila[0]
                         << left << setw(16) << fila[1]
                         << left << setw(18) << fila[2]
                         << left << setw(15) << fila[3]
                         << left << setw(0) << "Q." << fila[4]
                         << right << setw(13)<<"Q."<< total
                         << endl;
                 }
                 cout << endl;
                 SetConsoleTextAttribute(c, 1);
                 cout << "\t"<< right << setw(75) << "Total del Inventario: "<<"Q"<< totalInv << endl;
                 SetConsoleTextAttribute(c, 7);

             }
             else
             {
                 SetConsoleTextAttribute(c, 4);
                 cout << right << setw(65) << "(*)No se an encontrado Productos asocciados." << endl;
                 SetConsoleTextAttribute(c, 7);
             }

            

        }
        else
        {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
            SetConsoleTextAttribute(c, 7);

        }

        



}

void selectSingle()
{


    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    string codigo;
    unsigned int num_fields;


    cout << "ingrese Codigo del producto : ";
    cin >> codigo;

    string selectAll = "SELECT * from productos where productos.codigo='"+codigo+"'";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado)
    {

        resultado = mysql_store_result(objConnection);

        
            cout << endl;
            SetConsoleTextAttribute(c, 6);
            cout << right << setw(55) << "Productos Filtrados Por codigo : "<<codigo << endl;
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



            num_fields = mysql_num_rows(resultado);
           
            if (num_fields > 0) {
                while ((fila = mysql_fetch_row(resultado)) != NULL)
                {



                    cout << "\t"
                        << left << setw(8) << fila[0]
                        << left << setw(16) << fila[1]
                        << left << setw(18) << fila[2]
                        << left << setw(15) << fila[3]
                        << left << setw(0) << "Q." << fila[4]
                        << endl;


                }
            }
            else
            {
                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)No se an encontrado Productos asocciados a este codigo: "<<codigo << endl;
                SetConsoleTextAttribute(c, 7);
            }

    }
    else
    {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
        SetConsoleTextAttribute(c, 7);

    }

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
    else
    {

        string codigo;
       

        cout << "(+)Ingrese Codigo del Producto : ";
        cin >> codigo;


        string SelectSingle = "SELECT * from productos where productos.codigo='" + codigo + "'";
        const char* s = SelectSingle.c_str();
        q_estado = mysql_query(objConnection, s);
        if (!q_estado)
        {
            resultado = mysql_store_result(objConnection);
            num_fields = mysql_num_rows(resultado);


            if (num_fields > 0) {
                int q_estado_delete;

                string deleteData = "DELETE FROM productos WHERE productos.codigo = '" + codigo + "'";
                const char* i = deleteData.c_str();
                q_estado_delete = mysql_query(objConnection, i);

                if (!q_estado_delete)
                {
                    SetConsoleTextAttribute(c, 2);
                    cout << "****Producto Eliminado Correctamente!*****" << endl;
                    SetConsoleTextAttribute(c, 7);


                }
                else
                {
                    SetConsoleTextAttribute(c, 4);
                    cout << "\t(*)A Ocurrido un error Al Eliminar nuevamente." << endl;
                    SetConsoleTextAttribute(c, 7);

                }


            }
            else
            {

                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)No se an encontrado Productos asocciados a este codigo: " << codigo << endl;
                SetConsoleTextAttribute(c, 7);

            }


        }
        else
        {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
            SetConsoleTextAttribute(c, 7);

        }




        /**/

    }
}

void UpdateQuantity()
{


    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    string codigo;
    unsigned int num_fields;
    int cantidad,cantidadBefore,cantidadAfter, validateQ = 0, colorQ = 7;

    cout << "ingrese Codigo del producto : ";
    cin >> codigo;

    string selectAll = "SELECT * from productos where productos.codigo='" + codigo + "'";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado)
    {

        resultado = mysql_store_result(objConnection);


        cout << endl;
        SetConsoleTextAttribute(c, 6);
        cout << right << setw(55) << "Productos Filtrados Por codigo : " << codigo << endl;
        SetConsoleTextAttribute(c, 7);
        cout << "\t___________________________________________________________________________________" << endl;
        cout << "\t"
            << left << setw(8) << "ID"
            << left << setw(16) << "Codigo"
            << left << setw(18) << "Nombre"
            << left << setw(15) << "Cantidad"
            << left << setw(18) << "Precio U."
            << left << setw(10) << "Total"
            << endl;
        cout << "\t___________________________________________________________________________________" << endl;



        num_fields = mysql_num_rows(resultado);

        if (num_fields > 0) {
            while ((fila = mysql_fetch_row(resultado)) != NULL)
            {

                cantidadBefore = atoi(fila[3]);

                float cantidad, precio, total;
                cantidad = atoi(fila[3]);
                precio = atof(fila[4]);
                total = cantidad * precio;

                cout << "\t"
                    << left << setw(8) << fila[0]
                    << left << setw(16) << fila[1]
                    << left << setw(18) << fila[2]
                    << left << setw(15) << fila[3]
                    << left << setw(0) << "Q." << fila[4]
                    << right << setw(13) << "Q." << total
                    << endl;



            }
            cout << endl << endl;        
            do
            {
                SetConsoleTextAttribute(c, colorQ);
                cout << "(+) Ingrese cantidad Nueva del Producto : ";
                SetConsoleTextAttribute(c, 7);

                cin >> cantidad;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(22, '\n');
                    colorQ = 4;

                }
                else
                {
                    if (cantidad >= 0)
                    {
                        validateQ = 1;

                    }
                    else
                    {

                        SetConsoleTextAttribute(c, 4);
                        cout << endl;
                        cout << "\t (*) Ingrese Una cantidad mayor o igual a 0" << endl;
                        SetConsoleTextAttribute(c, 7);
                    }
                }



            } while (validateQ != 1);

            cantidadAfter = cantidad+cantidadBefore;

            int statusUp;
            string updatedata = "UPDATE productos SET cantidad='"+to_string(cantidadAfter) + "' WHERE productos.codigo='" + codigo + "'";
            const char* i = updatedata.c_str();
            statusUp = mysql_query(objConnection, i);

            if (!statusUp)
            {
                SetConsoleTextAttribute(c, 2);
                cout << "*****Datos Actualizados Correctamente!*****" << endl;
                SetConsoleTextAttribute(c, 7);
                tableData(codigo);


            }
            else
            {
                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)A Ocurrido un error intentelo nuevamente." << endl;
                SetConsoleTextAttribute(c, 7);

            }
            
        }
        else
        {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)No se an encontrado Productos asocciados a este codigo: " << codigo << endl;
            SetConsoleTextAttribute(c, 7);
        }

    }
    else
    {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
        SetConsoleTextAttribute(c, 7);

    }


}

void SubtractQuantity()
{


    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    string codigo;
    unsigned int num_fields;
    int cantidad, cantidadBefore=0, cantidadAfter, validateQ = 0, colorQ = 7;

    cout << "ingrese Codigo del producto : ";
    cin >> codigo;

    string selectAll = "SELECT * from productos where productos.codigo='" + codigo + "'";
    const char* s = selectAll.c_str();

    q_estado = mysql_query(objConnection, s);

    if (!q_estado)
    {

        resultado = mysql_store_result(objConnection);


        cout << endl;
        SetConsoleTextAttribute(c, 6);
        cout << right << setw(55) << "Productos Filtrados Por codigo : " << codigo << endl;
        SetConsoleTextAttribute(c, 7);
        cout << "\t___________________________________________________________________________________" << endl;
        cout << "\t"
            << left << setw(8) << "ID"
            << left << setw(16) << "Codigo"
            << left << setw(18) << "Nombre"
            << left << setw(15) << "Cantidad"
            << left << setw(18) << "Precio U."
            << left << setw(10) << "Total"
            << endl;
        cout << "\t___________________________________________________________________________________" << endl;



        num_fields = mysql_num_rows(resultado);

        if (num_fields > 0) {
            while ((fila = mysql_fetch_row(resultado)) != NULL)
            {

                cantidadBefore = atoi(fila[3]);

                float cantidad, precio, total;
                cantidad = atoi(fila[3]);
                precio = atof(fila[4]);
                total = cantidad * precio;

                cout << "\t"
                    << left << setw(8) << fila[0]
                    << left << setw(16) << fila[1]
                    << left << setw(18) << fila[2]
                    << left << setw(15) << fila[3]
                    << left << setw(0) << "Q." << fila[4]
                    << right << setw(13) << "Q." << total
                    << endl;


            }
            cout << endl << endl;
            do
            {
                SetConsoleTextAttribute(c, colorQ);
                cout << "(+) Ingrese cantidad a extraer del Producto : ";
                SetConsoleTextAttribute(c, 7);

                cin >> cantidad;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(22, '\n');
                    colorQ = 4;

                }
                else
                {
                    if (cantidad >= 0)
                    {
                        validateQ = 1;

                    }
                    else
                    {

                        SetConsoleTextAttribute(c, 4);
                        cout << endl;
                        cout << "\t (*) Ingrese Una cantidad mayor o igual a 0" << endl;
                        SetConsoleTextAttribute(c, 7);
                    }
                }



            } while (validateQ != 1);

           


            if (cantidadBefore> cantidad)
            {
                
                cantidadAfter = cantidadBefore - cantidad;
                int statusUp;
                string updatedata = "UPDATE productos SET cantidad='" + to_string(cantidadAfter) + "' WHERE productos.codigo='" + codigo + "'";
                const char* i = updatedata.c_str();
                statusUp = mysql_query(objConnection, i);

                if (!statusUp)
                {
                    SetConsoleTextAttribute(c, 2);
                    cout << "*****Datos Actualizados Correctamente!*****" << endl;
                    SetConsoleTextAttribute(c, 7);

                    tableData(codigo);

                }
                else
                {
                    SetConsoleTextAttribute(c, 4);
                    cout << "\t(*)A Ocurrido un error intentelo nuevamente." << endl;
                    SetConsoleTextAttribute(c, 7);

                }
            }
            else
            {
                SetConsoleTextAttribute(c, 4);
                cout << "\t(*)Existencia Insuficiente." << endl;
                SetConsoleTextAttribute(c, 7);

            }



        }
        else
        {
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)No se an encontrado Productos asocciados a este codigo: " << codigo << endl;
            SetConsoleTextAttribute(c, 7);
        }

    }
    else
    {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error al realizar la consulta" << endl;
        SetConsoleTextAttribute(c, 7);

    }


}

void SearchAndAddBilling()
{
    int optionIside = 1, optionQInside = 1, QueryResult, route=0, QuantityTable, Quantity, Qlater;
    string codigoPInside;
    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    unsigned int num_fields;



    do
    {
        system("cls");
           cout << "Ingrese Codigo de producto : ";
           cin >> codigoPInside;

           string search = "SELECT * from productos where productos.codigo='" + codigoPInside + "'";
           const char* s = search.c_str();

           QueryResult = mysql_query(objConnection, s);

           if (!QueryResult)
           {



               resultado = mysql_store_result(objConnection);
               num_fields = mysql_num_rows(resultado);
               HeaderTable(codigoPInside);
               if (num_fields > 0) {

                   string nombreR, codigoR;
                   float  precioR, total;
                   int idR;

                 

                   while ((fila = mysql_fetch_row(resultado)) != NULL)
                   {
                       idR = atof(fila[0]);
                       QuantityTable = atoi(fila[3]);

                       nombreR = fila[2];
                       precioR = atof(fila[4]);
                       codigoR = fila[1];



                       cout << "\t"
                           << left << setw(8) << fila[0]
                           << left << setw(16) << fila[1]
                           << left << setw(18) << fila[2]
                           << left << setw(15) << fila[3]
                           << left << setw(0) << "Q" << fila[4]
                           << endl;
                   }
                   cout << endl ;
                

                 


                   do
                   {
                       cout << "Cantidad a Comprar : ";
                       cin >> Quantity;
                       if (QuantityTable >= Quantity)
                       {
                           lsp[route].id = idR;
                           lsp[route].codigo = codigoR;
                           lsp[route].idIside = route;
                           lsp[route].nombre = nombreR;
                           lsp[route].cantidad=Quantity;
                           lsp[route].precio = precioR;

                           for (int i = 0; i < route; i++)
                           {

                           }


                           optionQInside = 0;
                       }
                       else

                       {
                           SetConsoleTextAttribute(c, 4);
                           cout << "\t(*)Cantidad Insuficiente, Intentelo nuevamente" << endl;
                           SetConsoleTextAttribute(c, 7);

                       }
                   } while (optionQInside != 0);



                   route+=1;

             
               }

              



               else
               {
                   SetConsoleTextAttribute(c, 4);
                   cout << "\t(*)No se an encontrado Productos asocciados a este codigo: " << codigoPInside << endl;
                   SetConsoleTextAttribute(c, 7);
               }


           }

           cout << endl<<endl;
           cout << "Desea Continuar? "<<endl;
           cout << "1...Si"<<endl;
           cout << "2...No" << endl;
           cin >> optionIside;

           if (optionIside ==2)
           {
               optionIside = 0;
           }
        
    } while (optionIside!=0);



    for (int i = 0; i < route; i++)
    {
        cout << lsp[i].id << "\t \t";
        cout << lsp[i].idIside << "\t \t \t";
        cout << lsp[i].codigo << "\t \t \t";

        cout << lsp[i].nombre << "\t \t \t";
        cout << lsp[i].precio << "\t \t";
        wcout << lsp[i].cantidad << "\t \t";

        cout << endl;
    }



}



void createBilling()
{

    MYSQL_ROW fila;
    MYSQL_RES* resultado;


    char nombre[50];
    int nit, statusQuery, statusquerySelect;
    string result;



   /* cout << "Ingrese Nombre Cliente : ";
    cin.getline(nombre, 50);
    cout << "Ingrese nit Cliente : ";
    cin >> nit;*/

    SearchAndAddBilling();

    /*
    string insert = "INSERT INTO clientes(nit, nombre) VALUES('" + to_string(nit) + "','" + nombre + "')";
    const char* i = insert.c_str();
    statusQuery = mysql_query(objConnection, i);


    if (!statusQuery)
    {
        if ((resultado = mysql_store_result(objConnection)) == 0 && mysql_field_count(objConnection) == 0 && mysql_insert_id(objConnection) != 0)
        {
          int  user_id = mysql_insert_id(objConnection);
        }
    }
    else
    {
        SetConsoleTextAttribute(c, 4);
        cout << "\t(*)A Ocurrido un error intentelo nuevamente." << endl;
        SetConsoleTextAttribute(c, 7);

    }


    */

}


int main()
{


    connect();
    createBilling();
    /*
    string StringSelect="Ingrese Una Opcion : ";
    int ColorString = 1;
    do
    {
           
        //system("cls");
        cout << "\t Seleccione Una Opcion:" << endl;
        cout << "___________________________________________"<<endl;
        cout << "1...Registrar producto en el inventario" << endl;
        cout << "2...Mostrar productos del inventario" << endl;
        cout << "3...Ingresar producto al inventario" << endl;
        cout << "4...Extraer  producto del inventario" << endl;
        cout << "5...Buscar Producto" << endl;
        cout << "6...Eliminar producto" << endl;
        cout << "7...Salir" << endl;
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

            break;
        default:
            system("cls");
            SetConsoleTextAttribute(c, 4);
            cout << "\t(*)Ingrese Una Opcion Valida, intente nuevamente." << endl<< endl;
            SetConsoleTextAttribute(c, 7);

            break;
        }
    } while (option!=6);
    */
    return 0;

  

}
