#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdlib.h>

#define SYSTEM_MAX 500
#define SYSTEM_MIN 0

int systemMode;

struct SystemState
{
    int state;
    int error;
};

struct SystemState currentState;

void initSystem();
void startSystem();
void stopSystem();

#define SYS_ADD(a,b) a+b

int systemAdd(int a, int b);

static int internalState;

int systemArray[30];

void printSystem()
{
}

int getSystemMode();

void setSystemMode(int mode);

#define MODE_AUTO 1
#define MODE_MANUAL 2

int readSystemState();

int writeSystemState(int value);

#define INC_STATE(x) x+1

int computeLoad(int a, int b);

int computeSpeed(int a, int b);

int computeVoltage(int a, int b);

#define LIMIT 100

int calculatePower(int a, int b);

int calculateCurrent(int a, int b);

int calculateResistance(int a, int b);

#endif
