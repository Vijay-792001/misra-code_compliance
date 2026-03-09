// Flap Position Controller Header
#ifndef FLAP_POSITION_CONTROLLER_H
#define FLAP_POSITION_CONTROLLER_H
#include <stdint.h>
#include <stdbool.h>
#include "command_interface.h"
#include "feedback_processing.h"
#include "motor_control.h"

typedef enum {
    MOVE_IDLE = 0,
    MOVE_IN_PROGRESS,
    MOVE_DONE,
    MOVE_LIMIT_ERROR
} MoveStatus_t;

void flap_position_controller_init(void);
void process_command(const Command_t *cmd);
MoveStatus_t get_move_status(void);

#endif // FLAP_POSITION_CONTROLLER_H
