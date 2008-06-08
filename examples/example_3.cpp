#include "./tiodbc.hpp"
#include <iostream>

using namespace std;

int main()
{   
    tiodbc::connection my_connection;
    tiodbc::statement my_statement;

    // Create a connection with an ODBC Data Source
    if (!my_connection.connect("MyDSN", "", ""))
    {
        cout << "Cannot connect to the Data Source" << endl
            << my_connection.last_error();
        return 1;
    }

    // Execute a direct query
    if (!my_statement.execute_direct(my_connection,
        "INSERT INTO books (id, title, author) VALUES(\'0\', \'TinyODBC Manual\', \'sque\')"))
    {
        cout << "Cannot execute query!" << endl
            << my_connection.last_error();
        return 2;
    }

    return 0;
}