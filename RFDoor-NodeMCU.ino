#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>

#define RST_PIN 100 // RST-PIN - 100 para n ter que usa-lo :)
#define SS_PIN  15  // SDA-PIN
MFRC522 mfrc522(SS_PIN, RST_PIN);

LiquidCrystal lcd(10, D4, D3, D2, D1, D0);

#define relePin 9

String usuarios[] = {
  "7ED9BDC1",
  "30182483"
};

void setup() {
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, 1);
  
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.begin(16, 2);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("Aproxime a tag");
  lcd.setCursor(0, 1);
  lcd.write("RFID");
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {   
    delay(100);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {   
    delay(10 0);
    return;
  }

////-------------------------------------------------RFID----------------------------------------------

  String tag = PegarTag(mfrc522);
  Serial.println("Cartao lido: " + tag + "\n");
  
  if(Autorizar(tag)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Acesso liberado!");

    Serial.println("Acesso liberado!");
    
    digitalWrite(relePin, 0);
    delay(500);
    digitalWrite(relePin, 1);
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Usuario nao");
    lcd.setCursor(0, 1);
    lcd.write("cadastrado!");
    
    Serial.println("Usuario nao cadastrado!");
  }

  delay(1000);
}

String PegarTag(MFRC522 mf) {
  String content = "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mf.uid.uidByte[i] < 0x10 ? " 0" : ""));
     content.concat(String(mf.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  content.toUpperCase();

  return content;
}

bool Autorizar(String tag) {
  for(int i = 0; i < sizeof(usuarios); i++) {
    if(usuarios[i] == tag)
      return true;
  }

  return false;
}
