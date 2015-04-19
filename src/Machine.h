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

// #define THROTTLE_SPEED /* Throttles driver speed from high (255) to low (0) */

#define A4988_PULSE_DELAY 	DELAY500NS;DELAY500NS
#define DRV8825_PULSE_DELAY DELAY500NS;DELAY500NS;DELAY500NS;DELAY500NS
#define STEPPER_PULSE_DELAY DRV8825_PULSE_DELAY
#define DELTA_COUNT 120
#define MOTOR_COUNT 4
#define AXIS_COUNT 6
#define POSITION_TYPE int16_t
#define SEGMENT_COUNT 100

#ifndef DELAY500NS
#define DELAY500NS \
	asm("nop");asm("nop");asm("nop");asm("nop"); asm("nop");asm("nop");asm("nop");asm("nop");
#endif

typedef struct MachineThread : Thread {
    void setup();
    void Heartbeat();
} MachineThread;

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
        int16_t travelMin;
        int16_t travelMax;
        int16_t searchVelocity;
        Axis() :
            mode((uint8_t)MODE_STANDARD),
            travelMin(0),
            travelMax(10000),
            searchVelocity(200)
        {};
} Axis;

template<class T>
class Position {
    public:
        T x;
        T y;
        T z;
        T a;
        T b;
        T c;
		Position() :x(0),y(0),z(0),a(0),b(0),c(0) {}
};

typedef class StrokeSegment {
	public:
		int8_t d[4];
} StrokeSegment;

typedef class Stroke {
    public:
        uint8_t 		length;			// number of segments
        float 			scale;			// steps per segment
        uint8_t 		curSeg;			// current segment index
        uint32_t 		usPath;			// planned traversal time in microseconds
        StrokeSegment 	seg[SEGMENT_COUNT];
    public:
        Stroke() : length(0), scale(1), curSeg(0), usPath(1000000) {}
        inline bool add(int8_t d1 = 0, int8_t d2 = 0, int8_t d3 = 0, int8_t d4 = 0) {
            if (length + 1 >= SEGMENT_COUNT) {
                return false;
            }
            int8_t *pd = seg[length].d;
            *pd++ = d1;
            *pd++ = d2;
            *pd++ = d3;
            *pd++ = d4;
            length++;
            return true;
        }
} Stroke;

typedef class Machine {
        friend class Controller;
        friend class JController;
    private:
        Motor motor[MOTOR_COUNT];
        Axis axis[AXIS_COUNT];
        Position<POSITION_TYPE> machinePosition;
        Stroke stroke;

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
