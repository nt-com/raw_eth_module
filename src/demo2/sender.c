/****************************************************
 * Raw Ethernet Packets - Main File 
 * Description: Example Usage of RAW_ETH Sender
 * nmt @ NT-COM
 ***************************************************/

/****************************************************
 * LIBRARIES
 ***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "raw_eth.h"

/****************************************************
 * MACROS
 ***************************************************/
#define SEND_FRAME_COUNT 10

/****************************************************
 * GLOBALS
 ***************************************************/

/****************************************************
 * FUNCTION PROTOTYPES
 ***************************************************/

/****************************************************
 * MAIN
 ***************************************************/
int main(int argc, char **argv) {

	uint16_t ui16_loop = 0x00;
	uint8_t frame_counter = 0x00;

	/* set your destination MAC address here */
	uint8_t dmac[6] = { 0x08, 0x00, 0x27, 0xe7, 0x66, 0xde };
	/* set the EtherType value here */
	uint16_t etype = 0x1234; /* EtherType */

	/* sending */
	uint16_t frame_size = 0x00;
	uint8_t send_payload[RAW_ETH_MAX_PAYLOAD_SIZE] = { 0x00 };

	int sockfd = -1;

	/* fill the payload data */	
	for(ui16_loop = 0; ui16_loop < RAW_ETH_MAX_PAYLOAD_SIZE; ui16_loop++) {
		send_payload[ui16_loop] = ui16_loop + 1;
	}

	/* create the socket file descriptor, set the ethertype to filter for */	
	if(raw_eth_socket(&sockfd, etype)) {
		printf("error creating socket interface\n");
	}

	printf("\n\n*** raw_eth sender demo ***\n\n");

	while((frame_counter++) < SEND_FRAME_COUNT) {
		/* assemble a long ethernet frame */
		frame_size = raw_eth_assemble(&dmac[0], &send_payload[0], RAW_ETH_MAX_PAYLOAD_SIZE);	
		if(frame_size > 1) {
			/* send the frame */
			if(raw_eth_send(&sockfd, frame_size)) {
				printf("error sending\n");
			}
		}
	}

	/* close the socket */
	if(raw_eth_close(&sockfd)) {
		printf("error closing\n");
	}

	printf("\n*** raw_eth sender demo end ***\n");

return EXIT_SUCCESS;

}
/****************************************************
 * FUNCTIONS
 ***************************************************/

