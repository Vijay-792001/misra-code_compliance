// Main Entry Point
#include "command_interface.h"
#include "flap_position_controller.h"
#include "motor_control.h"
#include "feedback_processing.h"
#include "status_indication.h"
#include "config_diag.h"

int main(void) {
    command_interface_init();
    flap_position_controller_init();
    motor_control_init();
    feedback_processing_init();
    status_indication_init();
    config_diag_init();

    // Run self-diagnostic at startup
    invoke_diagnostic();
    signal_startup();

    for (;;) {
        command_check_uart();
        Command_t cmd;
        if (get_validated_command(&cmd)) {
            process_command(&cmd);
        }
        MoveStatus_t move_status = get_move_status();
        set_status(move_status == MOVE_IN_PROGRESS ? STATUS_MOVING : STATUS_IDLE);
        // Error/status checks, feedback, and periodic can be placed here
    }
    return 0;
}
