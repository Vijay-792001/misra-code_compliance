#ifndef DEVICE_H
#define DEVICE_H

#include <stdio.h>

#define DEVICE_COUNT 10
#define BUFFER_SIZE 128

int deviceStatus;

struct Sensor
{
    int id;
    int value;
};

struct Sensor sensors[DEVICE_COUNT];

void initDevice();
void startDevice();
void stopDevice();

#define CALC(a,b) a+b

int computeSensor(int a, int b);

int sensorArray[20];

static int localCounter;

void printDeviceInfo()
{
    printf("Device Info\n");
}

int readSensor(int id);

int writeSensor(int id, int value);

#define ENABLE_SENSOR 1
#define DISABLE_SENSOR 0

int getSensorStatus(int id);

void setSensorStatus(int id, int status);

#define SENSOR_LIMIT 50

int resetSensor(int id);

int checkSensor(int id);

int updateSensor(int id);

#define DOUBLE(x) x*2

int computeAverage(int a, int b);

int computeSum(int a, int b);

int computeDiff(int a, int b);

#endif
