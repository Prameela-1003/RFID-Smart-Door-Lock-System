#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D4  // Reset pin for RFID
#define SS_PIN D8   // Slave Select pin for RFID
#define RELAY_PIN D3 // Relay pin for door unlocking

MFRC522 rfid(SS_PIN, RST_PIN);  // Create RFID object

// Define authorized RFID tag (Replace with actual UID)
String authorizedTag = "73A931FA"; // Example UID

void setup() {
    Serial.begin(115200);
    SPI.begin();          // Start SPI communication
    rfid.PCD_Init();      // Initialize RFID module
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Ensure relay is initially OFF
    Serial.println("Place RFID tag to unlock door...");
}

void loop() {
    // Check if an RFID tag is present
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    // Read the UID from the tag
    String tag = "";
    for (byte i = 0; i < 4; i++) {
        tag += String(rfid.uid.uidByte[i], HEX);
    }
    tag.toUpperCase(); // Convert to uppercase for consistency

    Serial.println("Detected Card UID: " + tag);

    // Check if the detected UID matches the authorized tag
    if (tag == authorizedTag) {
        Serial.println("Access Granted! Door Unlocked.");
        digitalWrite(RELAY_PIN, HIGH); // Activate relay
        delay(5000); // Keep the relay ON for 5 seconds (Adjust as needed)
        digitalWrite(RELAY_PIN, LOW); // Turn OFF relay
        Serial.println("Door Locked.");
    } else {
        Serial.println("Access Denied!");
    }

    delay(1000); // Small delay before reading next tag
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}
