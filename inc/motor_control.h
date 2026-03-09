// Motor Control Unit Header
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#include <stdint.h>

typedef enum {
    MOTOR_STOPPED = 0,
    MOTOR_RUNNING_FWD,
    MOTOR_RUNNING_REV,
    MOTOR_FAULT
} MotorState_t;

typedef struct {
    MotorState_t state;
    uint8_t config;
    uint8_t fault_flags;
} MotorControlStatus_t;

void motor_control_init(void);
void motor_drive(int8_t direction);
MotorControlStatus_t get_fault_flags(void);

#endif // MOTOR_CONTROL_H
