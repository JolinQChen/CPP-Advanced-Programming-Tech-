`Important: this file contains the basic command and information of this server-clients system`

## Compiling example for mac users:  

*clang++ -std=c++11 server.cpp -o server*  
*clang++ -std=c++11 client.cpp -o client*  
*./server 51717*  
*./client localhost 51717*



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
the user enters a “q” causes the socket to be closed and the program to terminate.

**Any messages received should be displayed as followed:**
Received Msg Type: 1, Seq: 33, Msg: message received
