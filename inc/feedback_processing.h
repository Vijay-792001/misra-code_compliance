// Feedback Processing Unit Header
#ifndef FEEDBACK_PROCESSING_H
#define FEEDBACK_PROCESSING_H
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SENSOR_OK = 0,
    SENSOR_RANGE_ERROR,
    SENSOR_FAULT
} SensorStatus_t;

void feedback_processing_init(void);
uint16_t get_position(void);
SensorStatus_t get_sensor_status(void);

#endif // FEEDBACK_PROCESSING_H
