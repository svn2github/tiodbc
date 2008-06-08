#include "./tiodbc.hpp"
#include <iostream>

using namespace std;

int main()
{   
   // Create a connection with an ODBC Data Source
    tiodbc::connection my_connection("MyDSN", "", "");

    // Check if it was connected succesfully
    if (my_connection.connected())
        cout << "Connection has been accomplished";
    else
        cout << "Error connecting with server" << endl
            << my_connection.last_error();

    
    // Do more things here
    //.....

    // Disconnect
    my_connection.disconnect();

    return 0;
}