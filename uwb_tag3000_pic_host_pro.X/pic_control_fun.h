#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>
#define RESET()	asm("reset")
#define CLRWDT() asm("clrwdt")
#define SLEEP()  asm("sleep")
#define PRINT_PMIC_STATUS   0
#define PMIC_STATSU_READ_TIME 50//300 // 30sec

#define HW_VERSION    5
#define PRINT_LOG     0
#define DEV_MODE      0

#define PW_CONNECTED    1
#define PW_DISCONNECTED 2

#define MSSP1_SPI   1
#define MSSP1_I2C   2

#define TX_INTERVAL_MS 300
#define PMIC_READ_INCREMENT_VAL (TX_INTERVAL_MS / 100)
#define TX_LED_TOGGLE_INCREMENT_VAL (TX_INTERVAL_MS / 100)

#define TX_LED_TOGGLE_COUNT 30

#define LOW_BATTERY 25

extern uint8_t battery_level;
extern uint32_t pic_time_counter_ms;

void process_pmic_status(uint8_t * status, uint8_t length);
void pmic_status_read(void);
uint8_t lipo_battery_level(uint16_t battery_volt);
void set_MSSSP1_mode(uint8_t mode);
void set_led_status(void);