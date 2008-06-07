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
	// Define TCHARed std::string as _tstring;
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
		connection();

		//! Construct by data source
		connection(const _tstring & _dsn,
			const _tstring & _user,
			const _tstring & _pass);

		//! Destructor
		~connection();

		//! Connect to a data source
		bool connect(const _tstring & _dsn,
			const _tstring & _user,
			const _tstring & _pass);

		//! Check if it is open
		bool connected() const;

		//! Close connection
		void disconnect();

		//! Get native HDBC handle
		HDBC native_dbc_handle()
		{	return conn_h;	}

		//! Get native HENV handle
		HDBC native_evn_handle()
		{	return env_h;	}

		//! Get last error description
		_tstring last_error();

		//! Get last error code
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
	};	// !connection

	//! An ODBC field
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
	}; //! field_impl


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
	};
};

#endif // !_TIODBC_HPP_DEFINED_