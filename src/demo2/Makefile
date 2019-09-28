CC=gcc
CFLAGS=-Wall -pedantic

all:	raw_eth.o sender receiver

raw_eth.o: raw_eth.c
	$(CC) $(CFLAGS) -c raw_eth.c

sender:	raw_eth.c raw_eth.h raw_eth_cfg.h
	$(CC) $(CFLAGS) -o sender raw_eth.o sender.c

receiver:	raw_eth.c raw_eth.h raw_eth_cfg.h
	$(CC) $(CFLAGS) -o receiver raw_eth.o receiver.c

clean:
	rm *.o sender receiver
