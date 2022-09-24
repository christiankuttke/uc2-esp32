/*
  This map is for relatively common ESP32 boards replicating the form factor of Arduino UNO.
  This map allows use of such uno-compatible board with very popular
  "Protoneer Arduino CNC shield" and is based on its pinout.
  This makes perfect match for retrofiting older Arduino+GRBL based machines
  with 32b microcontroler capable of running grblHAL and providing few extra IO pins (eg. for modbus).
  These boards are sold under several names, for instance:
   + ESPDUINO-32  (USB type B)
   + Wemos D1 R32 (Micro USB)
*/


#define BOARD_NAME "ESPDUINO-32 Wemos D1 R32"

// timer definitions
#define STEP_TIMER_GROUP TIMER_GROUP_0
#define STEP_TIMER_INDEX TIMER_0

// Define step pulse output pins.
#define X_STEP_PIN          GPIO_NUM_26
#define Y_STEP_PIN          GPIO_NUM_25
#define Z_STEP_PIN          GPIO_NUM_17
#define A_STEP_PIN          GPIO_NUM_19

// Define step direction output pins. NOTE: All direction pins must be on the same port.
#define X_DIRECTION_PIN     GPIO_NUM_16
#define Y_DIRECTION_PIN     GPIO_NUM_27
#define Z_DIRECTION_PIN     GPIO_NUM_14
#define A_DIRECTION_PIN     GPIO_NUM_18

#define X_END_STOP          GPIO_NUM_13 // arduino 9
#define X_END_STOP          GPIO_NUM_5  // arduino 10
#define X_END_STOP          GPIO_NUM_23  // arduino 11


// Define stepper driver enable/disable output pin(s).
#define STEPPERS_ENABLE_PIN GPIO_NUM_12

// Define homing/hard limit switch input pins and limit interrupt vectors.
#define X_LIMIT_PIN         GPIO_NUM_13
#define Y_LIMIT_PIN         GPIO_NUM_5
#define Z_LIMIT_PIN         GPIO_NUM_23

// Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_PIN  GPIO_NUM_18
#define SPINDLEPWMPIN       GPIO_NUM_19

// Define flood enable output pin.
#define COOLANT_FLOOD_PIN   GPIO_NUM_32

// Define user-control CONTROLs (cycle start, reset, feed hold) input pins.
#define RESET_PIN           GPIO_NUM_2
#define FEED_HOLD_PIN       GPIO_NUM_4
#define CYCLE_START_PIN     GPIO_NUM_35

// Define probe switch input pin.
#define PROBE_PIN           GPIO_NUM_39

#define UART2_RX_PIN            GPIO_NUM_33
#define UART2_TX_PIN            GPIO_NUM_32
#define MODBUS_DIRECTION_PIN    GPIO_NUM_15
#define MODBUS_BAUD             19200


// Pin mapping when using SPI mode.
// With this mapping, SD card can be used both in SPI and 1-line SD mode.
// Note that a pull-up on CS line is required in SD mode.
#define PIN_NUM_MISO        GPIO_NUM_19
#define PIN_NUM_MOSI        GPIO_NUM_23
#define PIN_NUM_CLK         GPIO_NUM_18
#define PIN_NUM_CS          GPIO_NUM_5
