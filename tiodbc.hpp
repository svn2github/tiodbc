#ifndef _TIODBC_HPP_DEFINED_
#define _TIODBC_HPP_DEFINED_

// System headers
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

// STL Headers
#include <string>
#include <map>

//! The only one namespace of TinyODBC
/**
	Everything is well organized under this namespace
*/
namespace tiodbc
{
	//! MACRO for std::wstring or std::string based on _UNICODE definition.
	#ifdef _UNICODE
		typedef std::wstring _tstring;	
	#else
		typedef std::string _tstring;
	#endif

	// Class prototypes
	class connection;
	class field_impl;
	class param_impl;
	class statement;	

	//! @name Library Version
	//! @{

	//! Get current major version of TinyODBC library
	/**
		The major version is increased only when we
		have major changes.*/
	unsigned short version_major();

	//! Get current minor version of TinyODBC library
	/**
		The minor version is increased when new features
		are added/removed or API breakage occurs.
	*/
	unsigned short version_minor();

	//! Get current revision of this version
	/**
		The version revision number is changed only
		for bug fixes.
	*/
	unsigned short version_revision();

	//! @}

	//! An ODBC connection representation object
	/**
		Connection object is implementing the actual connection
		as an ODBC Client, this object can be used from statement
		to perform queries on this connection.
	@note tiodbc::connection is <B>Uncopiable</b> and <b>NON inheritable</b>
	*/
	class connection
	{
	private:
		HENV env_h;			//!< Handle of enviroment
		HDBC conn_h;		//!< Handle of connection
		bool b_connected;	//!< A flag if we are connected

		// Uncopiable
		connection(const connection&);
		connection & operator=(const connection&);
	
	public:
		//! Default constructor
		/**
			It constructs a connection object that is ready
			to connect.
		@see connect()
		*/
		connection();

		//! Construct and connect to a Data Source
		/**
			It constructs a connection object and connects
			it to the desired Data Source.
		@param _dsn The name of the Data Source
		@param _user The username for authenticating to the Data Source.
			If _user is empty string, it will try to connect with the predefined
			user stored inside the Data Source.
		@param _pass The password for authenticating to the Data Source.
			If _pass is empty string, it will try to connect with the predefined
			password stored inside the Data Source.
		@remarks
			If the connection fails, the object will be constructed properly
			but it will be unconnected. A failed connection does not mean that
			the object is dirty, you can use connect() to try connecting again.
		@see connected(), connect()
		*/
		connection(const _tstring & _dsn,
			const _tstring & _user,
			const _tstring & _pass);

		//! Destructor
		/**
			It will disconnect (if connected) from the db and
			all the statements will be invalidated.
		*/
		~connection();

		//! Connect to a Data Source
		/**
			Connect this object with a Data Source.
			If the object is already connected, it will
			disconnect automatically before trying to connect
			to the new Data Source.
		@param _dsn The name of the Data Source
		@param _user The username for authenticating to the Data Source.
			If _user is empty string, it will try to connect with the predefined
			user stored inside the Data Source.
		@param _pass The password for authenticating to the Data Source.
			If _pass is empty string, it will try to connect with the predefined
			password stored inside the Data Source.
		@return <b>True</b> if the connection succeds or <b>False</b> if it fails.
			For extensive error reporting call last_error() or last_error_status_code()
			after failure.		
		@see disconnect(), connect(), last_error(), last_error_status_code()
		*/
		bool connect(const _tstring & _dsn,
			const _tstring & _user,
			const _tstring & _pass);

		//! Check if it is connected
		/**
		@return <b>True</b> If the object is connected to any server, or
			<b>False</b> if it isn't.
		*/
		bool connected() const;

		//! Close connection
		/**
			If the object is connected it will close the connection.
			If the object is already disconnected, calling disconnect()
			will leave the object unaffected.
		*/
		void disconnect();

		//! Get native HDBC handle
		/**
			This is the <b>DataBaseConnetcion</b>
			handle that the object has allocated
			internally with ODBC ISO API. This handle
			can be usefull to anyone who needs to use ODBC ISO API
			alogn with TinyODBC.
		@see native_evn_handle()
		*/
		HDBC native_dbc_handle()
		{	return conn_h;	}

