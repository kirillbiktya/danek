#include "DanekController.h"

struct Command {
  uint8_t command_type;
  uint8_t param_number;
  uint16_t value; 
};

DanekController controller = DanekController();

void setup() {
   Serial.begin(9600);
   controller.setup();
   Serial.println("Controller setup done.");
}

void loop() {
  if (Serial.available()) {
    Command command;
    Serial.readBytes((char *)&command, sizeof(Command));

    switch (command.command_type) {
      case 1: // set value
        switch (command.param_number) {
          case 0: // currentHour
            controller.setCurrentHour(command.value);
            break;
          case 1: // currentMinute
            controller.setCurrentMinute(command.value);
            break;
          case 2: // currentSecond
            controller.setCurrentSecond(command.value);
            break;
          case 3:
            if (command.value) {
              controller.setManualLightOn();
            } else {
              controller.setManualLightOff();
            }
            break;
          case 4: // manualLightR
            controller.setManualLightR(command.value);
            break;
          case 5: // manualLightG
            controller.setManualLightG(command.value);
            break;
          case 6: // manualLightB
            controller.setManualLightB(command.value);
            break;
          case 7:
            controller.showManualLight();
            break;
          default:
            Serial.println("Not found.");
            break;
        }
      default: // get value
        switch (command.param_number) {
          default:
            Serial.print("R: ");
            Serial.print(controller.getManualLightR());
            Serial.print(" G: ");
            Serial.print(controller.getManualLightG());
            Serial.print(" B: ");
            Serial.println(controller.getManualLightB());
            break;
        }
    }
  }

  controller.run_iteration();
}
