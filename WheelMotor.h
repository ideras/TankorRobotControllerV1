#ifndef _WHEEL_MOTOR_H_
#define _WHEEL_MOTOR_H_

#include "Arduino.h"

enum class MotorDirection { Forward, Reverse };
enum class MotorLocation { Left, Right };

class WheelMotor {
public:
    WheelMotor(int enablePin, int dirPin, int pwmPin, MotorLocation loc):
        enablePin(enablePin),
        dirPin(dirPin),
        pwmPin(pwmPin),
        enabled(false),
        dir(MotorDirection::Forward),
        speed(0),
        loc(loc) {
        pinMode(enablePin, OUTPUT);
        pinMode(dirPin, OUTPUT);
        pinMode(pwmPin, OUTPUT);
        
        digitalWrite(enablePin, LOW);
    }

    void powerOn() {
        digitalWrite(enablePin, HIGH);
        enabled = true;
    }

    void powerOff() {
        digitalWrite(enablePin, LOW);
        enabled = false;
    }

    void setSpeed(int _speed) {
        speed = _speed;
        analogWrite(pwmPin, _speed);
    }

    void setDir(MotorDirection _dir) {
        dir = _dir;
        int pinVal = dir == MotorDirection::Forward? HIGH : LOW;
        digitalWrite(dirPin, loc==MotorLocation::Right? pinVal : !pinVal);
    }

    MotorLocation getLocation() { return loc; }
    bool isOn() { return enabled; }
    bool isOff() { return !enabled; }
    int getSpeed() { return speed; }
    MotorDirection getDir() { return dir; }

private:
    bool enabled;
    MotorDirection dir;
    int speed;
    MotorLocation loc;
    int enablePin;
    int dirPin;
    int pwmPin;
};
#endif
