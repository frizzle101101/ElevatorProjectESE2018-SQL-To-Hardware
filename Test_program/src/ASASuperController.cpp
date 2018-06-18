#include "../include/pcanFunctions.h"
#include "../include/ASASuperController.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>  
#include <errno.h>
#include <unistd.h> 
#include <signal.h>
#include <string.h>
#include <fcntl.h>    					// O_RDWR
#include <unistd.h>
#include <ctype.h>
#include <libpcan.h>   					// PCAN library





HANDLE h;

TPCANMsg Txmsg;
TPCANMsg Rxmsg;
DWORD status;


int supercontroller(void){


	// Open a CAN channel 
	h = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h2);

	

	
	
	// Read 'num' messages on the CAN bus
	while(1) {
		while((status = CAN_Read(h, &Rxmsg)) == PCAN_RECEIVE_QUEUE_EMPTY){
			sleep(1);
		}
		if(status != PCAN_NO_ERROR) {						// If there is an error, display the code
			printf("Error 0x%x\n", (int)status);
			//break;
		}
		
		Txmsg.ID = 0x100; 	
		Txmsg.MSGTYPE = MSGTYPE_STANDARD; 
		Txmsg.LEN = 1; 
		Txmsg.DATA[0] = data; 
		switch(Rxmsg.ID) {
			case 0x101:
				Txmsg.DATA[0] = data;
				status = CAN_Write(h, &Txmsg);
				break;
			
			case 0x200:
				Txmsg.DATA[0] = 0x04 & Rxmsg.DATA[0];
				status = CAN_Write(h, &Txmsg);
				break;
			
			case 0x201:
				Txmsg.DATA[0] = 0x05;
				status = CAN_Write(h, &Txmsg);
				break;
			case 0x202:
				Txmsg.DATA[0] = 0x06;
				status = CAN_Write(h, &Txmsg);
				break;
			case 0x203:
				Txmsg.DATA[0] = 0x07;
				status = CAN_Write(h, &Txmsg);
				break;

				
			
			
			
		}
		
		
		
										
		if(Rxmsg.ID != 0x01 && Rxmsg.LEN != 0x04) {		// Ignore status message on bus	
			printf("  - R ID:%4x LEN:%1x DATA:%02x \n",	// Display the CAN message
				(int)Rxmsg.ID, 
				(int)Rxmsg.LEN,
				(int)Rxmsg.DATA[0]);
		
		}
	}
	

	// Close CAN 2.0 channel and exit	
	CAN_Close(h2);
}
