#include "Debug.h"

#include "WheelMotor.h"
#include "DriveController.h"

void DriveController::mapRCValueToPercent(unsigned int value, unsigned int& percent, RCPos& pos) {
    if (value < 1000) value = 1000;
    if (value > 2000) value = 2000;

    if (value > RC_IDLE_MAXVAL) {
        pos = RCPos::FirstHalf;
        percent = ((value - 1500) * 100) / 500;
    } else if (value < RC_IDLE_MINVAL) {
        pos = RCPos::SecondHalf;
        percent = ((1500 - value) * 100) / 500;
    } else {
        pos = RCPos::Center;
        percent = 0;
    }   
}


void DriveController::mapRCValueToDutyCycle(unsigned int value, unsigned int& dutyCycle, RCPos& pos) {
    if (value < 1000) value = 1000;
    if (value > 2000) value = 2000;

    uint32_t dc = 0;
    if (value > RC_IDLE_MAXVAL) {
        pos = RCPos::FirstHalf;
        dc = ((static_cast<uint32_t>(value) - 1500ul) * RCVAL_TO_DUTYCYCLE_RATE) / RCVAL_TO_DUTYCYCLE_DIV + MIN_DUTYCYCLE;
    } else if (value < RC_IDLE_MINVAL) {
        pos = RCPos::SecondHalf;
        dc = ((1500ul - static_cast<uint32_t>(value)) * RCVAL_TO_DUTYCYCLE_RATE) / RCVAL_TO_DUTYCYCLE_DIV + MIN_DUTYCYCLE;
    } else {
        pos = RCPos::Center;
    }
    dutyCycle = static_cast<unsigned int>(dc);
}

void DriveController::processInput(int speedInput, int dirInput) {
    unsigned int dutyCycle, percent;
    RCPos p1, p2;

    mapRCValueToPercent(dirInput, percent, p2);
    mapRCValueToDutyCycle(speedInput, dutyCycle, p1);

    if ((p1 != RCPos::Center) && (p2 == RCPos::Center)) { 
        // Forward or Reverse
        MotorDirection md = mapPosToMotorDir(p1);

        if (m1.getDir() != md) {
            // Direction change require a motor stop
            DEBUG_PRINTLN("**********Direction change");
            if (m1.getSpeed() != 0) {
                m1.setSpeed(0);
                m2.setSpeed(0);
                delay(300);
            }
            m1.setDir(md);
            m2.setDir(md);
        }
        if (m1.isOff()) {
            m1.powerOn();
            m2.powerOn();
        }
        m1.setSpeed(dutyCycle);
        m2.setSpeed(dutyCycle);

        DEBUG_PRINT( (md == MotorDirection::Forward)? "Forward :" : "Reverse :");
        DEBUG_PRINT(dutyCycle);
        DEBUG_PRINT(' ');
        DEBUG_PRINT(speedInput);
        DEBUG_PRINTLN();
        
        sysState = SystemState::Running;
    }
    else if ((p1 == RCPos::Center) && (p2 != RCPos::Center)) { 
        // Turning on robot center
        if (m1.isOff()) {
            m1.powerOn();
            m2.powerOn();
        }
        dutyCycle = (percent * DUTYCYCLE_RANGE) / 100 + MIN_DUTYCYCLE;
        
        MotorDirection md = mapPosToMotorDir(p2);
        m1.setDir(md==MotorDirection::Forward? MotorDirection::Reverse : MotorDirection::Forward);
        m2.setDir(md);
        m1.setSpeed(dutyCycle);
        m2.setSpeed(dutyCycle);
        sysState = SystemState::Running;
        
        DEBUG_PRINT("Turning on robot center: ");
        DEBUG_PRINTLN(dutyCycle);
    }
    else if ((p1 != RCPos::Center) && (p2 != RCPos::Center)) { 
        // Robot is moving: Turning on a wheel
        int dc1, dc2;

        if (p2 == RCPos::FirstHalf) { // RIGHT
            // Reduce the duty cycle on the right wheel by the value of the second pot
            dc1 = ((100 - percent) * (dutyCycle - MIN_DUTYCYCLE)) / 100 + MIN_DUTYCYCLE;
            dc2 = dutyCycle;
        }
        else { // LEFT
          // Reduce the duty cycle on the left wheel by the value of the second pot
            dc1 = dutyCycle;
            dc2 = ((100 - percent) * (dutyCycle - MIN_DUTYCYCLE)) / 100 + MIN_DUTYCYCLE;
        }

        if (m1.isOff()) {
            m1.powerOn();
            m2.powerOn();            
        }
        
        DEBUG_PRINT("Turning on a wheel (");
        DEBUG_PRINT(dc1);
        DEBUG_PRINT(',');
        DEBUG_PRINT(dc2);
        DEBUG_PRINT(')');
        DEBUG_PRINT(dirInput);
        DEBUG_PRINTLN();
        
        MotorDirection md = mapPosToMotorDir(p1);
        m1.setDir(md);
        m2.setDir(md);

        if (md == MotorDirection::Forward) {
            m1.setSpeed(dc1);
            m2.setSpeed(dc2);
        } else { // Reverse
            m1.setSpeed(dc2);
            m2.setSpeed(dc1);
        }

        sysState = SystemState::Running;
    }
    else { // Both positions at the center
        switch (sysState) {
            case SystemState::PowerOffInProcess:
                if (millis() >= powerOffEndTime) {
                    m1.setSpeed(0);
                    m2.setSpeed(0);
                    m1.powerOff();
                    m2.powerOff();
                    sysState = SystemState::PoweredOff;      
                    
                    DEBUG_PRINTLN("System is powered off");              
                }
                break;
           case SystemState::Running:
              m1.setSpeed(5);
              m2.setSpeed(5);
              sysState = SystemState::PowerOffInProcess;
              powerOffEndTime = millis() + 5000;
              
              DEBUG_PRINTLN("Power off started");
              break;
           default:
              break;
        }
    }
}
