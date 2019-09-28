CC=gcc
CFLAGS=-Wall -pedantic

all:	raw_eth.o raw_eth_test

raw_eth.o: raw_eth.c
	$(CC) $(CFLAGS) -c raw_eth.c

raw_eth_test:	raw_eth.c raw_eth.h raw_eth_cfg.h
	$(CC) $(CFLAGS) -o raw_eth_test raw_eth.o main.c

clean:
	rm *.o raw_eth_test
