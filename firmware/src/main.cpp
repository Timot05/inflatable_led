#include <Arduino.h>
#include "main.h"

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "../lib/StateMachine/StateMachine.hpp"
#include "../lib/StateMachine/State.hpp"

#include "../lib/bq25601/bq25601.h"

#define PRINT_DEBUG 0

unsigned long lastPressTime = 0;      // Last time the button was pressed

const unsigned long shortPressDuration = 1000;

StateMachine state_machine;
State state;

unsigned int brightness = 1;

uint slice_num;

void handleButtonPress() {
    lastPressTime = millis();
}

void handleButtonRelease() {
    unsigned long currentTime = millis();
    unsigned long pressDuration = currentTime - lastPressTime;

    if (2 < pressDuration < shortPressDuration) {
        state_machine.shortPress();
    }
}

void handleButtonChange() {
    if (digitalRead(BTN_1_PIN) == LOW) {
        handleButtonPress();
    } else {
        handleButtonRelease();
    }
}

// int is active low
bool chr_int = false;
void handleChgInt() {
    chr_int = true;
}


void setup() {
    #if PRINT_DEBUG
    Serial1.begin(115200); // USB serial
    Serial2.setTX(8); // Set TX to GPIO10
    Serial2.setRX(9); // Set RX to GPIO11
    Serial2.begin(115200); // UART serial
    #endif

    pinMode(CHG_STAT_PIN, INPUT);
    pinMode(CHG_PG_PIN, INPUT);
    pinMode(CHG_INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CHG_INT_PIN), handleChgInt, FALLING);
    pinMode(CHG_SDA_PIN, INPUT_PULLUP);
    pinMode(CHG_SCL_PIN, INPUT_PULLUP);

    pinMode(CHG_QON_PIN, OUTPUT);
    digitalWrite(CHG_QON_PIN, HIGH);

    // Setup the led pins
    pinMode(LED_ON_PIN, OUTPUT);
    pinMode(LED_DIM_CTRL_PIN, OUTPUT);

    pinMode(BTN_1_PIN, INPUT_PULLUP);

    // delay(20);
    // Power hold at startup
    pinMode(PWR_HOLD, OUTPUT);
    digitalWrite(PWR_HOLD, HIGH);

    attachInterrupt(digitalPinToInterrupt(BTN_1_PIN), handleButtonChange, CHANGE);

    digitalWrite(LED_ON_PIN, HIGH);

    gpio_set_function(LED_DIM_CTRL_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LED_DIM_CTRL_PIN);
    pwm_set_wrap(slice_num, 12499);


    if(!bq25601_init()) {
        #if PRINT_DEBUG
        Serial.println("Charger init failed!");
        #endif
        bq25601_writeRegister(REG00, 0b00010111);
    }
}

#define BR_1 1
#define BR_2 50
#define BR_3 255

void loop() {
    unsigned long currentTime = millis();

    state = state_machine.getState();

    #if PRINT_DEBUG
    void debugPrint(const char* stateName) {
        Serial.println(stateName);
        Serial2.println(stateName);
    }
    #endif

    switch (state) {
        case State::OFF:
            #if PRINT_DEBUG
            debugPrint("OFF");
            #endif
            digitalWrite(PWR_HOLD, LOW);
            break;

        case State::ON_1:
        case State::ON_2:
        case State::ON_3: {
            #if PRINT_DEBUG
            if (state == State::ON_1) debugPrint("ON_1");
            else if (state == State::ON_2) debugPrint("ON_2");
            else debugPrint("ON_3");
            #endif

            uint8_t brightness = (state == State::ON_1) ? BR_1 :
                                (state == State::ON_2) ? BR_2 : BR_3;

            unsigned int value = map(brightness, 0, 255, 0, 12499);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, value);
            pwm_set_enabled(slice_num, true);
            break;
        }

        case State::CHARGING:
            #if PRINT_DEBUG
            debugPrint("CHARGING");
            #endif
            digitalWrite(PWR_HOLD, LOW);
            digitalWrite(LED_ON_PIN, LOW);
            break;

        default:
            #if PRINT_DEBUG
            debugPrint("UNKNOWN");
            #endif
            break;
    }

    if (digitalRead(BTN_1_PIN) == LOW && shortPressDuration < currentTime - lastPressTime) {
        state_machine.longPress();
    }

    // Handle charger states
    // if (chr_int) {
    //     Serial2.println("Charger interrupt");
    //     Serial2.print("Previous message: ");
    //     Serial2.println(bq25601_readRegister(REG09), BIN);
    //     Serial2.print("Current message: ");
    //     Serial2.println(bq25601_readRegister(REG09), BIN);
    //     chr_int = false;
    // }

    // Serial2.println("Charger interrupt");
    // Serial2.print("Previous message: ");
    // Serial2.println(bq25601_readRegister(REG02), BIN);
    // Serial2.print("Current message: ");
    // Serial2.println(bq25601_readRegister(REG02), BIN);

    bq2501_updateStatus();
    if (bq2501_get_vbus() != NO_INPUT) {
        if (bq2501_get_chrg() == PRE_CHARGE1 || bq2501_get_chrg() == FAST_CHARGE) {
            state_machine.charging();
        }
        else if (bq2501_get_chrg() == CHARGE_DONE) {
            state_machine.charged();
        }
        else {
            state_machine.error();
        }
    }
    else {
        state_machine.not_plugged();
    }

    delay(50);
}