		//! Get native HENV handle
		/**
			This is the <b>Enviroment</b>
			handle that the object has allocated
			internally with ODBC ISO API. This handle
			can be usefull to anyone who needs to use ODBC ISO API
			alogn with TinyODBC.
		@see native_dbc_handle()
		*/
		HDBC native_evn_handle()
		{	return env_h;	}

		//! Get last error description
		/**
			Get the description of the error that occured
			with the last function call.
		@return If the last function call was succesfull it will
			return an empty string, otherwise it will return
			the description of the error that occured inside
			the ODBC driver.
		@see last_error_status_code()
		*/
		_tstring last_error();

		//! Get last error code
		/**
			Get the status of the error that occured
			with the last function call.
		@return If the last function call was succesfull it will
			return an empty string, otherwise it will return
			the status code of the error that occured inside
			the ODBC driver.
		@remarks The status codes are unique 5 legth strings that
			correspond to a unique error. For information of
			this status code check ODBC API Reference (http://msdn.microsoft.com/en-us/library/ms716412(VS.85).aspx)
			
		@see last_error();
		*/
		_tstring last_error_status_code();
	};	// !connection

	//! Representation of result set field.
	/**
	@note odbc::field_impl is <B>Copyable</b>, <b>NON inheritable</b> and <b>NOT direct constructable</b>
	@remarks
		You <b>must</b> not create objects of this class directly but instead invoke statement::field() to get
		one. You <b>must not</b> keep the objects for more than directly calling one of its members, see statement::field()
		for more details on this.
	*/
	class field_impl
	{
	public:
		friend class statement;

	private:
		HSTMT stmt_h;			//!< Handle of statement that field exists
		int col_num;			//!< Collumn number that field exists.
		
		// Not direct contructable
		field_impl(HSTMT _stmt, int _col_num);

	public:
	
		//! Copy constructor
		field_impl(const field_impl&);

		//! Copy operator
		field_impl & operator=(const field_impl&);

		//! Destructor
		~field_impl();

		//! @name Value retrieval functions
		//! @{

		//! Get field as string
		_tstring as_string() const;

		//! Get field as long
		long as_long() const;

		//! Get field as unsigned long
		unsigned long as_unsigned_long() const;

		//! Get field as short
		short as_short() const;

		//! Get field as unsigned short
		unsigned short as_unsigned_short() const;

		//! Get field as double
		double as_double() const;

		//! Get field as float
		float as_float() const;

		//! @}
	}; // !field_impl


	//! Handler prepared statements parameters.
	/**
	@note tiodbc::param_impl is <B>Copyable</b>, <b>NON inheritable</b> and <b>NOT direct constructable</b>
	@remarks
		You <b>must</b> not create objects of this class directly but instead invoke statement::param() to get
		one. You <b>must not</b> keep the objects for more than directly calling one of its members, 
		see statement::param() for more details on this.
	*/
	class param_impl
	{
	public:
		friend class statement;

	private:
		HSTMT stmt_h;			//!< Handle of statement that parameter is set
		int par_num;			//!< Order number of the parameter
		_tstring _int_string;	//!< Internal string buffer
		char _int_buffer[64];	//!< Internal buffer for small built-in types (64byte ... quite large)
		SQLINTEGER _int_SLOIP;	//!< Internal Str Length Or Indicator Pointer
		
		// Not direct contructable
		param_impl(HSTMT _stmt, int _par_num);

	public:
	
		//! Copy constructor
		param_impl(const param_impl&);

		//! Copy operator
		param_impl & operator=(const param_impl&);

		//! Destructor
		~param_impl();

		//! @name Value assignement functions
		//! @{

		//! Set parameter as string
		const _tstring & set_as_string(const _tstring & _str);
		
		//! Set parameter as long
		const long & set_as_long(const long & _value);

		//! Set parameter as unsigned long
		const unsigned long & set_as_unsigned_long(const unsigned long & _value);

		//! @}
	};	// !param_impl

	//! An ODBC statement representation object
	/**
		Represents a statement on the server. Statement is used to
		execute direct queries or prepare them and execute them
		multiple times with same or different parameters.
		
		A statement has a life-cycle from the time is opened
		to the time it is closed. A closed statement can be reused
		by reopening it.

		There is no need to directly open a statement, this is
		done automatically from the "statement construction"
		functions.
	@note tiodbc::statement is <B>Uncopiable</b> and <b>NON inheritable</b>
	*/
	class statement
	{
	private:
		HSTMT stmt_h;		//!< Handle of statement
		bool b_open;		//!< A flag if statement has been opened

