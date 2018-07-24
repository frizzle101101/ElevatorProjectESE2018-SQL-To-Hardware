#ifndef DB_FUNCTIONS
#define DB_FUNCTIONS

#include <stdlib.h>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class DBObj
{
public:
  void initDBConnection();
  void logFloorReq(int nodeID, int status, int currentFloor, int requestedFloor);
  int getQuedReqFloor(int prev_floorNumber);
  void updateCurrentFloor(int currentFloor);
  void cleanDBConnection();

private:
  sql::Driver *driver; 			// Create a pointer to a MySQL driver object
	sql::Connection *con; 			// Create a pointer to a database connection object
  sql::Statement *stmt;				// Crealte a pointer to a Statement object to hold statements
	sql::ResultSet *res;				// Create a pointer to a ResultSet object to hold results
	sql::PreparedStatement *pstmt; 		// Create a pointer to a prepared statement
};
#endif
