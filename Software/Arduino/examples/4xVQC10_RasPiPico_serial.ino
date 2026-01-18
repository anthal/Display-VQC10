#include "VQC10.h"

static VQC10<> LED({
  {20, 19, 18, 17, 16, 10}, // Digits über Dekoder - d:\Projekte\Displays\VQC 10\Arduino\VQC10-main\src\Font5x7.cpp
  {28, 27, 26, 22, 21}, 	  // Spalten
  {15, 14, 12, 8},      	  // Zeilen über Dekoder: Pin 8 (De-)Aktivierung des Zeilendecoders 
});

String text =   "";
bool stringComplete = false;     // Flag für vollständige Zeile

void setup() {
  Serial.begin(115200);
  // Init:
  LED.begin();
  Serial.printf("Start VQC10\n");
}

void loop() {
  static unsigned long usec{};
  static uint16_t count{};

  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();  // Nächstes Zeichen lesen
    text += inChar;
    if (inChar == '\n') {               // Zeilenende erkannt
      stringComplete = true;
    }  
  }  

  //stringComplete = true;
  if (stringComplete) {
    Serial.print("Empfangene Zeile: ");
    Serial.println(text);
    count = 0;
    // laengere Scrollzeit:   
    if ((unsigned long)(micros() - usec) > 150UL * 2000) {      
      usec = micros();

      // für alle 16 Stellen:
      //Serial.printf("count: %1d - %c %c %c %c\n", count, text[count + 0], text[count + 1], text[count + 2], text[count + 3]);
      for (uint8_t i = 0; i < 16; i++) {
        LED.show(i, text[count + i]);
      }
      text = "";
      stringComplete = false;
      
      count++;
      if (count + 16 == sizeof(text))
        count = 0;
    }
  }
  LED.loop();
  
}
