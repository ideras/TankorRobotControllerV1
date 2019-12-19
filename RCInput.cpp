#include "RCInput.h"

extern volatile RCChannelData ch1;
extern volatile RCChannelData ch2;
extern volatile RCChannelData ch3;
extern volatile RCChannelData ch4;

RCInput turnInput(ch1);
RCInput wheelSpeedInput(ch2);
RCInput weaponSpeedInput(ch3);
RCInput weaponSwitchInput(ch4);

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_UNO)
#include "arduino/RCInput_MegaImpl.cpp"
#else
#error "Implement RC for this board"
#endif
