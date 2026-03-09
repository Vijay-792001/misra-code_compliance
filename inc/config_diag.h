// Configuration & Diagnostics Unit Header
#ifndef CONFIG_DIAG_H
#define CONFIG_DIAG_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t cal_low;
    uint8_t cal_high;
    uint8_t config_crc;
} ConfigData_t;

typedef enum {
    DIAG_OK = 0,
    DIAG_NV_ERROR,
    DIAG_TEST_FAIL
} DiagResult_t;

void config_diag_init(void);
const ConfigData_t *get_config(void);
void log_event(uint8_t code);
DiagResult_t invoke_diagnostic(void);

#endif // CONFIG_DIAG_H
