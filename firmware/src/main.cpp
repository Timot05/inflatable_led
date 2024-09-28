#include <Arduino.h>
#include "main.h"

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "../lib/sleep/sleep.h"

#include "hardware/rtc.h"

#include "../lib/StateMachine/StateMachine.hpp"
#include "../lib/StateMachine/State.hpp"

#define PRINT_DEBUG FALSE

unsigned long lastPressTime = 0;      // Last time the button was pressed

const unsigned long shortPressDuration = 500;

StateMachine state_machine;

unsigned int brightness = 0;

uint slice_num;

void handleButtonPress() {
    lastPressTime = millis();
    state_machine.update();
}

void handleButtonRelease() {
    unsigned long currentTime = millis();
    unsigned long pressDuration = currentTime - lastPressTime;

    if (pressDuration < shortPressDuration) {
        state_machine.shortPress();
    }
    else {
        state_machine.longPress();
    }
}

void handleButtonChange() {
    if (digitalRead(ON_OFF_BT_PIN) == LOW) {
        handleButtonPress();
    } else {
        handleButtonRelease();
    }
}

void setup() {

    pinMode(LED_ON_PIN, OUTPUT);
    // pinMode(LED_DIM_CTRL_PIN, OUTPUT);
    pinMode(ON_OFF_BT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ON_OFF_BT_PIN), handleButtonChange, CHANGE);

    Serial.begin(115200); // USB serial

    // digitalWrite(LED_ON_PIN, HIGH);
    // analogWrite(LED_DIM_CTRL_PIN, brightness);  // Set initial brightness

    gpio_set_function(LED_DIM_CTRL_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LED_DIM_CTRL_PIN);
    pwm_set_wrap(slice_num, 12499);

}

void loop() {
    State state = state_machine.getState();
    #if PRINT_DEBUG
    Serial.print("State: ");
    #endif

    if (state == State::OFF) {
        #if PRINT_DEBUG
        Serial.println("OFF");
        #endif

        digitalWrite(LED_ON_PIN, HIGH);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
        pwm_set_enabled(slice_num, true);
    }
    else if (state == State::ON) {
        #if PRINT_DEBUG
        Serial.println("ON");
        #endif

        digitalWrite(LED_ON_PIN, LOW);
        unsigned int value = map(brightness, 0, 255, 0, 12499);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, value);
        pwm_set_enabled(slice_num, true);
    }
    else if (state == State::INCREMENT) {
        #if PRINT_DEBUG
        Serial.println("INCREMENT");
        #endif

        brightness += 1;
        if (brightness > 255) {
            brightness = 0;
        }
        unsigned int value = map(brightness, 0, 255, 0, 12499);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, value);
        pwm_set_enabled(slice_num, true);
    }
    #if PRINT_DEBUG
    Serial.print("Brightness: ");
    Serial.println(brightness);
    #endif
    delay(10);
}
