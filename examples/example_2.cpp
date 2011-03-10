/***************************************************************************

    This file is part of project: TinyODBC
    TinyODBC is hosted under: http://code.google.com/p/tiodbc/

    Copyright (c) 2008-2011 SqUe <squarious _at_ gmail _dot_ com>

    The MIT Licence

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*****************************************************************************/

#include "../tiodbc.hpp"
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
    if (!my_statement.execute_direct(my_connection, "SELECT * FROM my_table"))
    {
        cout << "Cannot execute query!" << endl
            << my_connection.last_error();
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
