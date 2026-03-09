// Flap Position Controller
#include "flap_position_controller.h"
#include <string.h>

static uint8_t current_position = 0;
static uint8_t target_position = 0;
static MoveStatus_t move_status = MOVE_IDLE;

void flap_position_controller_init(void) {
    current_position = 0;
    target_position = 0;
    move_status = MOVE_IDLE;
}

void process_command(const Command_t *cmd) {
    if (!cmd || !cmd->valid) {
        move_status = MOVE_IDLE;
        return;
    }
    if (cmd->target_position > 100) {
        move_status = MOVE_LIMIT_ERROR;
    } else {
        target_position = cmd->target_position;
        move_status = MOVE_IN_PROGRESS;
        // Request motor start: forward/reverse
        // Direction logic based on position difference
        int8_t dir = ((int16_t)target_position - (int16_t)current_position) > 0 ? 1 : -1;
        motor_drive(dir);
    }
}

MoveStatus_t get_move_status(void) {
    // Simplified: compare target and current
    if (current_position == target_position) {
        move_status = MOVE_DONE;
    }
    return move_status;
}
