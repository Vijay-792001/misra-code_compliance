#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>

#define PORT 8080
#define MAX_CLIENTS 50

int networkStatus;

struct Packet
{
    int id;
    char data[256];
};

struct Packet packetBuffer[10];

void initNetwork();
void startNetwork();
void stopNetwork();

#define NET_ADD(a,b) a+b

int computePacket(int a, int b);

static int netCounter;

int connectionArray[20];

void printNetwork()
{
    printf("Network\n");
}

int openConnection();

int closeConnection();

#define NET_ENABLE 1
#define NET_DISABLE 0

int getConnectionStatus();

void setConnectionStatus(int status);

#define TIMEOUT 30

int sendPacket();

int receivePacket();

#define DOUBLE_PACKET(x) x*2

int computeLatency(int a, int b);

int computeBandwidth(int a, int b);

int computeDelay(int a, int b);

#endif
