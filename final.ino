// Justin Choi
// Final Project
// Purpose: Sense humidity/temp and display on after authorized RFID is near sensor

#include <SPI.h>
#include "MFRC522.h"
#include <LiquidCrystal.h>
#include "dht.h"

#define RST_PIN   6   
#define SS_PIN    53
#define ENABLE 5
#define DIRA 3
#define DIRB 4

MFRC522 mfrc522(SS_PIN, RST_PIN);
byte key[] = {0x23, 0x6C, 0x06, 0x19};
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
dht d;
int toggle_state[] = {0, 1, 2, 3};
int toggle = 0;
byte currentState;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init(); 
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  pinMode(13, INPUT);
  lcd.begin(16,2);
  lcd.print("LOCKED");
  d = dht();
  d.measure_environment();
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  while(!d.measure_environment());
  lcd.clear();
  while (!pcd_authenticate(mfrc522));
  do {
    if (toggle == 0) {
      lcd.print("Temp = ");
      lcd.print(d.temperature);
      lcd.print(" d C");
    } else if (toggle == 1) {
      lcd.print("humidity = ");
      lcd.print(d.humidity);
      lcd.print(" %");
    } else if (toggle == 2) {
      lcd.print("fan sim");
      d.fan_sim();
    } 
    delay(1000);
    if ( currentState = digitalRead(13) ) {
      delay(50);
      while(digitalRead(13)); // wait for low
      toggle++;
    }
    lcd.clear();
  } while (toggle < 3);
  delay(1000);
  lcd.clear();
  toggle = 0;
  lcd.print("LOCKED");
}

bool pcd_authenticate(MFRC522 k) {
  for (int i = 0; i < 4; i++) {
    if (!(k.uid.uidByte[i] == key [i])) {
      return false;
    }
  }
  return true;
}
