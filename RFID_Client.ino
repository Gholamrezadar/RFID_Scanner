#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <String.h>

void door_open(String door);

// Data
#define USER_COUNT 3

#define CARD_1_UID "B7 3B BD C1"
#define CARD_2_UID "04 A2 8A D9"
#define CARD_3_UID "6D 64 93 39"

#define USER_1 "Gholamreza Dar"
#define USER_2 "Abolfazl Omrani"
#define USER_3 "Hamidreza Radfar"


#define SS_PIN 10
#define RST_PIN 9


// database
String users[] = {USER_1, USER_2, USER_3};
String cards[] = {CARD_1_UID, CARD_2_UID, CARD_3_UID};

// global variables
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
boolean RFID_mode = true;

void setup()
{
  // inits
  lcd.init();
  SPI.begin();
  lcd.backlight();
  mfrc522.PCD_Init();
  Serial.begin(9600);

  // print "Scan your Tag" to lcd
  lcd.setCursor(2, 0);
  lcd.print("Scan your Tag");
}

void loop()
{
  // check if there is a new card
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // read cards UID
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  // check available cards
  for (int i = 0; i < USER_COUNT; i++)
  {
    // check if its Card Number i
    if (content.substring(1) == cards[i])
    {
      // print authorized username to serial
      Serial.println(users[i]);

      // Greeting on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hello" + users[i]);
      lcd.setCursor(0, 1);
      lcd.print("Authorized access");
      delay(3000);
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Scan yor Tag");
      return;
    }
  }
  // Other cards
  lcd.setCursor(0, 1);
  lcd.print(" Access denied");
  delay(3000);
  lcd.clear();
  lcd.print(" Scan yor Tag");
}
