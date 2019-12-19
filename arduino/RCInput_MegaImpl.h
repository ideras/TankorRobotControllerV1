#ifndef _RCINPUT_MEGAIMPL_H_

#define _RCINPUT_MEGAIMPL_H_

#include <Arduino.h>
#include <pins_arduino.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>

struct RCChannelData {
    RCChannelData(int pinNumber);
    
    void enableInterrupt() {
        // Pin change interrupt setup
        *pcicr |= pcicr_bitmask;
    }

    uint8_t readPinValue() {
        return (*in_port) & pin_bitmask;
    }

    uint8_t *pcicr;
    uint8_t pcicr_bitmask;
    uint8_t *in_port;
    uint8_t pin_bitmask;

    unsigned long start_time;
    byte pin_prev_state;
    int curr_value;
};

#endif
