#include "Adafruit_Keypad.h"

// Hello, welcome to the meta puzzle code, its not too complicated... yet
const int usingSerial = false; //set if testing with serial input

const int redLed = A0;  // "A" means analog pin
const int greenLed = A1;
const int solenoid = 2;
const int speaker = 3; // number means digital pin
String entered = ""; // the password entered so far

const String passcode = "326459"; // the true password  "family"

// Keypad setup (credit to adafruit)
const byte ROWS = 4; // rows
const byte COLS = 3; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {4, 5, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 9, 8}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

keypadEvent e;

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(speaker, OUTPUT);
  pinMode(solenoid, OUTPUT);
  customKeypad.begin();
  reset();
  
  Serial.begin(9600);
  Serial.println("Enter letter:");
}

void loop() {
  // serial testing
  if (usingSerial && Serial.available() > 0) {
    char input = Serial.read();
    if (input == '\n' || input == '\r') return;

    entered = entered + input;
    Serial.print("So far you have entered: " + entered + '\n');
    // Serial.println();

    checkEntered();
    if (Serial.available() == 0) {
      Serial.println("Enter letter:");
    }
  } else { // real keypad
    customKeypad.tick();
    char input = '~'; //this character should never show up
    while(customKeypad.available()){
      e = customKeypad.read();
      input = (char)e.bit.KEY;
      if (input != '~') {
        Serial.print(input);
        if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
        // else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
      }
    }
    if (input != '~' && e.bit.EVENT == KEY_JUST_PRESSED) {
      entered = entered + input;
      Serial.print("So far you have entered: " + entered + '\n');
      checkEntered();
      delay(100);
    }
  }

}

void success() {
  Serial.println("Congratulations, you got the code right!");
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  digitalWrite(solenoid, HIGH);
  
  playNote(392, 120);
  playNote(523, 500);

  delay(3000);
  reset();
  Serial.println("Relocked");
}

void fail() {
  Serial.println("Wrong code, try again");
  digitalWrite(redLed, HIGH);
  playNote(55, 100);
  playNote(55, 400);
  delay(500);
  reset();
}

void reset() {
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(solenoid, LOW);
}

void playNote(int freq, int ms) {
  tone(speaker, freq, ms);
  delay(ms + 20);
}

void clearInput() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void checkEntered() {
  if (entered.length() >= passcode.length()) {
    entered.toUpperCase();
  if (entered == passcode) {
    success();
  } else {
    fail();
  }
  entered = "";
    clearInput();
  }
}

// To do: function that converts the actual keypad input to letters (using adafruit keypad library)
// To do: get input from actual keypad


