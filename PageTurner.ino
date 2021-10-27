#include <BleKeyboard.h>

#include <ezButton.h>

//thanks https://arduinogetstarted.com/tutorials/arduino-button-long-press-short-press

const int SHORT_PRESS_TIME = 1000; // 1000 milliseconds
const int LONG_PRESS_TIME = 1000; // 1000 milliseconds

ezButton button(32); // create ezButton object that attach to pin 32;

unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
bool isPressing = false;
bool isLongDetected = false;


BleKeyboard bleKeyboard("Tiny Turner", "Arnaud R.", 100);
void setup() {
  Serial.begin(115200);

  //configure debounce
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  //starting BLE broadcasting
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if (bleKeyboard.isConnected()) {

    button.loop(); // MUST call the loop() function first

    if (button.isPressed()) {
      pressedTime = millis();
      isPressing = true;
      isLongDetected = false;
    }

    if (button.isReleased()) {
      isPressing = false;
      releasedTime = millis();

      long pressDuration = releasedTime - pressedTime;

      if (pressDuration < SHORT_PRESS_TIME)
        Serial.println("A short press is detected");
      bleKeyboard.write(KEY_RIGHT_ARROW);
      bleKeyboard.releaseAll();
    }

    if (isPressing == true && isLongDetected == false) {
      long pressDuration = millis() - pressedTime;

      if (pressDuration > LONG_PRESS_TIME) {
        Serial.println("A long press is detected");
        bleKeyboard.write(KEY_DOWN_ARROW);
        bleKeyboard.releaseAll();
        isLongDetected = true;
      }
    }

  }
}
