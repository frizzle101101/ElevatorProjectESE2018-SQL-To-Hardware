// Includes required (headers located in /usr/include)
#include "../include/databaseFunctions.h"

using namespace std;

void DBObj::initDBConnection() {
	// Create a connection
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "pi", "ese");
	con->setSchema("eprojese2018db");
}
void DBObj::logFloorReq(int nodeID, int status, int currentFloor, int requestedFloor) {
	sql::Statement *stmt;				// Crealte a pointer to a Statement object to hold statements
	sql::ResultSet *res;				// Create a pointer to a ResultSet object to hold results
	sql::PreparedStatement *pstmt; 		// Create a pointer to a prepared statement

	// Query database (possibly not necessary)
	// *****************************
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT CURRENT_DATE()");	// message query
	string currentDate;
	while(res->next()){
		currentDate = res->getString("CURRENT_DATE()");
	}

	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT CURRENT_TIME()");	// message query
	string currentTime ;
	while(res->next()){
		currentTime = res->getString("CURRENT_TIME()");
	}

	// Update database
	// *****************************
	pstmt = con->prepareStatement("INSERT INTO elv_req_log(nodeID,date,time,status,currentFloor,requestedFloor) VALUES (?,?,?,?,?,?)");
	pstmt->setInt(1, nodeID);
	pstmt->setString(2, currentDate);
	pstmt->setString(3, currentTime);
	pstmt->setInt(4, status);
	pstmt->setInt(5, currentFloor);
	pstmt->setInt(6, requestedFloor);
	pstmt->executeUpdate();

	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT reqId FROM elv_req_log ORDER BY reqId DESC LIMIT 1");	// message query
	int requestID;
	while(res->next()){
		requestID = res->getInt("reqId");
	}

	pstmt = con->prepareStatement("INSERT INTO elv_req_que(reqId) VALUES (?)");
	pstmt->setInt(1, requestID);
	pstmt->executeUpdate();

	// Clean up pointers
	delete res;
	delete pstmt;
	delete stmt;
}
int DBObj::getQuedReqFloor() {
	sql::Statement *stmt;				// Crealte a pointer to a Statement object to hold statements
	sql::ResultSet *res;				// Create a pointer to a ResultSet object to hold results
	sql::PreparedStatement *pstmt; 		// Create a pointer to a prepared statement

	// Query database
	// *****************************
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT reqId FROM elv_req_que ORDER BY reqId ASC LIMIT 1");	// message query
	int requestID;
	while(res->next()){
		requestID = res->getInt("reqId");
	}

	pstmt = con->prepareStatement("SELECT currentFloor FROM elv_req_log WHERE reqId = ?");	// message query
	pstmt->setInt(1, requestID);
	res = pstmt->executeQuery();
	int floorNum;
	while(res->next()){
		floorNum = res->getInt("currentFloor");
	}

	pstmt = con->prepareStatement("DELETE FROM elv_req_que WHERE reqId = ?");
	pstmt->setInt(1, requestID);
	pstmt->executeUpdate();

	// Clean up pointers
	delete res;
	delete pstmt;
	delete stmt;

	return floorNum;
}
void DBObj::cleanDBConnection() {
	// Clean up pointers
	delete con;
}
