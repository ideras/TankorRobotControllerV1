#ifndef _RCPIN_H_
#define _RCPIN_H_

#include <stdint.h>

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_UNO)
#include "arduino/RCInput_MegaImpl.h"
#else
#error "Implement RC for this board"
#endif

inline unsigned long time_diff(unsigned long t1, unsigned long t2) {
    if (t1 > t2) {
        return t1 - t2;
    } else {
        return t2 - t1;
    }
}

class RCInput {
public:
    RCInput(volatile RCChannelData &rc_channel): 
        prevValueMicrosec(0),
        currValueMicrosec(0),
        error(false),
        valChanged(false),
        rc_channel(rc_channel) {}
    
    void startDataAcquisition() {
        rc_channel.enableInterrupt();
    }

    void acquireData() {
        currValueMicrosec = rc_channel.curr_value;
        error = !(currValueMicrosec >= 900 && currValueMicrosec <= 2100);
        valChanged = (!error && time_diff(prevValueMicrosec, currValueMicrosec) > 20);
        
        if (valChanged) {
            lastDiff = time_diff(prevValueMicrosec, currValueMicrosec);
            prevValueMicrosec = currValueMicrosec;
        }
    }

    bool hasError() { return error; }
    bool valueChanged() { return valChanged; }
    unsigned long getLastDiff() { return lastDiff; }
    unsigned long getMicrosec() { return currValueMicrosec; }
    
private:
    unsigned long lastDiff;
    unsigned long prevValueMicrosec;
    unsigned long currValueMicrosec;
    bool error;
    bool valChanged;
    volatile RCChannelData &rc_channel;
};

extern RCInput wheelSpeedInput;
extern RCInput turnInput;
extern RCInput weaponSpeedInput;
extern RCInput weaponSwitchInput;

#endif
