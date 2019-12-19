#ifndef _WEAPON_CONTROLLER_H_
#define _WEAPON_CONTROLLER_H_

#include "Weapon.h"

#define WEAPON_MIN_DUTYCYCLE 0
#define WEAPON_MAX_DUTYCYCLE 255
#define WEAPON_DUTYCYCLE_RANGE (WEAPON_MAX_DUTYCYCLE - WEAPON_MIN_DUTYCYCLE)

// Conversion between the Remote Control values and the duty cycle
#define WRC_MINVAL  1000
#define WRC_MAXVAL  2000
#define WRCVAL_RANGE (WRC_MAXVAL-WRC_MINVAL)
#define WRCVAL_TO_DUTYCYCLE_RATE ((int)(((double)WEAPON_DUTYCYCLE_RANGE) / ((double)WRCVAL_RANGE) * 1000.0))
#define WRCVAL_TO_DUTYCYCLE_DIV 1000

class RCInput;

class WeaponController {
public:
    WeaponController(Weapon &w): weapon(w) {}

    void processRCInput(const RCInput& rcWeaponSwitch, const RCInput& rcWeaponSpeed);

private:
    Weapon& weapon;
};

#endif
