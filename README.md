# C++ Advanced Programming Code Examples
`Important: Use g++ to compile the .cpp files`

The code files in this repository are based on C++, covering the content of: 
OperatorOverload, Multi-thread(openMP), Socket, MPI

## 1. Algorithm: https://projecteuler.net/problem=669 The King's Banquet

## 2. OperatorOverriding

## 3. Multi-thread method implementation through openMP

## 4. UDP(server, client), socket, multi-thread

This is a console program that takes as a command line argument the port number on which the UDP Server will receive messages. The UDP server collects parts of a larger composite message from the clients. The UDP server collects these message parts from different clients and assembles them in prder based on the lSeqNum (defined in the private data structure: udpMessage). The UDP server keeps a running list of all clients that have sent a message to it and will broadcast the composite message to all clients when commanded. The UDP server is able to receive data from clients without blocking the main application thread. The program can respond to user input while handing socket communications at the same time.
