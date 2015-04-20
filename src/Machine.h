#ifndef MACHINE_H
#define MACHINE_H
#include "SerialTypes.h"
#include "Thread.h"

namespace firestep {

#define PIN_X_DIR	2
#define PIN_X		3
#define PIN_Y_DIR	5
#define PIN_Y		6
#define PIN_Z_DIR	9
#define PIN_Z		10


#define ANALOG_SPEED_PIN	5 /* ADC5 (A5) */

#ifdef PRR
/* DIECIMILA */
#define PIN_X_LIM	14
#define PIN_Y_LIM	15
#define PIN_Z_LIM	16
#else
/* MEGA */
#define PIN_X_LIM	54
#define PIN_Y_LIM	55
#define PIN_Z_LIM	56
#endif

//***********************************
// Stuff specific to FirePick Delta (most of this section is a hack)
//***********************************
//#define VACUUM_PIN         30 // Modular Tool #1 for now...
//#define NEOPIXEL_PIN       16 //For LED ring light on end effector
//#define SERVO0_PIN         11 //X [0]
//#define SERVO1_PIN         6  //Y [1]
//#define SERVO2_PIN         5  //Z [2]
//#define SERVO4_PIN         4  //  [3]

//****************************************************************************************
// FirePick Delta EMC01 pin assignment (custom PCB based on ATMega1284P)
//****************************************************************************************
//#ifdef MOTHERBOARD == 639 //RAMPS board
  #define KNOWN_BOARD 1
  
  #define LARGE_FLASH true
  
  //Delta Motor "X" ***********
  #define X_STEP_PIN         23 //X is the BACK motor!
  #define X_DIR_PIN          22
  #define X_ENABLE_PIN       14
  #define X_MIN_PIN          21
  #define X_MAX_PIN          -1
  
  //Delta Motor "Y" ***********
  #define Y_STEP_PIN         4 //Y is the FRONT LEFT motor!
  #define Y_DIR_PIN          3
  #define Y_ENABLE_PIN       14
  #define Y_MIN_PIN          2
  #define Y_MAX_PIN          -1
  
  //Delta Motor "Z" ***********
  #define Z_STEP_PIN         13 //Z is the FRONT RIGHT motor!
  #define Z_DIR_PIN          12
  #define Z_ENABLE_PIN       14
  #define Z_MIN_PIN          11
  #define Z_MAX_PIN          -1
  
  //Modular Tool #1 ***********
  #define E0_STEP_PIN        20
  #define E0_DIR_PIN         15
  #define E0_ENABLE_PIN      1
  #define VACUUM_PIN         30 // Modular Tool #1 for now...
  
  //Modular Tool #2 ***********
  #define E1_STEP_PIN        20
  #define E1_DIR_PIN         15
  #define E1_ENABLE_PIN      0
  #define HEATER_0_PIN       28 //Digital numbering
  #define TEMP_0_PIN         A2 //Analog numbering
  
  //Modular Tool #3 ***********
  #define E2_STEP_PIN        20
  #define E2_DIR_PIN         15
  #define E2_ENABLE_PIN      19
  #define HEATER_1_PIN       27 //Digital numbering
  #define TEMP_1_PIN         A5 //Analog numbering

  //Modular Tool #4 ***********
  #define E3_STEP_PIN        20
  #define E3_DIR_PIN         15
  #define E3_ENABLE_PIN      18
  #define HEATER_2_PIN       25
  #define TEMP_2_PIN         A7 //Analog numbering

  #define HEATER_BED_PIN     -1 //25 //Digital numbering
  #define TEMP_BED_PIN       -1 //A7 //Analog numbering

