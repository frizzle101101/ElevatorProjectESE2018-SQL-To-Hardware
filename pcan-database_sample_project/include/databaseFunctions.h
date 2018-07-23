#ifndef DB_FUNCTIONS

#define DB_FUNCTIONS

class DBObj
{
public:
  void initDBConnection();
  void logFloorReq(int nodeID, int status, int currentFloor, int requestedFloor);
  int getQuedReqFloor();
  void cleanDBConnection();

private:
  sql::Driver *driver; 			// Create a pointer to a MySQL driver object
	sql::Connection *con; 			// Create a pointer to a database connection object
};
#endif
