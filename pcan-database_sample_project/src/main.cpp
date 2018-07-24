#include "../include/pcanFunctions.h"
#include "../include/databaseFunctions.h"
#include "../include/mainFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <time.h>

using namespace std;


// ******************************************************************

int main() {

	int choice;
	int ID;
	int data;
	int numRx;
	int floorNumber = 1, prev_floorNumber = 1;
	PCanObj pCan;
	DBObj dbObj;
	time_t startTime = 0;
	time_t currentTime = 0;
	int moving = 0;
	while(1) {
		system("@cls||clear");
		choice = menu();
		switch (choice) {
			case 4:
			printf("\nNow listening to commands from the website, and reciving from pcan as SC - press ctrl-z to cancel\n");
					pCan.pcanInit();

					startTime = time(NULL);
					while(1){
						dbObj.initDBConnection();
						currentTime = time(NULL);

						if(currentTime - startTime >= 10)
						{
							moving = 0;
						}

						//polling database
						if(moving == 0)
						{
							//deque if not moving still
							floorNumber = dbObj.getQuedReqFloor(prev_floorNumber);
						}
						// If floor number changes in database
						if (prev_floorNumber != floorNumber) {
							//send the comand to ec
							pCan.pcanTx(ID_SC_TO_EC, HexFromFloor(floorNumber));
							prev_floorNumber = floorNumber;
							startTime = time(NULL);
							moving = 1;
						}


						//checcking for can rx
						pCan.pcanLogRecievedRequest(dbObj);
						dbObj.cleanDBConnection();
					}
					pCan.pcanClose();

			case 5:
				return(0);

			default:
				printf("Error on input values");
				sleep(3);
				break;
		}
		sleep(1);					// delay between send/receive
	}

	return(0);
}
