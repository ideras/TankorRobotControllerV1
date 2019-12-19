#include "Debug.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <Energia.h>
#endif

#include "RCInput.h"
#include "WeaponController.h"

void WeaponController::processRCInput(const RCInput& rcWeaponSwitch, const RCInput& rcWeaponSpeed) {
    if (!rcWeaponSwitch.hasError() && rcWeaponSwitch.valueChanged()) {
        unsigned long val = rcWeaponSwitch.getMicrosec();

        if (val > 1700 && weapon.isOn()) {
            DEBUG_PRINT("Weapon off: ");
            DEBUG_PRINT(val);
            DEBUG_PRINT(' ');
            DEBUG_PRINT("Current Speed: ");
            DEBUG_PRINTLN(weapon.getSpeed());
            
            weapon.setSpeed(0);
            weapon.powerOff();
        } else if (val < 1300 && weapon.isOff()) {
            DEBUG_PRINT("Weapon on: ");
            DEBUG_PRINTLN(val);
            
            weapon.setSpeed(WEAPON_MIN_DUTYCYCLE); // ~ 1V
            weapon.powerOn();
        }
    }
    if (!rcWeaponSpeed.hasError() && rcWeaponSpeed.valueChanged() && weapon.isOn()) {
        unsigned long val = rcWeaponSpeed.getMicrosec();

        unsigned long speed;
        if (val > WRC_MINVAL) {
            speed = ((val-WRC_MINVAL)*WRCVAL_TO_DUTYCYCLE_RATE)/WRCVAL_TO_DUTYCYCLE_DIV + WEAPON_MIN_DUTYCYCLE;
            if (speed > 255) {
                speed = 255;
            }
        } else {
            speed = WEAPON_MIN_DUTYCYCLE;
        }
        weapon.setSpeed(speed);

        DEBUG_PRINT("Weapon speed: ");
        DEBUG_PRINTLN(speed);
    }
}
