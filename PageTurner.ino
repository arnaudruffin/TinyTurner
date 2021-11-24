#include <BleKeyboard.h>

#include <ezButton.h>

//thanks https://arduinogetstarted.com/tutorials/arduino-button-long-press-short-press

const int LONG_PRESS_TIME = 600; // 1000 milliseconds

ezButton buttonRight(32); // create ezButton object that attach to pin 32;
ezButton buttonLeft(33);

unsigned long pressedTimeRight = 0;
unsigned long releasedTimeRight = 0;
bool isPressingRight = false;
bool isLongDetectedRight = false;

unsigned long pressedTimeLeft = 0;
unsigned long releasedTimeLeft = 0;
bool isPressingLeft = false;
bool isLongDetectedLeft = false;

BleKeyboard bleKeyboard("Tiny Turner", "Arnaud R.", 100);
void setup() {
  Serial.begin(115200);

  // set debounce time to 50 milliseconds
  buttonRight.setDebounceTime(50);
  buttonLeft.setDebounceTime(50);

  //starting BLE broadcasting
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if (bleKeyboard.isConnected()) {

    buttonRight.loop(); // MUST call the loop() function first
    buttonLeft.loop(); // MUST call the loop() function first

    if (buttonRight.isPressed()) {
      pressedTimeRight = millis();
      isPressingRight = true;
      isLongDetectedRight = false;
    }
    if (buttonLeft.isPressed()) {
      pressedTimeLeft = millis();
      isPressingLeft = true;
      isLongDetectedLeft = false;
    }

    if (buttonRight.isReleased()) {
      isPressingRight = false;
      releasedTimeRight = millis();

      long pressDurationRight = releasedTimeRight - pressedTimeRight;

      if (pressDurationRight < LONG_PRESS_TIME) {
        Serial.println("A short press is detected on Right");
        bleKeyboard.write(KEY_RIGHT_ARROW);
        bleKeyboard.releaseAll();
      }
    }
    if (buttonLeft.isReleased()) {
      isPressingLeft = false;
      releasedTimeLeft = millis();

      long pressDurationLeft = releasedTimeLeft - pressedTimeLeft;

      if (pressDurationLeft < LONG_PRESS_TIME) {
        Serial.println("A short press is detected on Left");
        bleKeyboard.write(KEY_LEFT_ARROW);
        bleKeyboard.releaseAll();
      }
    }

    if (isPressingRight == true && isLongDetectedRight == false) {
      long pressDurationRight = millis() - pressedTimeRight;

      if (pressDurationRight > LONG_PRESS_TIME) {
        Serial.println("A long press is detected on Right");
        bleKeyboard.write(KEY_DOWN_ARROW);
        bleKeyboard.releaseAll();
        isLongDetectedRight = true;
      }
    }

    if (isPressingLeft == true && isLongDetectedLeft == false) {
      long pressDurationLeft = millis() - pressedTimeLeft;

      if (pressDurationLeft > LONG_PRESS_TIME) {
        Serial.println("A long press is detected on Left");
        bleKeyboard.write(KEY_UP_ARROW);
        bleKeyboard.releaseAll();
        isLongDetectedLeft = true;
      }
    }
    // handleButton(buttonRight, KEY_RIGHT_ARROW, KEY_DOWN_ARROW,pressedTimeRight, releasedTimeRight,isPressingRight,isLongDetectedRight);
    // handleButton(buttonLeft, KEY_LEFT_ARROW, KEY_UP_ARROW,pressedTimeLeft, releasedTimeLeft,isPressingLeft,isLongDetectedLeft);
  }

}
