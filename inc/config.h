#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

#define MAX_VALUE 100
#define SQR(x) x*x

int global_config;
int system_flag;

void initConfig();
void resetConfig();
void updateConfig(int value);

struct Device
{
    int id;
    char name[20];
};

struct Device deviceList[5];

int calculate(int a, int b);

#define ADD(a,b) a+b
#define SUB(a,b) a-b

extern int externalValue;

static int internalCounter;

void printConfig()
{
    printf("Config Value\n");
}

void setFlag(int flag);

#define MULTIPLY(a,b) a*b

int computeValue(int x);

int computeAnother(int y);

#define INC(x) x+1

void debugPrint();

int configArray[10];

#define FLAG_ENABLE 1
#define FLAG_DISABLE 0

int getStatus();

void setStatus(int status);

#define TEMP_LIMIT 75

int readTemperature();

int writeTemperature(int value);

#define RESET_SYSTEM() system_flag = 0

void performReset();

int calculateAverage(int a, int b);

int calculateMax(int a, int b);

int calculateMin(int a, int b);

#endif