  //Vestigial Leftovers *******
  #define LED_PIN            -1 //Not used on EMC01
  #define FAN_PIN            -1 //Not used on EMC01
  #define PS_ON_PIN          -1 //Not currently used, but might be on next pcb rev...
  
//#endif //EMC01 pinout

//****************************************************************************************
// RAMPS 1.4
//****************************************************************************************
//#ifdef MOTHERBOARD == 33 //RAMPS board
//  #define KNOWN_BOARD 1
//  
//  #define LARGE_FLASH true
//  
//  #define X_STEP_PIN         54 //A0
//  #define X_DIR_PIN          55 //A1
//  #define X_ENABLE_PIN       38
//  #define X_MIN_PIN           3 //3
//  #define X_MAX_PIN          -1 //2
//  
//  #define Y_STEP_PIN         60 //A6
//  #define Y_DIR_PIN          61 //A7
//  #define Y_ENABLE_PIN       56 //A2
//  #define Y_MIN_PIN          14 //seems fried on current 2560 //14
//  #define Y_MAX_PIN          -1 //15
//  
//  #define Z_STEP_PIN         46
//  #define Z_DIR_PIN          48
//  #define Z_ENABLE_PIN       62 //A8
//  #define Z_MIN_PIN          18 //18
//  #define Z_MAX_PIN          -1 //19 (auto Z leveling probe)
//  
//  #define E0_STEP_PIN        26
//  #define E0_DIR_PIN         28
//  #define E0_ENABLE_PIN      24
//  
//  #define E1_STEP_PIN        36
//  #define E1_DIR_PIN         34
//  #define E1_ENABLE_PIN      30
//  
//  #define LED_PIN            13
//  
//  #define FAN_PIN            -1 // (Sprinter config)
//  
//  #define PS_ON_PIN          12
//  
//  //MOSFETS:
//  #define HEATER_0_PIN       10   // D10 EXTRUDER 1
//  #define HEATER_1_PIN       -1   // D9  (Use this for VACUUM instead!)
//  #define HEATER_2_PIN       -1   // N/C on RAMPS
//  #define HEATER_BED_PIN     8    // BED +12V2, 11A
//  
//  //ANALOG / THERMISTORS:
//  #define TEMP_0_PIN         13   // A13 
//  #define TEMP_1_PIN         15   // A15 
//  #define TEMP_2_PIN         -1   // N/C on RAMPS
//  #define TEMP_BED_PIN       14   // A14 
//#endif //RAMPS pinout


// #define THROTTLE_SPEED /* Throttles driver speed from high (255) to low (0) */

#define A4988_PULSE_DELAY 	DELAY500NS;DELAY500NS
#define DRV8825_PULSE_DELAY DELAY500NS;DELAY500NS;DELAY500NS;DELAY500NS
#define STEPPER_PULSE_DELAY DRV8825_PULSE_DELAY
#define DELTA_COUNT 120
#define MOTOR_COUNT 4
#define AXIS_COUNT 6
#define SEGMENT_COUNT 100

#ifndef DELAY500NS
#define DELAY500NS \
	asm("nop");asm("nop");asm("nop");asm("nop"); asm("nop");asm("nop");asm("nop");asm("nop");
#endif

typedef struct MachineThread : Thread {
    void setup();
    void Heartbeat();
} MachineThread;

template<class T> class Quad {
    public:
        T value[4];
    public:
        Quad(T v1 = 0, T v2 = 0, T v3 = 0, T v4 = 0) {
            value[0] = v1;
            value[1] = v2;
            value[2] = v3;
            value[4] = v4;
        }
        Quad operator+=(Quad<T> that) {
            value[0] += that.value[0];
            value[1] += that.value[1];
            value[2] += that.value[2];
            value[3] += that.value[3];
        }
        bool operator==(Quad<T> that) {
            return value[0] == that.value[0] &&
                   value[1] == that.value[1] &&
                   value[2] == that.value[2] &&
                   value[3] == that.value[3];
        }
};
template<class T1, class T2>
Quad<T1>& operator+=(Quad<T1> &qa, Quad<T2> qb) {
    qa.value[0] += (T1) qb.value[0];
    qa.value[1] += (T1) qb.value[1];
    qa.value[2] += (T1) qb.value[2];
    qa.value[3] += (T1) qb.value[3];
    return qa;
};
template<> inline Quad<int32_t>& operator+=(Quad<int32_t> &qa, Quad<int16_t> qb);

typedef struct CommandParser {
    int cPeek;
    byte peekAvail;

    byte readCommand();
    byte peek(byte c);
    void reset();
} CommandParser;

typedef struct Slack {
    byte maxSlack;
    byte curSlack;
    bool isPlusDelta;

    void init(byte maxSlack);
    int deltaWithBacklash(int delta);
} Slack;

typedef struct SlackVector {
    Slack x;
    Slack y;
    Slack z;
    void init(byte xMax, byte yMax, byte zMax);
} SlackVector;

typedef struct Motor {
    uint8_t	axisMap; 	// index into axis array
    float	stepAngle;
    uint8_t	microsteps;
    uint8_t	polarity;
    uint8_t powerManagementMode;

    Motor() :
        stepAngle(1.8),
        microsteps(16),
        polarity(0),			// 0:clockwise, 1:counter-clockwise
        powerManagementMode(0) 	// 0:off, 1:on, 2:on in cycle, 3:on when moving
    {}
} Motor;

enum AxisMode {
    MODE_DISABLE = 0,
    MODE_STANDARD = 1,
};

typedef class Axis {
    public:
        uint8_t mode;
		uint8_t pinStep;
		uint8_t pinDir;
		uint8_t pinMin;
		uint8_t pinEnable;
        int16_t travelMin;
        int16_t travelMax;
        int16_t searchVelocity;
		int16_t position;
        Axis() :
            mode((uint8_t)MODE_STANDARD),
			pinStep(0),
			pinDir(0),
			pinMin(0),
            travelMin(0),
            travelMax(10000),
            searchVelocity(200),
			position(0)
        {};
} Axis;

typedef class Stroke {
    public:
        uint8_t 		length;			// number of segments
        float 			scale;			// steps per segment
        uint8_t 		curSeg;			// current segment index
        int32_t 		planMicros;		// planned traversal time in microseconds
        Quad<int8_t> 	seg[SEGMENT_COUNT];
		Quad<int16_t>	velocity;
    public:
        Stroke() : length(0), scale(1), curSeg(0), planMicros(1000000) {}
} Stroke;

typedef class Machine {
        friend class Controller;
        friend class JsonController;
    private:
        Motor motor[MOTOR_COUNT];
        Axis axis[AXIS_COUNT];
        Stroke stroke;
		int32_t processMicros;

        float pathPosition;
        SerialInt16 maxPulses;
        SerialInt32 pulses;
        SerialInt32 heartbeats;
        SerialInt32 heartbeatMicros;
        SerialInt32 actualMicros;
        SerialVector32 unitLengthSteps;
        SerialVector32 drivePos;
        bool xReverse;
        bool yReverse;
        bool zReverse;
        union {
            struct {
                SerialInt32 estimatedMicros;
                SerialVector32 startPos;
                SerialVector32 endPos;
                SerialVector32 segmentStart;
                SerialVector32 segmentStartPos;
                SerialVector16 toolVelocity;
            };
            struct {
                SerialVector16 jogDelta;
                SerialInt32 jogFrequency;
                SerialInt32 jogCount;
                bool		jogOverride;
            };
            struct {
                SerialVector32 backlash;
            };
        };
        SerialVectorF drivePathScale;
        SlackVector slack;
        int segmentIndex;
        SerialInt16 deltaCount;
        SerialVector8 deltas[DELTA_COUNT];

    public:
        Machine();
        void init();
        bool doJog();
        bool doAccelerationStroke();
        bool pulseDrivePin(byte stepPin, byte dirPin, byte limitPin, int delta, bool reverse, char axis);
        bool pulseLow(byte stepPin, byte limitPin);
        void sendXYZResponse(struct SerialVector32 *pVector);
        void sendBacklashResponse(struct SerialVector32 *pVector);
} Machine;

typedef class Controller {
    private:
        CommandParser	parser;
        char			guardStart;
        CLOCK			lastClock;
        byte			speed;
        byte			guardEnd;

    public:
        byte 			cmd; // TODO
        Machine 		machine; // TODO
        void init();
        byte readCommand();
        bool processCommand();
        bool readAccelerationStroke();
} Controller;

} // namespace firestep

#endif
