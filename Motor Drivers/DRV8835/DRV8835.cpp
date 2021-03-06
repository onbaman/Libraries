#include "DRV8835.h"

// Note: This class currently only supports Phase/Enable mode

DRV8835::DRV8835(int phase1, int enable1, int phase2, int enable2, int mode) {
	phase = new int[NUM_MOTORS];
	phase[0] = phase1;
	phase[1] = phase2;
	enable = new int[NUM_MOTORS];
	enable[0] = enable1;
	enable[1] = enable2;
	modePin = mode;
	modeState = true;
	inverted = new bool[NUM_MOTORS];
	inverted[0] = false;
	inverted[1] = false;
}

DRV8835::DRV8835(int phase1, int enable1, int phase2, int enable2, bool mode) {
	phase = new int[NUM_MOTORS];
	phase[0] = phase1;
	phase[1] = phase2;
	enable = new int[NUM_MOTORS];
	enable[0] = enable1;
	enable[1] = enable2;
	modeState = mode;
	modePin = -1;
	inverted = new bool[NUM_MOTORS];
	inverted[0] = false;
	inverted[1] = false;
}

void DRV8835::init() {
	for (int i=0; i<NUM_MOTORS; i++) {
		pinMode(phase[i], OUTPUT);
		pinMode(enable[i], OUTPUT);
	}
	if (modePin >= 0) {
		pinMode(modePin, OUTPUT);
		digitalWrite(modePin, modeState);
	}
}

void DRV8835::setSpeed(int motor, int speed) {
	if (motor < 0 || motor >= NUM_MOTORS) return;
	speed = constrain(speed, -255, 255) * (inverted[motor] ? -1 : 1);
	digitalWrite(phase[motor], speed > 0 ? HIGH : LOW);
	analogWrite(enable[motor], abs(speed));
}

void DRV8835::setInverted(int motor, bool invert) {
	inverted[motor] = invert;
}

/**	
 *	08/24/17 - Rupesh Chinta: Created Library
 *  10/03/17 - Rupesh Chinta: Added constructor for no mode pin
 */