#include <TimerOne.h>
#include "SevSeg.h"

SevSeg sevseg;

int countdown_seconds = 0;
int savedCountdown_seconds = 30;

//// SWITCH ////

static const int buttonPin = 2;                    // switch pin
int buttonStatePrevious = LOW;                      // previousstate of the switch

unsigned long minButtonLongPressDuration = 700;    // Time we wait before we see the press as a long press
unsigned long buttonLongPressMillis;                // Time in ms when we the button was pressed
bool buttonStateLongPress = false;                  // True if it is a long press

const int intervalButton = 50;                      // Time between two readings of the button state
unsigned long previousButtonMillis;                 // Timestamp of the latest reading

unsigned long buttonPressDuration;                  // Time the button is pressed in ms

unsigned long currentMillis;          // Variabele to store the number of milleseconds since the Arduino has started

//// config mode ////

bool configMode = false;
int countdownTimeSelectedInConfigMode = 0;

void sevenSegmentSetup()
{
  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {9, 8, 7, 6, 5, 4, 3, 2};
  byte displayType = COMMON_CATHODE;
  bool resistorsOnSegments = true;
  sevseg.begin(displayType, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(30);
}

// Function for reading the button state
void readButtonState() {

  // If the difference in time between the previous reading is larger than intervalButton
  if(currentMillis - previousButtonMillis > intervalButton) {
    
    // Read the digital value of the button (LOW/HIGH)
    int buttonState = digitalRead(buttonPin);    

    // If the button has been pushed AND
    // If the button wasn't pressed before AND
    // IF there was not already a measurement running to determine how long the button has been pressed
    if (buttonState == HIGH && buttonStatePrevious == LOW && !buttonStateLongPress) {
      buttonLongPressMillis = currentMillis;
      buttonStatePrevious = HIGH;
      Serial.println("Button pressed");

      if (configMode == true)
      {
        savedCountdown_seconds = countdownTimeSelectedInConfigMode;
        countdown_seconds = savedCountdown_seconds;
        sevseg.setNumber(countdown_seconds / 10);
        sevseg.refreshDisplay();
        delay(50);

        Timer1.attachInterrupt(blinkLED); // attach the service routine here

        configMode = false;
        Serial.print("// ******Countdown Timer Updated to: ");
        Serial.println(savedCountdown_seconds);
        Serial.println("// ******EXIT CONFIG MODE ******//");
      }  
    }

    // Calculate how long the button has been pressed
    buttonPressDuration = currentMillis - buttonLongPressMillis;

    // If the button is pressed AND
    // If there is no measurement running to determine how long the button is pressed AND
    // If the time the button has been pressed is larger or equal to the time needed for a long press
    if (buttonState == HIGH && !buttonStateLongPress && buttonPressDuration >= minButtonLongPressDuration) {
      buttonStateLongPress = true;
      Serial.println("Button long pressed");

      configMode = true;
      Timer1.detachInterrupt();
      Serial.println("// ******ENTERED CONFIG MODE ******//");

      // configMode = !configMode;

      // if (configMode == true)
      // {
      //   Timer1.detachInterrupt();
      //   Serial.println("// ******ENTERED CONFIG MODE ******//");
      // }
      // else
      // {
      //   savedCountdown_seconds = countdownTimeSelectedInConfigMode;
      //   countdown_seconds = savedCountdown_seconds;
      //   sevseg.setNumber(countdown_seconds / 10);
      //   sevseg.refreshDisplay();
      //   delay(50);

      //   Timer1.attachInterrupt(blinkLED); // attach the service routine here

      //   configMode = false;
      //   Serial.print("// ******Countdown Timer Updated to: ");
      //   Serial.println(savedCountdown_seconds);
      //   Serial.println("// ******EXIT CONFIG MODE ******//");
      // }
    }
      
    // If the button is released AND
    // If the button was pressed before
    if (buttonState == LOW && buttonStatePrevious == HIGH) {
      buttonStatePrevious = LOW;
      buttonStateLongPress = false;
      Serial.println("Button released");

      // If there is no measurement running to determine how long the button was pressed AND
      // If the time the button has been pressed is smaller than the minimal time needed for a long press
      // Note: The video shows:
      //       if (!buttonStateLongPress && buttonPressDuration < minButtonLongPressDuration) {
      //       since buttonStateLongPress is set to FALSE on line 75, !buttonStateLongPress is always TRUE
      //       and can be removed.
      if (buttonPressDuration < minButtonLongPressDuration) {
        Serial.println("Button pressed shortly");
      }
    }
    
    // store the current timestamp in previousButtonMillis
    previousButtonMillis = currentMillis;

  }

}

void setup() {
  Serial.begin(115200);

  sevenSegmentSetup();

  sevseg.setNumber(savedCountdown_seconds / 10);
  sevseg.refreshDisplay();
  delay(50);

  Timer1.initialize(5000000); // set a timer of length 500000 microseconds (or 0.5 sec)
  Timer1.attachInterrupt(blinkLED); // attach the service routine here

  pinMode(A0, INPUT);
}

void loop() {
  //savedCountdown_seconds = 50;
  
  if (Serial.available()) { // check if data has been sent to the serial port
    Serial.println("Serial.available()");
    char c = Serial.read(); // read the incoming byte
    switch (c) {
      case '1':
        savedCountdown_seconds = 10;
        Serial.println("case '1':");
        break;
      case '2':
        savedCountdown_seconds = 20;
        Serial.println("case '2':");
        break;
      case '3':
        savedCountdown_seconds = 30;
        Serial.println("case '3':");
        break;
      case '4':
        savedCountdown_seconds = 40;
        Serial.println("case '4':");
        break;
      case '5':
        savedCountdown_seconds = 50;
        Serial.println("case '5':");
        break;
      case '6':
        savedCountdown_seconds = 60;
        Serial.println("case '6':");
        break;
      case '7':
        savedCountdown_seconds = 70;
        Serial.println("case '7':");
        break;
      case '8':
        savedCountdown_seconds = 80;
        Serial.println("case '8':");
        break;
      case '9':
        savedCountdown_seconds = 90;
        Serial.println("case '9':");
        break;
      default:
        Serial.println("invalid number");
        break;
  }
      countdown_seconds = savedCountdown_seconds;
      sevseg.setNumber(countdown_seconds / 10);
      sevseg.refreshDisplay();
      delay(50);
      //Serial.print(serialNum);
  }

  currentMillis = millis();    // store the current time
  readButtonState();           // read the button state

  if (configMode == true)
  {
    sevseg.blank();
    delay(80);

    currentMillis = millis();    // store the current time
    readButtonState();           // read the button state
    // sevseg.setNumber(savedCountdown_seconds / 10);
    // delay(200);
    // sevseg.blank();
    // delay(200);
    // sevseg.setNumber(savedCountdown_seconds / 10);
    // delay(200);

    int potValue = analogRead(A0);
    countdownTimeSelectedInConfigMode  = map(potValue, 1023, 0, 1, 9) * 10;

    sevseg.setNumber(countdownTimeSelectedInConfigMode / 10);
    sevseg.refreshDisplay();
    delay(50);
  }

  //Serial.println(savedCountdown_seconds);
  delay(50);
  // your program here...
}

void blinkLED() {
  countdown_seconds -= 5;

  if (countdown_seconds <= 0)
  {
      Serial.print("Dandan! Its time to eat! XDD");
      Serial.println(savedCountdown_seconds);
      countdown_seconds = savedCountdown_seconds;
  }
  sevseg.setNumber(countdown_seconds / 10);
  sevseg.refreshDisplay();
  delay(50);

  Serial.print("countdown_seconds: ");
  Serial.println(countdown_seconds);
}
