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

    // Prepare the query
    if (!my_statement.prepare(my_connection, "SELECT * FROM books WHERE author_id = ?"))
    {
        cout << "Cannot prepare query!" << endl
            << my_connection.last_error();
        return 2;
    }

    // Execute it with parameter (author_id) "2"
    my_statement.param(1).set_as_long(2);
    if (!my_statement.execute())
    {
        cout << "Cannot execute prepared query!" << endl
            << my_statement.last_error();
        return 2;
    }

    // Get results from statement
    while(my_statement.fetch_next())
    {
        // print all fields for each row
        for(int i = 1;i <= my_statement.count_columns(); i ++)
            cout << my_statement.field(i).as_string() << "\t";

        cout << endl;
    }

    // Execute again with parameter (author_id) "1"
    my_statement.param(1).set_as_long(1);
    if (!my_statement.execute())
    {
        cout << "Cannot execute prepared query!" << endl
            << my_statement.last_error();
        return 2;
    }

    // Get results from statement
    while(my_statement.fetch_next())
    {
        // print all fields for each row
        for(int i = 1;i <= my_statement.count_columns(); i ++)
            cout << my_statement.field(i).as_string() << "\t";

        cout << endl;
    }
    return 0;
}