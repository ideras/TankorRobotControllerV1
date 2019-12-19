#ifndef _WEAPON_H_
#define _WEAPON_H_

class Weapon {
public:
    Weapon(int enPin, int pwmPin):
        state(false),
        speed(0),
        enPin(enPin),
        pwmPin(pwmPin) {
        pinMode(enPin, OUTPUT);
        pinMode(pwmPin, OUTPUT);
        digitalWrite(enPin, LOW);
    }

    void powerOn() {
        state = true;
        digitalWrite(enPin, HIGH);
    }

    void powerOff() {
        state = false;
        digitalWrite(enPin, LOW);
    }

    void setSpeed(int _speed) {
        speed =_speed;
        analogWrite(pwmPin, speed);
    }

    bool isOn() { return state; }
    bool isOff() { return !state; }
    int getSpeed() { return speed; }

private:
    bool state;
    int speed;
    int enPin;
    int pwmPin;
};

#endif
