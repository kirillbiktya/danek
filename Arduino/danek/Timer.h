#ifndef Timer_h
#define Timer_r

#include "Arduino.h"

class Timer
{
public:
	Timer(unsigned long p) {
		this->_period = p;
		this->_start = millis();
	}

	bool check() {
		if (millis() - this->_start >= this->_period) {
			this->_start = millis();
			return true;
		}
		else {
			return false;
		}
	}

private:
	unsigned long _start;
	unsigned long _period;
};

#endif