// Command Interface Unit Header
#ifndef COMMAND_INTERFACE_H
#define COMMAND_INTERFACE_H
#include <stdint.h>
#include <stdbool.h>

#define CMD_BUFFER_SIZE 8

typedef enum {
    CMD_NO_ERROR = 0,
    CMD_FORMAT_ERROR,
    CMD_RANGE_ERROR
} CommandError_t;

typedef struct {
    uint8_t target_position;
    bool    valid;
    CommandError_t error_code;
} Command_t;

void command_interface_init(void);
void command_check_uart(void);
bool get_validated_command(Command_t *cmd);

#endif // COMMAND_INTERFACE_H
