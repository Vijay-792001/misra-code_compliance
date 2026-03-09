// Feedback Processing Unit
#include "feedback_processing.h"
// [TBD — Not found in technical source] ADC peripheral details

static uint16_t last_adc = 0;
static SensorStatus_t sensor_status = SENSOR_OK;

void feedback_processing_init(void) {
    last_adc = 0;
    sensor_status = SENSOR_OK;
}

uint16_t get_position(void) {
    // [TBD — Not found in technical source] Read ADC
    return last_adc;
}

SensorStatus_t get_sensor_status(void) {
    // [TBD — Not found in technical source] Sensor plausibility/check
    return sensor_status;
}
