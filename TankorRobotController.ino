#include "Debug.h"
#include "RCInput.h"
#include "WheelMotor.h"
#include "DriveController.h"
#include "WeaponController.h"

#ifdef ARDUINO

#if defined(ARDUINO_AVR_MEGA2560)

#define M1_ENABLE_PIN 12
#define M1_DIR_PIN    11
#define M1_PWM_PIN    10
#define M2_ENABLE_PIN 3
#define M2_DIR_PIN    4
#define M2_PWM_PIN    5

#define W_ENABLE_PIN 8
#define W_PWM_PIN    9

#elif defined(ARDUINO_AVR_UNO)

#define M1_ENABLE_PIN 8
#define M1_DIR_PIN    9
#define M1_PWM_PIN    10
#define M2_ENABLE_PIN 3
#define M2_DIR_PIN    4
#define M2_PWM_PIN    5

#define W_ENABLE_PIN 7
#define W_PWM_PIN    6

#else

#error "This arduino board is not supported"

#endif


const byte ledPin = 14;

#else

// For Energia

DECLARE_RCINPUT(wheelSpeedInput, PB_3);
DECLARE_RCINPUT(turnInput, PC_4);
DECLARE_RCINPUT(weaponSpeedInput, PB_7);
DECLARE_RCINPUT(weaponSwitchInput, PB_6);

#define M1_ENABLE_PIN PC_6
#define M1_DIR_PIN    PC_7
#define M1_PWM_PIN    PD_6
#define M2_ENABLE_PIN PD_1
#define M2_DIR_PIN    PD_2
#define M2_PWM_PIN    PD_3

#define W_ENABLE_PIN PB_0
#define W_PWM_PIN    PD_0

const byte ledPin = GREEN_LED;

#endif

WheelMotor m1(M1_ENABLE_PIN, M1_DIR_PIN, M1_PWM_PIN, MotorLocation::Right);
WheelMotor m2(M2_ENABLE_PIN, M2_DIR_PIN, M2_PWM_PIN, MotorLocation::Left);
Weapon weapon(W_ENABLE_PIN, W_PWM_PIN);

DriveController driveCtrl(m1, m2);
WeaponController weaponCtrl(weapon);
    
void setup() {
    DEBUG_INIT();
    pinMode(ledPin, OUTPUT);

    wheelSpeedInput.startDataAcquisition();
    turnInput.startDataAcquisition();
    weaponSpeedInput.startDataAcquisition();
    weaponSwitchInput.startDataAcquisition();

    DEBUG_PRINT("Starting Tankor controller ...");
}

void loop() {
    
    wheelSpeedInput.acquireData();
    turnInput.acquireData();
    weaponSpeedInput.acquireData();
    weaponSwitchInput.acquireData();

    if (!wheelSpeedInput.hasError() && !turnInput.hasError()) {
        driveCtrl.processInput(wheelSpeedInput.getMicrosec(), turnInput.getMicrosec());
    } else {
      /*if (wheelSpeedInput.hasError()) {
          Serial.println("Error on Channel 1"); 
      }
      if (turnInput.hasError()) {
          Serial.println("Error on Channel 2"); 
      }*/
    }

   #ifdef DEBUG 
   if (weaponSpeedInput.valueChanged()) {
        Serial.print("Channel Weapon Speed: ");
        Serial.println(weaponSpeedInput.getMicrosec());
   } else {
        if (weaponSpeedInput.hasError()) {
            Serial.println("Error on Channel Weapon Speed");
        }
   }
   if (weaponSwitchInput.valueChanged()) {
        Serial.print("Channel Weapon Switch: ");
        Serial.println(weaponSwitchInput.getMicrosec());
   } else {
       if (weaponSwitchInput.hasError()) {
          Serial.println("Error on Channel Weapon Switch"); 
      }
   }
   #endif
   
   weaponCtrl.processRCInput(weaponSwitchInput, weaponSpeedInput);
   delay(20);
}
