/****************************************************
 * Raw Ethernet Frames - Header File
 * nmt @ NT-COM
 ***************************************************/

#ifndef RAW_ETH_H
#define RAW_ETH_H

/****************************************************
 * LIBRARIES
 ***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

#include "raw_eth_cfg.h"

/****************************************************
 * MACROS
 ***************************************************/
/* function return values */
#define RAW_ETH_SUCCESS	0
#define RAW_ETH_FAILURE 1

/* destination MAC, source MAC and FCS size */
#define ETHERNET_HEADER_FCS 14

/****************************************************
 * TYPES
 ***************************************************/

/****************************************************
 * GLOBALS
 ***************************************************/

/****************************************************
 * FUNCTION PROTOTYPES
 ***************************************************/

/**
 * @brief creates a raw ethernet socket
 * @fd socket file descriptor to use
 * @et ethertype to filter for
 * @return 0 on success, 1 on failure
 */
extern uint8_t raw_eth_socket(int *fd, uint16_t et);

/**
 * @brief assembles an ethernet frame
 * @param d destination MAC
 * @param p payload
 * @param l payload length in bytes
 * @return size of complete frame
 */
extern uint16_t raw_eth_assemble(uint8_t *d, uint8_t *p, uint16_t l);

/**
 * @brief sends a raw ethernet frame
 * @param fd the fd to send on
 * @param size size of the frame in bytes
 * @return 0 on success, 1 on failure
 */
extern uint8_t raw_eth_send(int *fd, uint16_t size);

/**
 * @brief receives a raw ethernet frame
 * @param fd the fd to receive from
 * @return 1 on failure, otherwise num bytes received
 */
extern ssize_t raw_eth_recv(int *fd);

/**
 * @brief extract payload data from frame
 * @param buf buffer to store payload in
 * @param size size of the buffer in bytes
 * @return void
 */
extern void raw_eth_disass_payload(void *buf, uint16_t size);

/**
 * @brief clears the receive frame buffer
 * @return void
 */
extern void raw_eth_clear_recv(void);

/**
 * @brief closes the socket
 * @param fd the socket to close
 * @return 0 on success, 1 on failure
 */
extern uint8_t raw_eth_close(int *fd);

#endif
