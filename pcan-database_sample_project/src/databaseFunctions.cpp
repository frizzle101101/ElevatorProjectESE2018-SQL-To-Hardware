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

	// Query database (possibly not necessary)
	// *****************************
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT CURRENT_DATE()");	// message query
	string currentDate;
	if(res->next()){
		currentDate = res->getString("CURRENT_DATE()");
	}

	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT CURRENT_TIME()");	// message query
	string currentTime ;
	if(res->next()){
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
	if(res->next()){
		requestID = res->getInt("reqId");
	}

	pstmt = con->prepareStatement("INSERT INTO elv_req_que(reqId) VALUES (?)");
	pstmt->setInt(1, requestID);
	pstmt->executeUpdate();

	cleanDBPointers();
}
int DBObj::getQuedReqFloor(int prev_floorNumber) {

	// Query database
	// *****************************
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT reqId FROM elv_req_que ORDER BY reqId ASC LIMIT 1");	// message query
	int requestID;
	if(res->next()){
		requestID = res->getInt("reqId");
	}

	pstmt = con->prepareStatement("SELECT requestedFloor FROM elv_req_log WHERE reqId = ?");	// message query
	pstmt->setInt(1, requestID);
	res = pstmt->executeQuery();
	int floorNum;
	if(res->next()){
		floorNum = res->getInt("requestedFloor");
	}
	else{
		floorNum = prev_floorNumber;
	}

	pstmt = con->prepareStatement("DELETE FROM elv_req_que WHERE reqId = ?");
	pstmt->setInt(1, requestID);
	pstmt->executeUpdate();

	cleanDBPointers();

	return floorNum;
}
void DBObj::updateCurrentFloor(int currentFloor) {

	pstmt = con->prepareStatement("UPDATE elevator SET currentFloor = ? WHERE nodeID = 512");
	pstmt->setInt(1, currentFloor);
	pstmt->executeUpdate();

	delete pstmt;
}
void DBObj::cleanDBConnection() {
	// Clean up pointers
	delete con;
}
void DBObj::cleanDBPointers() {
// Clean up pointers
delete res;
delete pstmt;
delete stmt;
}
