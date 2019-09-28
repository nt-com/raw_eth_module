/****************************************************
 * Raw Ethernet Packets - Main File 
 * Description: Example Usage of RAW_ETH Receiver
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
#define RECEIVE_FRAME_COUNT 10

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

	uint8_t frame_counter = 0x00;

	/* set the EtherType value here */
	uint16_t etype = 0x1234; /* EtherType */

	/* receiving */
	ssize_t recv_size = 0x00;	
	uint8_t recv_payload[RAW_ETH_MAX_PAYLOAD_SIZE] = { 0x00 };

	int sockfd = -1;

	/* create the socket file descriptor, set the ethertype to filter for */	
	if(raw_eth_socket(&sockfd, etype)) {
		printf("error creating socket interface\n");
	}

	printf("\n\n*** raw_eth receiver demo ***\n\n");

	/* receive some frames */
	printf("waiting for frames...\n");
	while((frame_counter++) < RECEIVE_FRAME_COUNT) {
		/* receive a frame */
		recv_size = raw_eth_recv(&sockfd);	
		/* check if reception was successful, if yes get the payload */
		if(recv_size > 1) {
			raw_eth_disass_payload((void*)recv_payload, recv_size);
			/* clears the internal receive buffer before the next reception,
				 call this before each new raw_eth_recv */
			raw_eth_clear_recv();
		}
	}

	/* close the socket */
	if(raw_eth_close(&sockfd)) {
		printf("error closing\n");
	}

	printf("\n*** raw_eth receiver demo end ***\n");

return EXIT_SUCCESS;

}
/****************************************************
 * FUNCTIONS
 ***************************************************/

