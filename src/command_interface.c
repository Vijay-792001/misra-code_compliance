// Command Interface Unit
#include "command_interface.h"
#include <string.h>

static uint8_t rx_buffer[CMD_BUFFER_SIZE];
static Command_t last_cmd = {0};

void command_interface_init(void) {
    memset(rx_buffer, 0, sizeof(rx_buffer));
    last_cmd.valid = false;
}

static bool uart_read(uint8_t *data, uint8_t *len) {
    // [TBD — Not found in technical source] UART RX implementation is MCU-specific
    // Dummy stub for compilation
    return false;
}

void command_check_uart(void) {
    uint8_t len = 0;
    if (uart_read(rx_buffer, &len) && len == 2) { // Assume [cmd][target_pos]
        last_cmd.target_position = rx_buffer[1];
        if (last_cmd.target_position > 100) { // Example range limit
            last_cmd.valid = false;
            last_cmd.error_code = CMD_RANGE_ERROR;
        } else {
            last_cmd.valid = true;
            last_cmd.error_code = CMD_NO_ERROR;
        }
    } else {
        last_cmd.valid = false;
        last_cmd.error_code = CMD_FORMAT_ERROR;
    }
}

bool get_validated_command(Command_t *cmd) {
    if (cmd) {
        memcpy(cmd, &last_cmd, sizeof(Command_t));
        return last_cmd.valid;
    }
    return false;
}
