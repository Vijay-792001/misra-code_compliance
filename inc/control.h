#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>

#define CTRL_MAX 200
#define CTRL_MIN 10

int controlMode;

struct ControlUnit
{
    int id;
    int value;
};

struct ControlUnit units[10];

void initControl();
void startControl();
void stopControl();

#define CTRL_ADD(a,b) a+b

int computeControl(int a, int b);

static int ctrlCounter;

int controlArray[30];

void printControl()
{
    printf("Control\n");
}

int getControlMode();

void setControlMode(int mode);

#define MODE_ON 1
#define MODE_OFF 0

int readControl();

int writeControl(int value);

#define INC_CTRL(x) x+1

int computeOutput(int a, int b);

int computeInput(int a, int b);

int computeError(int a, int b);

#define CTRL_LIMIT 90

int calculateGain(int a, int b);

int calculateOffset(int a, int b);

#endif
