#pragma once



//default Pin structure
struct PinConfig
{
    //motor x direction pin
    int MOTOR_X_DIR = 0;
    //motor x step pin
    int MOTOR_X_STEP = 0;
    //motor y direction pin
    int MOTOR_Y_DIR = 0;
    //motor y step pin
    int MOTOR_Y_STEP = 0;
    //motor z direction pin
    int MOTOR_Z_DIR = 0;
    //motor z step pin
    int MOTOR_Z_STEP = 0;
    //motor a direction pin
    int MOTOR_A_DIR = 0;
    //motor a step pin
    int MOTOR_A_STEP = 0;
    //motor enable power 
    int MOTOR_ENABLE = 0;
    //motor power pin is inverted
    bool MOTOR_ENABLE_INVERTED = false;

    //led control pin
    int LED_PIN = 0;
    //led count from the strip
    int LED_COUNT = 0;

    //anlog joystick x pin
    int ANLOG_JOYSTICK_X = 0;
    //analog joystick y pin
    int ANLOG_JOYSTICK_Y = 0;

    String PSX_MAC = "";
    int PSX_CONTROLLER_TYPE = 0; //1 = ps3, 2 =ps4
};

struct XYZ_MOTOR_JOYSTICK : PinConfig
{
    int MOTOR_X_DIR = 16;
    int MOTOR_X_STEP = 26;
    int MOTOR_Y_DIR = 27;
    int MOTOR_Y_STEP = 25;
    int MOTOR_Z_DIR = 14;
    int MOTOR_Z_STEP = 17;
    int MOTOR_ENABLE = 12;
    bool MOTOR_ENABLE_INVERTED = true;

    int ANLOG_JOYSTICK_X = 35;
    int ANLOG_JOYSTICK_Y = 34;

    String PSX_MAC = "4c:63:71:cd:31:a0";
    int PSX_CONTROLLER_TYPE = 2;
};

struct X_MOTOR_64LED : PinConfig
{
    int MOTOR_X_DIR = 21;
    int MOTOR_X_STEP = 19;
    int MOTOR_ENABLE = 18;
    bool MOTOR_ENABLE_INVERTED = true;

    int LED_PIN = 27;
    int LED_COUNT = 64;
};

const XYZ_MOTOR_JOYSTICK pinConfig;