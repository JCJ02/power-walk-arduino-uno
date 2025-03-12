#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // Slave Select pin
#define RST_PIN 9  // Reset pin

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
    Serial.begin(9600);  // Start Serial Monitor
    SPI.begin();         // Initialize SPI bus
    mfrc522.PCD_Init();  // Initialize RFID reader
    Serial.println("Scan your RFID tag...");
}

void loop() {
    // Check if an RFID card is present
    if (!mfrc522.PICC_IsNewCardPresent()) return;
    if (!mfrc522.PICC_ReadCardSerial()) return;

    // Print the UID
    Serial.print("UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");  // Add leading zero
      Serial.print(mfrc522.uid.uidByte[i], DEC);
      Serial.print(" ");
    }
    Serial.println();

    mfrc522.PICC_HaltA();      // Stop communication with the card
    mfrc522.PCD_StopCrypto1(); // End encryption
}