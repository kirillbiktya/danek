#ifndef DanekController_h
#define DanekController_h

unsigned long TIMER_1S = 1000;
unsigned long TIMER_1M = 60000;
unsigned long TIMER_1H = 3600000;


#define ledModulePin 11
#define ledModuleClasterCount 7
#define moistureSensorPin 14
#define waterLevelSensorPin 12
#define pumpControlPin 10
#define buzzerPin 15
#define lcdModuleI2CAddress 0x27
#define lcdModuleCols 16
#define lcdModuleRows 2

#include "Arduino.h"
#include "Timer.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

class DanekController
{
public:
	DanekController() {};

	void setup() 
	{
		pinMode(waterLevelSensorPin, INPUT_PULLUP);
		pinMode(pumpControlPin, OUTPUT);

		ledModule.begin();
		lcdModule.init();
		lcdModule.backlight();
	};

	void run_iteration() {
		if (second_timer.check()) {
			if (currentSecond == 59) {
				currentSecond = 0;
				if (currentMinute == 59) {
					currentMinute = 0;
					if (currentHour == 23) {
						currentHour = 0;
					}
					else {
						currentHour += 1;
					}

					hourPassed();
				}
				else {
					currentMinute += 1;
				}

				minutePassed();
			}
			else {
				currentSecond += 1;
			}

			secondPassed();
		}

	};

#pragma region getters_setters
	uint8_t getLightStartHour() {
		return lightStartHour;
	}

	void setLightStartHour(uint8_t value) {
		lightStartHour = value;
	}
	
	uint8_t getLightStopHour() {
		return lightStopHour;
	}
	
	void setLightStopHour(uint8_t value) {
		lightStopHour = value;
	}
	
	int getMinimalMoistureValue() {
		return minimalMoistureValue;
	}
	
	void setMinimalMoistureValue(int value) {
		minimalMoistureValue = value;
	}
	
	int getMaximalMoistureValue() {
		return maximalMoistureValue;
	}
	
	void setMaximalMoistureValue(int value) {
		maximalMoistureValue = value;
	}
	
	int getOptimalMoistureValue() {
		return optimalMoistureValue;
	}
	
	void setOptimalMoistureValue(int value) {
		optimalMoistureValue = value;
	}

	int getCurrentHour() {
		return currentHour;
	}

	void setCurrentHour(int value) {
		currentHour = value;
	}
	
	int getCurrentMinute() {
		return currentMinute;
	}
	
	void setCurrentMinute(int value) {
		currentMinute = value;
	}
	
	int getCurrentSecond() {
		return currentSecond;
	}
	
	void setCurrentSecond(int value) {
		currentSecond = value;
	}

	void getCurrentMoisture() {
		return currentMoisture;
	}

	void setManualLightR(uint8_t value) {
		manualLightColorR = value;
	}

	uint8_t getManualLightR() {
		return manualLightColorR;
	}
	
	void setManualLightG(uint8_t value) {
		manualLightColorG = value;
	}

	uint8_t getManualLightG() {
		return manualLightColorG;
	}
	
	void setManualLightB(uint8_t value) {
		manualLightColorB = value;
	}

	uint8_t getManualLightB() {
		return manualLightColorB;
	}

	void setManualLightOn() {
		manualLightActive = true;
	}

	void setManualLightOff() {
		manualLightActive = false;
	}

  void showManualLight() {
    setLedModuleColor(ledModule.Color(manualLightColorR, manualLightColorG, manualLightColorB));
  }

	bool getManualLight() {
		return manualLightActive;
	}
#pragma endregion

private:
	uint8_t lightStartHour = 5;
	uint8_t lightStopHour = 20;

	uint16_t minimalMoistureValue = 650;
	uint16_t optimalMoistureValue = 700;
	uint16_t maximalMoistureValue = 750;

	uint8_t currentHour = 0;
	uint8_t currentMinute = 0;
	uint8_t currentSecond = 0;

	uint16_t currentMoisture = 0;

	bool pumpActive = false;

	bool buzzerActive = false;

	bool cuckooActive = false;
	uint8_t cuckooStep = 0;

	bool manualLightActive = false;
	uint8_t manualLightColorR = 0;
	uint8_t manualLightColorG = 0;
	uint8_t manualLightColorB = 0;


	Adafruit_NeoPixel ledModule = Adafruit_NeoPixel(ledModuleClasterCount, ledModulePin, NEO_GRB + NEO_KHZ800);
	LiquidCrystal_I2C lcdModule = LiquidCrystal_I2C(lcdModuleI2CAddress, lcdModuleCols, lcdModuleRows);

	Timer second_timer = Timer(TIMER_1S);

	int getMoistureValue() {
		return analogRead(moistureSensorPin);
	};

	bool getWaterLevelValue() {
		return not digitalRead(waterLevelSensorPin);
	};

