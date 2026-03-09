// Status Indication Unit
#include "status_indication.h"
// [TBD — Not found in technical source] UART, LED interface details

static SystemStatus_t sys_status = STATUS_IDLE;

void status_indication_init(void) {
    sys_status = STATUS_STARTUP;
    // [TBD — Not found in technical source] Initialize LEDs/UART
}

void set_status(SystemStatus_t status) {
    sys_status = status;
    // [TBD — Not found in technical source] Update/out
}

void report_error(uint8_t error) {
    // [TBD — Not found in technical source] Error propagation
}

void signal_startup(void) {
    sys_status = STATUS_STARTUP;
}
