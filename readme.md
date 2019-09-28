# raw_eth - Send and Receive Ethernet Frames under Linux

### Description

raw_eth is a module to send and receive Ethernet frames under Linux. You can use the module as it is or modify it for your own project. I wrote this module for a software I'm working on and thought more people could make use of it / save time instead of reading manual pages and coding this stuff themselves.

### Documentation

See **raw_eth.h** and **raw_eth_cfg.h** in **src/demo1** or **src/demo2** and the **Usage** paragraph below.

### Usage

#### Demo 1 - Loopback, send and receive

You can test the module with a demo program supplied in **src/demo1**. I set the interface to be the loopback interface in **raw_eth_cfg.h**.

Running the demo:

* Change to the **src/demo1** directory and run make, after that execute raw_eth_test (you might need elevated privileges for this)
* Start a packet analyzer / packet capture software (for example Wireshark) to see the frames the demo program sends
* Start the demo 
* The demo program sends two frames, after that it expects to receive two frames


#### Demo 2 - Sender and Receiver

You will need virtual machines or computers for this, assuming Computer A is the sender and Computer B is receiver: 

* Set your specific interface name in **src/demo2/raw_eth_cfg.h**
* Change to the **src/demo2** directory
* Use make to compile, yielding the sender and receiver program
* Give the sender executable to Computer A and the receiver executable to Computer B
* Start the receiver first, then run the sender to send ten frames from Computer A to Computer B
* Start a packet analyzer / packet capture software (for example Wireshark) to see the frames the demo program sends

**NOTE** - I'm assuming your interface names are the same on Computer A and Computer B (I'm using virtual machines). If not, compile twice.

### License

Do whatever you want with this code.

### Notes

* For sending the raw_eth_send and raw_eth_assemble functions could be encapsulated in a wrapper function
* For receiving the raw_eth_recv, raw_eth_disass_payload and raw_eth_clear_recv functions could be encapsulated in a wrapper function.

I didn't do this because it was not desired in my use case. However, for you it might be.

nmt @ NT-COM
