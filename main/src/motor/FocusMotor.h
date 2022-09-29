#include "../../config.h"
#ifdef IS_MOTOR
#pragma once
#include "../../config.h"
#include "AccelStepper.h"
#include "ArduinoJson.h"
#include "../../pinstruct.h"
#if defined IS_PS3 || defined IS_PS4
#include "../gamepads/ps_3_4_controller.h"
#endif
#include "../wifi/WifiController.h"
#include "../config/ConfigController.h"

struct MotorData
{
    long speed = 0;
    long maxspeed = 20000;
    long acceleration = 0;
    long currentPosition = 0;
    long targetPosition = 0;
    int SIGN = 1;
    bool isforever = false;
};

struct MotorPins
{
    int STEP;
    int DIR;
    int ENABLE;
};

enum Stepper
{
    A,
    X,
    Y,
    Z
};

class FocusMotor
{

public:
    bool DEBUG = false;

// for stepper.h
#define MOTOR_STEPS 200
#define SLEEP 0
#define MS1 0
#define MS2 0
#define MS3 0
#define RPM 120

    bool isaccel = false;
    bool isforever = false;
    bool motor_enable = false;
    bool isBusy = false;

    // global variables for the motor

    boolean isstop = 0;

    int MOTOR_ACCEL = 5000;
    int MOTOR_DECEL = 5000;

    int isabs = true;
    int isen = false;
    bool isactive = false;
    static const int FULLSTEPS_PER_REV_A = 200;
    static const int FULLSTEPS_PER_REV_X = 200;
    static const int FULLSTEPS_PER_REV_Y = 200;
    static const int FULLSTEPS_PER_REV_Z = 200;

    long MAX_VELOCITY_A = 20000;
    long MAX_VELOCITY_X = 20000;
    long MAX_VELOCITY_Y = 20000;
    long MAX_VELOCITY_Z = 20000;
    long MAX_ACCELERATION_A = 100000;
    long MAX_ACCELERATION_X = 100000;
    long MAX_ACCELERATION_Y = 100000;
    long MAX_ACCELERATION_Z = 100000;

    std::array<AccelStepper *, 4> steppers;
    std::array<MotorData *, 4> data;
    std::array<MotorPins, 4> pins;

    void act();
    void set();
    void get();
    void setup();
    bool background();
private:
    void stopAllDrives();
    void startAllDrives();
};

extern FocusMotor motor;

#endif