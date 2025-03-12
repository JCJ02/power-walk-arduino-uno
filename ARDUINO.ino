
unsigned long startTime;
const unsigned long timerDuration = 15 * 60 * 1000;  // 15 minutes in milliseconds
#include <LiquidCrystal_I2C.h>

unsigned long previousMillis = 0; // Store the last time the display was updated
const long interval = 1000; // Interval of 1 second (in milliseconds)
int remainingTime = 900; // Timer starts at 15 minutes (900 seconds)
bool onoff=false;
#include <Wire.h>
const int RELAY_PIN2 = 4;
//initialize the liquid crystal library
//the first parameter is  the I2C address
//the second parameter is how many rows are on your screen
//the  third parameter is how many columns are on your screen
LiquidCrystal_I2C lcd(0x27,  16, 2);

void setup() {
  pinMode(RELAY_PIN2, OUTPUT);
  // digitalWrite(RELAY_PIN2, HIGH); // Ensure relay is OFF initially
  Serial.begin(115200);  
  lcd.init();
  // lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light

  lcd.setCursor(0,0); //Defining positon to write from first row,first column .
  lcd.print("FREE CHARGE"); //You can write 16 Characters per line .
  delay(1000);//Delay used to give a dynamic effect
  lcd.setCursor(0,1);  //Defining positon to write from second row,first column .
  lcd.print("INITIALIZING....");
  delay(1000);
  lcd.clear();//Clean the screen
}

String data="";

String removeSpaces(String input) {
  String output = "";
  for (int i = 0; i < input.length(); i++) {
    if (input.charAt(i) != ' ') {
      output += input.charAt(i);
    }
  }
  return output;
}

void loop() {
  // pinMode(RELAY_PIN2, OUTPUT);
  if (Serial.available()) {
    String receivedData = Serial.readString();  // Read data as string
    Serial.println("Received Data: " + receivedData);
    data = receivedData;
  }

  String noSpaces = removeSpaces(data);
  Serial.print("Scan:");  // Output: HelloWorld!
  Serial.println(noSpaces);  // Output: HelloWorld!

  if(data == ""){
    lcd.setCursor(0,0); //Defining positon to write from first row,first column .
    lcd.print("FREE CHARGE"); //You can write 16 Characters per line .
    delay(1000);//Delay used to give a dynamic effect
    lcd.setCursor(0,1);  //Defining positon to write from second row,first column .
    lcd.print("Scan Your ID");
    digitalWrite(RELAY_PIN2, HIGH);
    delay(1000);
    lcd.clear();//Clean the screen
  } else {

    if (onoff == false) {

      String str = noSpaces; // Test string
  
      // Check if "Yes" is in the string
      if (str.indexOf("Yes") != -1) {
      lcd.clear();//Clean the screen
      lcd.setCursor(0,0); //Defining positon to write from first row,first column .
      lcd.print("STUDENT CHARGING"); //You can write 16 Characters per line .
      lcd.setCursor(0,1); //Defining positon to write from second row,first column .
      lcd.print("VERIFIED RDID!");
      digitalWrite(RELAY_PIN2, LOW);
      onoff=true;
      Serial.println(data);
      delay(1000);//Delay used to give a dynamic effect
      lcd.clear();//Clean the screen
    } else {
      lcd.setCursor(0,0); //Defining positon to write from first row,first column .
      lcd.print("FREE CHARGE"); //You can write 16 Characters per line .
      lcd.setCursor(0,1); //Defining positon to write from second row,first column .
      lcd.print(data);
      delay(1000); //Delay used to give a dynamic effect
      lcd.clear();//Clean the screen
      digitalWrite(RELAY_PIN2, HIGH);  
      delay(2000); //Delay used to give a dynamic effect
      data = "";
      onoff=false;
      Serial.print("INVALID RFID!");
    }

    if(noSpaces!="Yes" ){

    } else {

    }
  }
}

  if (onoff == true) {
    unsigned long currentMillis = millis(); // Get the current time
    if (currentMillis - previousMillis >= interval) {
      // Save the last time the display was updated
      previousMillis = currentMillis;

      if (remainingTime > 0) {
        remainingTime--;
      }

      // Calculate minutes and seconds
      int minutes = remainingTime / 60;
      int seconds = remainingTime % 60;

      // Ensure first row is consistent
      lcd.setCursor(0, 0);
      // lcd.print("FREE CHARGE  "); // Ensure spaces to remove leftover characters
      lcd.print("STUDENT CHARGING...");
      // Update only the timer portion of the LCD
      lcd.setCursor(0, 1);
      lcd.print("TIME LEFT: ");
      if (minutes < 10) lcd.print("0"); // Add leading zero if needed
      lcd.print(minutes);
      lcd.print(":");
      if (seconds < 10) lcd.print("0"); // Add leading zero if needed
      lcd.print(seconds);
      lcd.print("  "); // Extra spaces to clear old characters
      Serial.println("timer");
      if (remainingTime == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("FREE CHARGE");
        lcd.setCursor(0, 1);
        lcd.print("Scan Your ID");
        delay(1000); // Give a slight delay for effect
        onoff = false;
        data = "";
        digitalWrite(RELAY_PIN2, HIGH);
      }
    }
  }
  
}

