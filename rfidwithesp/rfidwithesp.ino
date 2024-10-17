
// #include <SPI.h>
// #include <MFRC522.h>

// #define SS_PIN 21
// #define RST_PIN 0
// MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// void setup()
// {
//   Serial.begin(9600);   // Initiate a serial communication
//   SPI.begin();      // Initiate  SPI bus
//   mfrc522.PCD_Init();   // Initiate MFRC522
//   Serial.println("Approximate your card to the reader...");
//   Serial.println();

// }
// void loop()
// {
//   // Look for new cards
//   if ( ! mfrc522.PICC_IsNewCardPresent())
//   {
//     return;
//   }
//   // Select one of the cards
//   if ( ! mfrc522.PICC_ReadCardSerial())
//   {
//     return;
//   }
//   //Show UID on serial monitor
//   Serial.print("UID tag :");
//   String content= "";
//   byte letter;
//   for (byte i = 0; i < mfrc522.uid.size; i++)
//   {
//      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//      Serial.print(mfrc522.uid.uidByte[i], HEX);
//      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
//      content.concat(String(mfrc522.uid.uidByte[i], HEX));
//   }
//   Serial.println();
//   Serial.print("Message : ");
//   content.toUpperCase();
//   if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
//   {
//     Serial.println("Authorized access");
//     Serial.println();
//     delay(3000);
//   }

//  else   {
//     Serial.println(" Access denied");
//     delay(3000);
//   }
// }



#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 0     // Reset pin for RC522 module
#define SDA_PIN 21      // Slave Select pin for RC522 module
MFRC522 rfid(SDA_PIN, RST_PIN);   // Create MFRC522 instance
 
void setup() {
  Serial.begin(9600);   // Initialize serial communication
  SPI.begin();          // Initialize SPI communication
  rfid.PCD_Init();      // Initialize RC522 module
}
 
void loop() {
  
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // If a new card is present and successfully read
    
    Serial.print("Card UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      Serial.print(rfid.uid.uidByte[i], DEC);   // Print the card UID in decimal format
      
    }
    Serial.println();
    Serial.println("by Programming Digest");
    Serial.println();
  }
  delay(1000);   // Wait for 1 second before trying to read the card again
}






