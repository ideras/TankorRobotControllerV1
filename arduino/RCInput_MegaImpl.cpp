#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_UNO)

#include <Arduino.h>
#include "RCInput_MegaImpl.h"

RCChannelData::RCChannelData(int pinNumber) {
    uint8_t *pcmskr = digitalPinToPCICR(pinNumber);
    uint8_t pcmskr_bitmask = _BV(digitalPinToPCICRbit(pinNumber));
    
    // Pin change interrupt setup
    *pcmskr |= pcmskr_bitmask;
    pcicr = digitalPinToPCMSK(pinNumber);
    pcicr_bitmask = _BV(digitalPinToPCMSKbit(pinNumber));
    in_port = portInputRegister(digitalPinToPort(pinNumber));
    pin_bitmask = digitalPinToBitMask(pinNumber);
    
    // Set the pin as input
    pinMode(pinNumber, INPUT_PULLUP);
}

// 4 channels
#if defined(ARDUINO_AVR_MEGA2560)

#define CH1_PIN 50
#define CH2_PIN 51
// #define CH3_PIN 52
#define CH3_PIN 13
#define CH4_PIN 53

#elif defined(ARDUINO_AVR_UNO)

#define CH1_PIN A0
#define CH2_PIN A1
#define CH3_PIN A2
#define CH4_PIN A3

#endif

volatile RCChannelData ch1(CH1_PIN);
volatile RCChannelData ch2(CH2_PIN);
volatile RCChannelData ch3(CH3_PIN);
volatile RCChannelData ch4(CH4_PIN);

inline void updateChannel(volatile RCChannelData &d, unsigned long time_us) {
    if (d.pin_prev_state == 0 && (d.readPinValue() != 0)) {
        d.pin_prev_state = 1;
        d.start_time = time_us;
    } else if (d.pin_prev_state == 1 && (d.readPinValue() == 0)) {
        d.pin_prev_state = 0;
        d.curr_value = time_us - d.start_time;
    }
}

ISR(PCINT0_vect) {
    unsigned long startTime = micros();

    updateChannel(ch1, startTime);
    updateChannel(ch2, startTime);
    updateChannel(ch3, startTime);
    updateChannel(ch4, startTime);
}
#endif