		// List of parameters
		typedef std::map<int, param_impl *> param_map_type;
		typedef param_map_type::iterator param_it;
		param_map_type m_params;

		// Uncopiable
		statement(const statement&);
		statement & operator=(const statement&);
	
	public:
		//! Default constructor
		/**
			It will construct a statement ready
			to execute or prepare a query. At the construction
			time statement is not opened, but it is ready to.
		*/
		statement();

		//! Construct and prepare
		/**
			It will construct and open a new statement at
			a desired connectiona and prepare a query
			on it. After the construction the statement
			will be in open mode and will hold the prepared
			statement. You can use it to execute the stament
			multiple times, by passing (if any) different
			parameters.
		@param _conn The connection object to prepare the query
			on it. Queries are always prepared on servers.
		@param _stmt The sql query to prepare.

		@remarks The prepared query is <b>not</b> stored on
			server but it is temporary and will get deleted when
			the connection is closed or the statement.

			To check if the query was prepared succesfully you
			can run is_open() after construction to see if it is
			opened. If the preparation fails, you can use this
			statement object to open other queries, direct or prepared.

		@note If you want to execute a direct query to the server without
			preparing it, you can use execute_direct() after constructing
			a statement with the default constructor!
		@see prepare()
		*/
		statement(connection & _conn, const _tstring & _stmt);

		//! Destructor
		/**
			It will close the query or delete the store prepared query,
			and close the result cursor if any.
		@see close();
		*/
		~statement();

		//! @name Core functionality
		//! @{

		//! Used to create a statement (used automatically by the "statement construction" functions)
		/**
			There is no need to call this function directly. It is called
			by "Statement construction" function when they want to open a new
			fresh statement with the server.

		@param _conn The connection to the server where
			the statement will be opened inside it.

		@return
			- <b>True</b> If the statement was successfully opened in the server.
			- <b>False</b> If there was an error creating the statement.

		@remarks If there was an error opening a statement you can check
			for detailed error description with connection::last_error()
			of the connection object that you tried to open the connection and
            <b>NOT</b> by calling statement::last_error() as a closed statemenet
			is unable to do error reporting.
		*/
		bool open(connection & _conn);

		//! Check if it is an open statement
		bool is_open() const;

		//! Close statement
		/**
			It will close the statement, delete any stored results
			or prepared queries, or stored parameters.
		*/
		void close();

		//! Get native HSTMT handle
		/**
			This is the <b>Statement</b>
			handle that the object has allocated
			internally with ODBC ISO API. This handle
			can be usefull to anyone who needs to use ODBC ISO API
			alogn with TinyODBC.
		*/
		HDBC native_stmt_handle()
		{	return stmt_h;	}

		//! Get last error description
		/**
			Get the description of the error that occured
			with the last function call.
		@return If the last function call was succesfull it will
			return an empty string, otherwise it will return
			the description of the error that occured inside
			the ODBC driver.
		@see last_error_status_code()
		*/
		_tstring last_error();

		//! Get last error code
		/**
			Get the status of the error that occured
			with the last function call.
		@return If the last function call was succesfull it will
			return an empty string, otherwise it will return
			the status code of the error that occured inside
			the ODBC driver.
		@remarks The status codes are unique 5 legth strings that
			correspond to a unique error. For information of
			this status code check ODBC API Reference (http://msdn.microsoft.com/en-us/library/ms716412(VS.85).aspx)
			
		@see last_error();
		*/
		_tstring last_error_status_code();

		//! @}

		//! @name Statement construction
		//! @{ 

		//! Prepare a query
		/**
			It will close any previous open operation and will prepare
			an sql query for execution. For more information about <b>query 
			preparation</b> visit http://msdn.microsoft.com/en-us/library/ms716365.aspx

			If you need to execute direct an sql query directly you can use
			execute_direct().

		@param _conn The connection object to prepare the query
			on it. Queries are always prepared on servers.
		@param _stmt The sql query to prepare.
		@return <b>True</b> if the preparation was succesfull or <b>False</b> if
			there was an error. In case of error check last_error() for detailed
			description of error.

		@note This is a <b>"statement construction" function</b> which means
			that any previous opened operation of this statement will be closed
			and a new statement will be created.
		@see param(), execute(), fetch_next(), field(),  close()
		*/
		bool prepare(connection & _conn, const _tstring & _stmt);

