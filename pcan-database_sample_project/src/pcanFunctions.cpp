#include "../include/pcanFunctions.h"

void PCanObj::pcanInit()
{
	h = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);		// Open PCAN channel

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h);
}
void PCanObj::pcanClose()
{
	// Close CAN 2.0 channel and exit
	CAN_Close(h);
}
int PCanObj::pcanTx(int id, int data){
	// Set up message
	Txmsg.ID = id;
	Txmsg.MSGTYPE = MSGTYPE_STANDARD;
	Txmsg.LEN = 1;
	Txmsg.DATA[0] = data;

	sleep(1);
	status = CAN_Write(h, &Txmsg);
}

int PCanObj::pcanRxN(int num_msgs){
	int i = 0;

	// Open a CAN channel
	h2 = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h2, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h2);

	// Clear screen to show received messages
	system("@cls||clear");

	// receive CAN message  - CODE adapted from PCAN BASIC C++ examples pcanread.cpp
	printf("\nReady to receive message(s) over CAN bus\n");

	// Read 'num' messages on the CAN bus
	while(i < num_msgs) {
		while((status = CAN_Read(h2, &Rxmsg)) == PCAN_RECEIVE_QUEUE_EMPTY){
			sleep(1);
		}
		if(status != PCAN_NO_ERROR) {						// If there is an error, display the code
			printf("Error 0x%x\n", (int)status);
			//break;
		}

		if(Rxmsg.ID != 0x01 && Rxmsg.LEN != 0x04) {		// Ignore status message on bus
			printf("  - R ID:%4x LEN:%1x DATA:%02x \n",	// Display the CAN message
				(int)Rxmsg.ID,
				(int)Rxmsg.LEN,
				(int)Rxmsg.DATA[0]);
		i++;
		}
	}


	// Close CAN 2.0 channel and exit
	CAN_Close(h2);
	//printf("\nEnd Rx\n");
	return ((int)Rxmsg.DATA[0]);						// Return the last value received
}

void PCanObj::pcanLogRecievedRequest(DBObj& dbObj){

	if((status = CAN_Read(h, &Rxmsg)) != PCAN_RECEIVE_QUEUE_EMPTY)
	{
		if(status != PCAN_NO_ERROR) {						// If there is an error, display the code
			printf("Error 0x%x\n", (int)status);
			//break;
		}

		if(Rxmsg.ID == ID_EC_TO_ALL)
		{
			currentFloor = (int)Rxmsg.DATA[0] - 4;
		}
		else if(Rxmsg.ID != 0x01 && Rxmsg.LEN != 0x04) // Ignore status message on bus
		{
			printf("  - R ID:%4x LEN:%1x DATA:%02x \n",	// Display the CAN message
				(int)Rxmsg.ID,
				(int)Rxmsg.LEN,
				(int)Rxmsg.DATA[0]);
			if(Rxmsg.ID == ID_CC_TO_SC)
			{
				dbObj.logFloorReq((int)Rxmsg.ID, status, currentFloor, (int)Rxmsg.DATA[0]);
			}
			else if(Rxmsg.ID == ID_F1_TO_SC)
			{
				dbObj.logFloorReq((int)Rxmsg.ID, status, currentFloor, 1);
			}
			else if(Rxmsg.ID == ID_F2_TO_SC)
			{
				dbObj.logFloorReq((int)Rxmsg.ID, status, currentFloor, 2);
			}
			else if(Rxmsg.ID == ID_F3_TO_SC)
			{
				dbObj.logFloorReq((int)Rxmsg.ID, status, currentFloor, 3);
			}
		}
	}
}
