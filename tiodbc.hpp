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
	class statement;
	class param_impl;
	class field_impl;


	//! An ODBC connection object
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
		@return <b>True</b> if the connection succeds or <b>False<b> if it fails.
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


	//! An ODBC statement object
	/**
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
		statement();

		//! Construct and initialize
		statement(connection & _conn, const _tstring & _stmt);

		//! Destructor
		~statement();

		//! @name Core functionality
		//! @{

		//! Used to create a statement (used automatically by the other functions)
		bool open(connection & _conn);

		//! Check if it is an open statement
		bool is_open() const;

		//! Close statement
		void close();

		//! Get native HSTMT handle
		HDBC native_stmt_handle()
		{	return stmt_h;	}

		//! Get last error description
		_tstring last_error();

		//! Get last error code
		_tstring last_error_status_code();

		//! @}

		//! @name Statement construction
		//! @{ 

		//! Prepare statement
		bool prepare(connection & _conn, const _tstring & _stmt);

		//! Execute statement
		bool execute();

		//! Execute direct a query
		bool execute_direct(connection & _conn, const _tstring & _query);

		//! @}

		//! @name Result gathering
		//! @{	

		//! Fetch next
		bool fetch_next();

		//! Get a field by column number (1-based)
		const field_impl field(int _num) const;

		//! Count columns of the result
		int count_columns() const;

		//! Free results (aka SQLCloseCursor)
		void free_results();

		//! @}

		//! @name Parameters handling
		//! @{

		//! Handle a parameter
		param_impl &param(int _num);

		//! Reset parameters (unbind all parameters
		void reset_parameters();

		//! @}
	};	// !statement

	//! An ODBC field_impl
	/**
	@note odbc::field_impl is <B>Copyable</b>, <b>NON inheritable</b> and <b>NOT direct constructable</b>
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

		// Get field as float
		float as_float() const;
	}; // !field_impl


	//! An ODBC statement parameter
	/**
	@note tiodbc::param_impl is <B>Copyable</b>, <b>NON inheritable</b> and <b>NOT direct constructable</b>
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

		//! Set parameter as string
		const _tstring & set_as_string(const _tstring & _str);
		
		//! Set parameter as long
		const long & set_as_long(const long & _value);

		//! Set parameter as usigned long
		const unsigned long & set_as_unsigned_long(const unsigned long & _value);
	};	// !param_impl
};

#endif // !_TIODBC_HPP_DEFINED_