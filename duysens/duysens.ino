//Arduino IDE kullanılarak yazılmıştır.
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <avr/interrupt.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int mikrofonPin = A0;
const int ledPin = 8; // LED pin

float filtreliSes = 0;
float filtreKatsayisi = 0.95;
int gurultuEsik = 50; // EEPROM'dan okunacak

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(ledPin, OUTPUT);

  // EEPROM'dan eşik oku
  gurultuEsik = EEPROM.read(0);
  if (gurultuEsik < 10 || gurultuEsik > 100) gurultuEsik = 50;
}

void loop() {
  int sesDegeri = analogRead(mikrofonPin);
  filtreliSes = (filtreliSes * filtreKatsayisi) + (sesDegeri * (1 - filtreKatsayisi));
  int yuzdeSes = map(filtreliSes, 0, 1023, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print("Ses: ");
  lcd.print(yuzdeSes);
  lcd.print(" %   ");

  // Sadece 70 ve üstünde LED yanar
  if (yuzdeSes >= 70) {
    digitalWrite(ledPin, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Tehlikeli!      ");
  } else {
    digitalWrite(ledPin, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Normal          ");
  }

  delay(100);
}
