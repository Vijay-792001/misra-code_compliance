// Status Indication Unit Header
#ifndef STATUS_INDICATION_H
#define STATUS_INDICATION_H
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    STATUS_IDLE = 0,
    STATUS_MOVING,
    STATUS_ERROR,
    STATUS_STARTUP
} SystemStatus_t;

void status_indication_init(void);
void set_status(SystemStatus_t status);
void report_error(uint8_t error);
void signal_startup(void);

#endif // STATUS_INDICATION_H
