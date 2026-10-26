// Hello, welcome to the meta puzzle code, its not too complicated... yet
const int redLed = A0;  // "A" means analog pin
const int greenLed = A2;
const int speaker = 9; // number means digital pin
String entered = ""; // the password entered so far

const String passcode = "FAMILY"; // the true password

void setup() {
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(speaker, OUTPUT);
  reset();
  
  Serial.begin(9600);
  Serial.println("Enter letter:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.length() <= 0) return;
    if (input.length() > 1) {
      Serial.print("One letter at a time\n");
      return;
    }
    entered = entered + input;
    Serial.print("So far you have entered: " + entered + '\n');
    // Serial.println();

    if (entered.length() >= passcode.length()) {
      entered.toUpperCase();
      if (entered == passcode) {
        success();
      } else {
        fail();
      }
      entered = "";
    }
    Serial.println("Enter letter:");
  }
}

void success() {
  Serial.println("Congratulations, you got the code right!");
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  
  playNote(392, 120);
  playNote(523, 500);

  //unlock solenoid TBD

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
}

void playNote(int freq, int ms) {
  tone(speaker, freq, ms);
  delay(ms + 20);
}

// To do: function that converts the actual keypad input to letters (using adafruit keypad library)
// To do: get input from actual keypad
// To do: solenoid lock/unlock code


