#ifndef _DRIVER_CONTROLLER_H_
#define _DRIVER_CONTROLLER_H_

#define RC_IDLE_MINVAL 1430
#define RC_IDLE_MAXVAL 1550

#define MAX_DUTYCYCLE 225u
#define MIN_DUTYCYCLE 50u
#define DUTYCYCLE_RANGE (MAX_DUTYCYCLE - MIN_DUTYCYCLE)

// Conversion between the Remote Control values and the duty cycle
#define RCVAL_RANGE 500u
#define RCVAL_TO_DUTYCYCLE_RATE ((unsigned int)(((double)DUTYCYCLE_RANGE) / ((double)RCVAL_RANGE) * 1000.0))
#define RCVAL_TO_DUTYCYCLE_DIV 1000u

enum class RCPos { FirstHalf, SecondHalf, Center };
enum class SystemState { PoweredOff, PowerOffInProcess, Running };

class WheelMotor;

class DriveController
{
  public:
    DriveController(WheelMotor &m1, WheelMotor &m2):
        m1(m1),
        m2(m2),
        sysState(SystemState::PoweredOff) {}

    void processInput(int speedInput, int dirInput);

private:
    void mapRCValueToPercent(unsigned int value, unsigned int& percent, RCPos& pos);
    void mapRCValueToDutyCycle(unsigned int value, unsigned int& dutyCycle, RCPos& pos);

    MotorDirection mapPosToMotorDir(RCPos pos) {
        switch (pos) {
            case RCPos::FirstHalf: return MotorDirection::Reverse;
            case RCPos::SecondHalf: return MotorDirection::Forward;
            default: return MotorDirection::Forward;
        }
    }

private:
    SystemState sysState;
    unsigned long powerOffEndTime;
    WheelMotor &m1;
    WheelMotor &m2;
};

#endif
