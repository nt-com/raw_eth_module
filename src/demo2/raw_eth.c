/****************************************************
 * Raw Ethernet Frames - C File
 * nmt @ NT-COM
 ***************************************************/

/****************************************************
 * LIBRARIES
 ***************************************************/
#include "raw_eth.h"

/****************************************************
 * MACROS
 ***************************************************/

/****************************************************
 * TYPES
 ***************************************************/

/****************************************************
 * GLOBALS
 ***************************************************/
struct ifreq if_idx;
struct ifreq if_mac;
struct ifreq ifopts;
char ifName[IFNAMSIZ] = { 0x00 };
int sockopt = -1;

/* sending */
uint8_t send_frame_buffer[RAW_ETH_MAX_PAYLOAD_SIZE] = { 0x00 };
struct ether_header *seh = (struct ether_header *) send_frame_buffer;
struct sockaddr_ll socket_address;

/* receiving */
uint8_t recv_frame_buffer[RAW_ETH_MAX_PAYLOAD_SIZE] = { 0x00 };
struct ether_header *reh = (struct ether_header *) recv_frame_buffer;

uint16_t send_len = 0;

uint16_t set_ethertype = 0x00;

/****************************************************
 * FUNCTION PROTOTYPES
 ***************************************************/

/* for verbose mode, prints an ethernet frame for debugging */

/**
 * @brief prints an ethernet frame
 * @param p the ethernet frame to print 
 * @param l the frame length
 * @return void
 */
static void raw_eth_print_frame(void *p, uint16_t l);

/****************************************************
 * INTERNAL
 ***************************************************/

static void raw_eth_print_frame(void *p, uint16_t l) {

	uint16_t ui16_loop = 0x00;
	uint8_t *ptr = (uint8_t*)p;

	for(ui16_loop = 0; ui16_loop < l; ui16_loop++) {
		printf("%02x", *(ptr+ui16_loop));
	} printf("\n--EOF--\n");

}
/****************************************************
 * EXTERNAL
 ***************************************************/

extern uint8_t raw_eth_socket(int *fd, uint16_t et) {
	
	*fd = socket(PF_PACKET, SOCK_RAW, htons(et));
	set_ethertype = htons(et);
	if( (*fd) == -1 ) {
		printf("raw_eth_socket - socket creation: %s\n", strerror(errno));
		return RAW_ETH_FAILURE;
	}

	strncpy(ifName, DEFAULT_INTERFACE, strlen(DEFAULT_INTERFACE));

	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl((*fd), SIOCGIFINDEX, &if_idx) == -1) {
		printf("raw_eth_socket - ifindex: %s\n", strerror(errno));
		return RAW_ETH_FAILURE;
	}
	
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl((*fd), SIOCGIFHWADDR, &if_mac) == -1) {
		printf("raw_eth_socket - get hwaddr: %s\n", strerror(errno));
		return RAW_ETH_FAILURE;
	}

	/* set source MAC address using interface information */
	seh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	seh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	seh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	seh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	seh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	seh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	socket_address.sll_halen = ETH_ALEN;

	/* receive options, set to promisc mode */
	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
	ioctl((*fd), SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl((*fd), SIOCSIFFLAGS, &ifopts);

	/* catch if socket is closed */
	if (setsockopt((*fd), SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1) {
		printf("raw_eth_socket: %s\n", strerror(errno));
		return RAW_ETH_FAILURE;
	}
	/* bind the socket */
	if (setsockopt((*fd), SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)	{
		printf("raw_eth_socket: %s\n", strerror(errno));
		return RAW_ETH_FAILURE;
	}
	
	return RAW_ETH_SUCCESS;

}

extern uint16_t raw_eth_assemble(uint8_t *d, uint8_t *p, uint16_t l) {

	uint8_t ui8_loop = 0x00;

	if(l > RAW_ETH_MAX_PAYLOAD_SIZE) {
		printf("raw_eth_assemble: payload is too long\n");
		return RAW_ETH_FAILURE;
	}

	if(l < RAW_ETH_MIN_PAYLOAD_SIZE) {
		printf("raw_eth_assemble: payload is too short\n");
		return RAW_ETH_FAILURE;
	}
	
	for(ui8_loop = 0; ui8_loop < 6; ui8_loop++) {
		seh->ether_dhost[ui8_loop] = *(d+ui8_loop);
		socket_address.sll_addr[ui8_loop] = *(d+ui8_loop);
	}

	seh->ether_type = 0x00;
	seh->ether_type = set_ethertype;

	memset(send_frame_buffer+ETHERNET_HEADER_FCS, 0, RAW_ETH_MAX_PAYLOAD_SIZE-ETHERNET_HEADER_FCS);
	memcpy(send_frame_buffer+ETHERNET_HEADER_FCS, p, l);

	send_len = ETHERNET_HEADER_FCS + l;

#ifdef VERBOSE
	printf("#VERBOSE# assembled Ethernet frame: \n");
	raw_eth_print_frame((void*)send_frame_buffer, send_len);		
#endif

	return send_len;

}

extern uint8_t raw_eth_send(int *fd, uint16_t size) {

	ssize_t sent_bytes = -1;
	sent_bytes = sendto((*fd), send_frame_buffer, size, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
	
	if (sent_bytes == -1)  {
		printf("raw_eth_send: %s\n", strerror(errno));
		return RAW_ETH_FAILURE;
	}

	if( (uint16_t)sent_bytes != size) {
		printf("#WARNING# not all bytes sent\n");
	}

#ifdef VERBOSE
	printf("#VERBOSE# frame sent!\n");
#endif

	return RAW_ETH_SUCCESS;

}

extern ssize_t raw_eth_recv(int *fd) {

	ssize_t recv_bytes = -1;
	
	recv_bytes = recvfrom((*fd), (void*)recv_frame_buffer, RAW_ETH_MAX_PAYLOAD_SIZE, 0, NULL, NULL);
	if(recv_bytes == -1) {
		printf("raw_eth_recv: %s\n", strerror(errno));
		return RAW_ETH_FAILURE;
	}

#ifdef VERBOSE
	printf("#INFO# received %ld bytes\n", recv_bytes);
#endif

	return recv_bytes;

}

extern void raw_eth_disass_payload(void *buf, uint16_t size) {

	size = size - ETHERNET_HEADER_FCS;
	memcpy(buf, recv_frame_buffer+ETHERNET_HEADER_FCS, size);

#ifdef VERBOSE
	raw_eth_print_frame(buf, size);	
#endif

}

extern void raw_eth_clear_recv(void) {
	memset(recv_frame_buffer, 0, RAW_ETH_MAX_PAYLOAD_SIZE);
}

extern uint8_t raw_eth_close(int *fd) {

	if(close(*fd) == -1) {
		printf("raw_eth_close: %s\n", strerror(errno));
		return RAW_ETH_FAILURE;
	}
	
	return RAW_ETH_SUCCESS;
	
}