	void sprinkle() {
		pumpActive = true;
    setLedModuleColor(ledModule.Color(0, 0, 255));
		digitalWrite(pumpControlPin, 1);
	}

	void dry() {
		pumpActive = false;
		digitalWrite(pumpControlPin, 0);
	}

	void setLedModuleColor(uint32_t c) {
      for (uint8_t i = 0; i < ledModule.numPixels(); i++) {
        if (ledModule.getPixelColor(i) != c) {
          ledModule.setPixelColor(i, c);
        }
      }
      ledModule.show();
	}

	void lcdModulePrintChar(char value, uint8_t row, uint8_t col) {
		lcdModule.setCursor(col, row);
		lcdModule.write(value);
	}

	void lcdModulePrintTime() {
		lcdModulePrintChar(currentHour / 10 + '0', 0, 4);
		lcdModulePrintChar(currentHour % 10 + '0', 0, 5);
		lcdModulePrintChar(':', 0, 6);
		lcdModulePrintChar(currentMinute / 10 + '0', 0, 7);
		lcdModulePrintChar(currentMinute % 10 + '0', 0, 8);
		lcdModulePrintChar(':', 0, 9);
		lcdModulePrintChar(currentSecond / 10 + '0', 0, 10);
		lcdModulePrintChar(currentSecond % 10 + '0', 0, 11);
	}

	void lcdModulePrintMoistureValue() {
		lcdModulePrintChar('M', 1, 1);
		lcdModulePrintChar('V', 1, 2);
		lcdModulePrintChar(':', 1, 3);
		if (currentMoisture > 0) {
			lcdModulePrintChar(currentMoisture / 100 + '0', 1, 5);
			lcdModulePrintChar(currentMoisture % 100 / 10 + '0', 1, 6);
			lcdModulePrintChar(currentMoisture % 10 + '0', 1, 7);
		}
		else {
			lcdModulePrintChar(' ', 1, 5);
			lcdModulePrintChar(' ', 1, 6);
			lcdModulePrintChar(' ', 1, 7);
		}
	}

	void lcdModulePrintWaterLevel() {
		lcdModulePrintChar('W', 1, 9);
		lcdModulePrintChar('L', 1, 10);
		lcdModulePrintChar(':', 1, 11);
		bool waterLevel = getWaterLevelValue();
		if (waterLevel) {
      lcdModulePrintChar(' ', 1, 12);
			lcdModulePrintChar('O', 1, 13);
			lcdModulePrintChar('K', 1, 14);
			lcdModulePrintChar(' ', 1, 15);
		}
		else {
			lcdModulePrintChar('E', 1, 12);
			lcdModulePrintChar('R', 1, 13);
			lcdModulePrintChar('R', 1, 14);
		}
		
	}

	void buzz() {
		buzzerActive = true;
    setLedModuleColor(ledModule.Color(255, 0, 0));
		tone(buzzerPin, 500);
	}

	void cuckoo() {
		uint8_t stepCount = 0;

		if (currentHour == 0 or currentHour == 12) {
			stepCount = 12;
		}
		else {
			stepCount = currentHour % 12;
		}

		if (cuckooStep < stepCount) {
			cuckooStep += 1;
      setLedModuleColor(ledModule.Color(255, 255, 0));
			tone(buzzerPin, 900);
			delay(200);
			noTone(buzzerPin);
			delay(20);
      setLedModuleColor(ledModule.Color(255, 0, 255));
			tone(buzzerPin, 600);
			delay(200);
			noTone(buzzerPin);
      setLedModuleColor(ledModule.Color(0, 0, 0));
		}
		else {
			cuckooActive = false;
			cuckooStep = 0;
		}
	}

	void secondPassed() {
		lcdModulePrintTime();

		if (pumpActive) {
			currentMoisture = getMoistureValue();
			if (currentMoisture > optimalMoistureValue) {
				dry();
			}
		}

		if (buzzerActive) {
			buzzerActive = false;
			noTone(buzzerPin);
		}

		if (cuckooActive) {
			cuckoo();
		}
	};

	void minutePassed() {
		currentMoisture = getMoistureValue();

		lcdModulePrintWaterLevel();
		lcdModulePrintMoistureValue();

    if (not manualLightActive) {
      if (currentHour >= lightStartHour and currentHour < lightStopHour) {
        setLedModuleColor(ledModule.Color(255, 255, 255));
      }
      else {
        setLedModuleColor(ledModule.Color(0, 0, 0));
      }
    } else {
      setLedModuleColor(ledModule.Color(manualLightColorR, manualLightColorG, manualLightColorB));
    }

		if (currentMoisture <= minimalMoistureValue) {
			if (getWaterLevelValue()) {
				sprinkle();
			}
			else {
				buzz();
			}
		}
	};

	void hourPassed() {
		cuckooActive = true;
	};
};
#endif