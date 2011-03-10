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
