#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>

#define CHARGER_I2C_ADDRESS 0x6B

#define REG00 0x00
#define REG01 0x01
#define REG02 0x02
#define REG03 0x03
#define REG04 0x04
#define REG05 0x05
#define REG06 0x06
#define REG07 0x07
#define REG08 0x08
#define REG09 0x09
#define REG0B 0x0B

typedef enum {
    NO_INPUT = 0b000,
    USB_HOST = 0b001,
    ADAPTER = 0b011,
    OTG = 0b111,
    UNKNOWN = 0b101
} bq25601_vbus_stat;

typedef enum {
    NOT_CHARGING = 0b00,
    PRE_CHARGE1 = 0b01,
    FAST_CHARGE = 0b10,
    CHARGE_DONE = 0b11
} bq25601_chrg_stat;

typedef enum {
    POWER_GOOD_NOT_GOOD = 0b0,
    POWER_GOOD = 0b1
} bq25601_pg_stat;

bq25601_vbus_stat vbus_stat;
bq25601_chrg_stat chrg_stat;
bq25601_pg_stat pg_stat;

void bq25601_writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(CHARGER_I2C_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t bq25601_readRegister(uint8_t reg) {
    Wire.beginTransmission(CHARGER_I2C_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(CHARGER_I2C_ADDRESS, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0;
}

bool bq25601_init() {
    Wire.begin();
    uint8_t ret = bq25601_readRegister(REG0B);
    // 0b0010 signature for BQ25601
    uint8_t value = (ret >> 3) & 0b1111;
    if (value == 0b0010) return true;
    return false;
}

void bq2501_updateStatus() {
    uint8_t reg_value = bq25601_readRegister(REG08);

    vbus_stat = (bq25601_vbus_stat) ((reg_value >> 5) & 0x07); // Bits [7:5]
    chrg_stat = (bq25601_chrg_stat) ((reg_value >> 3) & 0x03); // Bits [4:3]
    pg_stat = (bq25601_pg_stat) ((reg_value >> 2) & 0x01);   // Bit 2
}

bq25601_vbus_stat bq2501_get_vbus() {
    return vbus_stat;
}

bq25601_chrg_stat bq2501_get_chrg() {
    return chrg_stat;
}

bq25601_pg_stat bq2501_get_pg() {
    return pg_stat;
}