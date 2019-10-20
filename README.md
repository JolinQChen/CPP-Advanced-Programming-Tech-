# C++ Advanced Programming Code Examples
`Important: Use g++ to compile the .cpp files`

The code files in this repository are based on C++, covering the content of: 
OperatorOverload, Multi-thread(openMP), Socket, MPI

## 1. Algorithm: https://projecteuler.net/problem=669 The King's Banquet

## 2. OperatorOverriding

## 3. Multi-thread method implementation through openMP

## 4. UDP(server, client), socket, multi-thread

This is a console program that takes as a command line argument the port number on which the UDP Server will receive messages. The UDP server collects parts of a larger composite message from the clients. The UDP server collects these message parts from different clients and assembles them in prder based on the lSeqNum (defined in the private data structure: udpMessage). The UDP server keeps a running list of all clients that have sent a message to it and will broadcast the composite message to all clients when commanded. The UDP server is able to receive data from clients without blocking the main application thread. The program can respond to user input while handing socket communications at the same time.  

`Important: this file contains the basic command and information of this server-clients system`
## Data Structure

The data sent back and forth are using this following packet structure:

struct udpMessage  
{  
  unsigned char uVersion;  
  unsigned char nType;  
  unsigned short nMsgLen;  
  unsigned long lSeqNum;  
  char chMsg[1000];  
};  

## UDP Server  
### The UDP server have the following functionalities from the command prompt:  
if **command == 0**, the server immediately sends to all clients the current composite
message and clears out the composite message;  
if **command == 1**, the server immediately clears out the composite message;  
if **command == 2**, the server immediately displays to the console the composite message
but takes no other actions.

### The UDP server have the following functionalities when receiving messages:  
if **nVersion != 1**, then the message is ignored;  
If **nType == 0**, the composite message is immediately cleared and anything in the chMsg buffer is ignored;  
If **nType == 1**, the composite message is immediately cleared and the message in chMsg is used as the start of 
a new composite message;  
If **nType == 2**, the message in chMsg is added to the composite message based on its lSeqNum;
If **nType == 3**, the server immediately sends to all clients the current composite message and 
clears out the composite message.  
`If the composite message becomes larger than 1000 then the composite message (up to 1000) is immediately set 
out to all clients and any remaining characters are used to start a new composite message with a lSeqNum = 0.`  

## UDP Client  
The client.cpp is a console program that takes as a command line argument the IP Address and port 
number of the server as shown below:  
./client.out localhost 51717  
The program should prompt the user for inputs and display any messages received.  
Here are example user inputs:  
**Please enter command: v #**  
(the user enters a “v”, a space, and then a version number. This version number is now used in all new messages.)  
**Please enter command: t # # message string**  
(the user enters a “t”, a space, and then a type number, and then a sequence number, followed by the message (if any).)  
**Please enter command: q**  