		//! Execute directly an sql query to the server.
		/**
			It will close any previous opened operation and will execute
			directly the desired sql query at the server.

		@param _conn The connection object with the server where the
			query will be executed at.
		@param _query The sql query that will be execute at the server.
		@return <b>True</b> if the execution was succesfull or <b>False</b> if
			there was an error. In case of error check last_error() for detailed
			description of problem.
		@remarks
			After a succesfull execution of a query, the result cursor is pointed
			one slot before first row. To get the results of first row you 
			must first call once fetch_next() and then use field() 
			to get each field of the current row.

		@note This is a <b>"statement construction" function</b> which means
			that any previous opened operation of this statement will be closed
			and a new statement will be created.
		@see fetch_next(), count_columns(), field(), close()
		*/
		bool execute_direct(connection & _conn, const _tstring & _query);

		//! @}

		//! @name Result gathering
		//! @{	

		//! Execute a prepared statement
		/**
			It will execute the query that was previously prepared
			in this statement. 
			
			To execute a prepared query there are some preconditions
			that must be satisfied.\n
			- The statment must be opened and an sql query must have
			been prepared.
			- Any previous result set must be have been freed.
			- All the parameters of the prepared statement must been passed
			with param()

		@return <b>True</b> if the prepared query was succesfully executed
			or <b>False</b> if there was an error. In case of error 
			check last_error() for detailed	description of problem.

		@remarks
			After a succesfull execution of a query, the result cursor is pointed
			one slot before first row. To get the results of first row you 
			must first call once fetch_next() and then use field() 
			to get each field of the current row.

		@see fetch_next(), count_columns(), field(), close()
		*/
		bool execute();

		//! Fetch next result row
		/**
			If the statement has opened a result set, it
			will advance the cursor to the next row. Result sets
			are opened automatically when executing an sql query that 
			returns some results.

		@return
		- <b>True</b> if the cursor was advanced succesfully.
		- <b>False</b> if the end of result set has been reached or
			there isn't opened any result set.

		@see field()
		*/
		bool fetch_next();

		//! Get a field of the current row in the result set
		/**
			It will return a field of the current selected
			row from the result set.
		@param _num The column of the field that you want
			to retrieve. First column is the 1.
		@return 
			- If the operation was successful a valid field_impl 
			object that contains info about the selected field.
			- An invalid field_impl if there isn't any
			result set opened or the result cursor is not pointing 
			to any valid row, or the number of the field was wrong.
		@remarks Objects returns from field() should be used directly and
			must not be stored.\n
			<b>good</b> example: @code m_long = field(1).as_long() @endcode\n
			<b>bad</b> example: @code field_impl tmp_field = field(1);  m_long = tmp_field.as_long(); @endcode
		@see fetch_next();
		*/
		const field_impl field(int _num) const;

		//! Count columns of the result set
		/**
			It will return the number of columns of the
			current open result set.
		@return
			- If the operation was <b>successful</b> it will return a number
			<b>bigger than zero</b>.
			- <b>Less than zero or equal to</b> in case of any <b>error</b>.
		*/
		int count_columns() const;

		//! Free current opened result set.
		void free_results();

		//! @}

		//! @name Parameters handling
		//! @{

		//! Handle a parameter
		/**
			Returns a handle to a parameter of the prepared statement.
		@param _num The parameter number, starting from 1. This is the 
			way ODBC uses to identify the parameter markers. If there 
			are three parameters, the leftmost one is parameter no.1 
			and the rightmost is parameter no.3

		@return
			- If the operation was successful a valid param_impl
			object that can be used to set a value to the specific
			parameter.
			- An invalid param_impl object if there isn't any prepared
			statement, or the parameter number is wrong.
		@remarks Objects returns from param() should be used directly and
			must not be stored.\n
			<b>good</b> example: @code = param(1).set_as_long(1) @endcode\n
			<b>bad</b> example: @code param_impl tmp_parm = param(1);  tmp_parm.set_as_long(1); @endcode
		*/
		param_impl &param(int _num);

		//! Reset parameters (unbind all parameters)
		/**
			It will remove (unbind) all the setted
			parameters of the prepared statement.

			In case that this is not a prepared statement
			it will fail silently.
		*/
		void reset_parameters();

		//! @}
	};	// !statement
};

#endif // !_TIODBC_HPP_DEFINED_