#include "VQC10.h"

static VQC10<> LED({
  {20, 19, 18, 17, 16, 10}, // Digits über Dekoder - d:\Projekte\Displays\VQC 10\Arduino\VQC10-main\src\Font5x7.cpp
  {28, 27, 26, 22, 21}, 	  // Spalten
  {15, 14, 12, 8},      	  // Zeilen über Dekoder: Pin 8 (De-)Aktivierung des Zeilendecoders 
});

static const char text[] = "1234567890123456";

void setup() {
  Serial.begin(115200);
  // Init:
  LED.begin();
  delay(1000);
  Serial.printf("Start VQC10\n");
}

void loop() {
  static unsigned long usec{};
  static uint16_t count{};

  // für alle 16 Stellen:
  //Serial.printf("count: %1d - %c %c %c %c\n", count, text[count + 0], text[count + 1], text[count + 2], text[count + 3]);
  for (uint8_t i = 0; i < 16; i++) {
    LED.show(i, text[count + i]);
  }
    
  LED.loop();
}
