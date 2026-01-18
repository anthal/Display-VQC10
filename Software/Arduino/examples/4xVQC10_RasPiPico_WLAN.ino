#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>        // Michael Margolis
#include <RP2040_RTC.h>     // Khoi Hoang (optional, für persistente RTC)
#include "VQC10.h"

// WLAN
const char* ssid = "ssid";
const char* password = "password";

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);  // CET

datetime_t dt;  // RTC-Datentyp

static VQC10<> LED({
  {20, 19, 18, 17, 16, 10}, // Digits über Dekoder - d:\Projekte\Displays\VQC 10\Arduino\VQC10-main\src\Font5x7.cpp
  {28, 27, 26, 22, 21}, 	// Spalten
  {15, 14, 12, 8},      	// Zeilen über Dekoder: Pin 8 (De-)Aktivierung des Zeilendecoders 
});

//String text =   "VQC10 Terminal  ";
String text =   "";
bool stringComplete = false;     // Flag für vollständige Zeile

void setup() {
  // Init:
  LED.begin();

  Serial.begin(115200);
  delay(1000);

  // WLAN & NTP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWLAN OK");

    timeClient.begin();
  timeClient.update();
  unsigned long epoch = timeClient.getEpochTime();

  // Epoch zu datetime_t konvertieren
  time_t rawtime = epoch;
  struct tm *ti = gmtime(&rawtime);
  dt.year  = ti->tm_year + 1900;
  dt.month = ti->tm_mon + 1;
  dt.day   = ti->tm_mday;
  dt.dotw  = ti->tm_wday;
  dt.hour  = ti->tm_hour;
  dt.min   = ti->tm_min;
  dt.sec   = ti->tm_sec;

  // RTC initialisieren & setzen
  rtc_init();
  rtc_set_datetime(&dt);
  char datumStr[64];
  strftime(datumStr, sizeof(datumStr), "%Y.%m.%d %H:%M:%S", ti);
  Serial.println("RTC gesetzt: " + String(datumStr));

  WiFi.disconnect();

  Serial.printf("Start VQC10\n");
}

void loop() {
  static unsigned long usec{};
  static uint16_t count{};
  char buffer[64];

  // RTC lesen
  datetime_t now;
  rtc_get_datetime(&now);
  sprintf(buffer, "%02d:%02d:%02d  %02d.%02d.", now.hour, now.min, now.sec, now.day, now.month);
  text = buffer;

  //Serial.printf(text);
  count = 0;
  // laengere Scrollzeit:   
  if ((unsigned long)(micros() - usec) > 150UL * 2000) {      
    usec = micros();

    // für alle 16 Stellen:
    //Serial.printf("count: %1d - %c %c %c %c\n", count, text[count + 0], text[count + 1], text[count + 2], text[count + 3]);
    for (uint8_t i = 0; i < 16; i++) {
      LED.show(i, text[count + i]);
    }
      
    count++;
    if (count + 16 == sizeof(text))
      count = 0;
  }
  //delay(1000);
  LED.loop();  
  
}
