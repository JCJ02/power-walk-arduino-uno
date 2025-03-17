// ARDUINO.ino
unsigned long startTime;
const unsigned long timerDuration = 15 * 60 * 1000; // 15 MINUTES IN MILLISECONDS
#include <LiquidCrystal_I2C.h>

unsigned long previousMillis = 0; // STORE THE LAST TIME THE DISPLAY WAS UPDATED
const long interval = 1000; // INTERVAL OF 1 SECOND (IN MILLISECONDS)
int remainingTime = 900; // TIMER STARTS AT 15 MINUTES (900 SECONDS)
bool onoff = false;
#include <Wire.h>
const int RELAY_PIN2 = 4;
// INITIALIZE THE LIQUID CRYSTAL LIBRARY
// THE FIRST PARAMETER IS THE I2C ADDRESS
// THE SECOND PARAMETER IS HOW MANY ROWS ARE ON YOUR SCREEN
// THE THIRD PARAMETER IS HOW MANY COLUMNS ARE ON YOUR SCREEN
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(RELAY_PIN2, OUTPUT);
  // digitalWrite(RELAY_PIN2, HIGH); // ENSURE RELAY IS OFF INITIALLY
  Serial.begin(115200);
  lcd.init();
  // lcd.begin(16,2); // DEFINING 16 COLUMNS AND 2 ROWS OF LCD DISPLAY
  lcd.backlight(); // TO POWER ON THE BACK LIGHT
  lcd.setCursor(0, 0); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN.
  lcd.print("FREE CHARGE"); // YOU CAN WRITE 16 CHARACTERS PER LINE.
  delay(1000); // DELAY USED TO GIVE A DYNAMIC EFFECT
  lcd.setCursor(0, 1); // DEFINING POSITION TO WRITE FROM SECOND ROW, FIRST COLUMN
  lcd.print("INITIALIZING....");
  delay(1000);
  lcd.clear(); // CLEAR THE SCREEN
}

String data = "";

String removeSpaces(String input) {
  String output = "";
  for (int i = 0; i < input.length(); i++) {
    if (input.charAt(i) != ' ') {
      output += input.charAt(i);
    }
  }
  return output;
}

void resetSystem() {
  lcd.clear(); // CLEAR THE SCREEN
  remainingTime = 900; // RESET TIMER TO 15 MINUTES
  onoff = false; // RESET THE CHARGING STATE
  data = ""; // CLEAR THE RFID DATA
  lcd.setCursor(0, 0); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN
  lcd.print("FREE CHARGE"); // YOU CAN WRITE 16 CHARACTERS PER LINE
  delay(1000); // DELAY USED TO GIVE A DYNAMIC EFFECT
  lcd.setCursor(0, 1); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN
  lcd.print("Scan Your ID");
  digitalWrite(RELAY_PIN2, HIGH);
  delay(1000);
  lcd.clear(); // CLEAR THE SCREEN
}

void loop() {
  // pinMode(RELAY_PIN2, OUTPUT);
  if (Serial.available()) {
    String receivedData = Serial.readString(); // READ DATA AS STRING
    Serial.println("Received Data: " + receivedData);
    data = receivedData;
  }

  String noSpaces = removeSpaces(data);
  Serial.print("Scan: ");
  Serial.println(noSpaces);

  if (data == "") {
    lcd.setCursor(0, 0); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN
    lcd.print("FREE CHARGE"); // YOU CAN WRITE 16 CHARACTERS PER LINE
    delay(1000); // DELAY USED TO GIVE A DYNAMIC EFFECT
    lcd.setCursor(0, 1); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN
    lcd.print("Scan Your ID");
    digitalWrite(RELAY_PIN2, HIGH);
    delay(1000);
    lcd.clear(); // CLEAR THE SCREEN
  } else {

    if (onoff == false) {
      String str = noSpaces; // TEST STRING

      // CHECK IF "YES" IS IN THE STRING
      if (str.indexOf("Yes") != -1) {
        lcd.clear(); // CLEAR THE SCREEN
        lcd.setCursor(0, 0); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN
        lcd.print("STUDENT CHARGING"); // YOU CAN WRITE 16 CHARACTERS PER LINE
        lcd.setCursor(0, 1); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN
        lcd.print("VERIFIED RDID!");
        digitalWrite(RELAY_PIN2, LOW);
        onoff = true;
        Serial.println(data);
        delay(1000); // DELAY USED TO GIVE A DYNAMIC EFFECT
        lcd.clear(); // CLEAR THE SCREEN
      } else {
        // NOT REGISTERED or FULLY CONSUMED
        lcd.setCursor(0, 0); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN
        lcd.print("FREE CHARGE"); // YOU CAN WRITE 16 CHARACTERS PER LINE
        lcd.setCursor(0, 1); // DEFINING POSITION TO WRITE FROM FIRST ROW, FIRST COLUMN
        lcd.print(data);
        delay(1000); // DELAY USED TO GIVE A DYNAMIC EFFECT
        lcd.clear(); // CLEAR THE SCREEN
        digitalWrite(RELAY_PIN2, HIGH);
        delay(1000); // DELAY USED TO GIVE A DYNAMIC EFFECT
        data = "";
        onoff = false;
      }

      if (noSpaces != "Yes") {

      } else {

      }
    }

    if (onoff == true) {
      unsigned long currentMillis = millis(); // GET THE CURRENT TIME
      if (currentMillis - previousMillis >= interval) {
        // SAVE THE LAST TIME THE DISPLAY WAS UPDATED
        previousMillis = currentMillis;

        if (remainingTime > 0) {
          remainingTime--;
        }

        // CALCULATE MINUTES AND SECONDS
        int minutes = remainingTime / 60;
        int seconds = remainingTime % 60;

        // ENSURE FIRST ROW IS CONSISTENT
        lcd.setCursor(0, 0);
        // lcd.print("FREE CHARGE  "); // ENSURE SPACES TO REMOVE LEFTOVER CHARACTERS
        lcd.print("STUDENT CHARGING...");
        // UPDATE ONLY THE TIMER PORTION OF THE LCD
        lcd.setCursor(0, 1);
        lcd.print("TIME LEFT: ");
        if (minutes < 10) lcd.print("0"); // ADD LEADING ZERO IF NEEDED
        lcd.print(minutes);
        lcd.print(":");
        if (seconds < 10) lcd.print("0"); // ADD LEADING ZERO IF NEEDED
        lcd.print(seconds);
        lcd.print("  "); // EXTRA SPACES TO CLEAR OLD CHARACTERS
        Serial.println("Timer");
        if (remainingTime == 0) {
          resetSystem();
        }
      }
    }
  }
}
