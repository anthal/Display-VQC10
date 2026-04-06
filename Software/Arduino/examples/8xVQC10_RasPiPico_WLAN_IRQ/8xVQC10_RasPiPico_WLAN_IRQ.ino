#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>        // Michael Margolis
#include <RP2040_RTC.h>     // Khoi Hoang (optional, für persistente RTC)
#include "RPi_Pico_TimerInterrupt.h"  // Khoi Hoang
#include "VQC10.h"
#include "secrets.h"

// NTP
WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);  // CET
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);  // UTC

datetime_t dt;  // RTC-Datentyp

static VQC10<> LED({
  {20, 19, 18, 17, 16, 10}, // Digits über Dekoder
  {28, 27, 26, 22, 21}, 	  // Spalten
  {15, 14, 12, 8},      	  // Zeilen über Dekoder: Pin 8 (De-)Aktivierung des Zeilendecoders
});

static unsigned long usec{};
static uint16_t count{};
char buffer[64];
String text;

// Init RPI_PICO_Timer, can use any from 0-15 pseudo-hardware timers
RPI_PICO_Timer ITimer(0);  // Timer 0-2 verfügbar

#define TIMER_INTERVAL_US 2000  // 200 MicroSekunden = 5 kHz (mit 500 Hz auch OK)
volatile bool timerFlag = false;


bool TimerHandler(struct repeating_timer *t) {
  timerFlag = true;
  return true;  // Wichtig für repeating_timer!
}


// Anzeige von Text fuer eine bestimmte Zeit (time) auf dem VQC10:
void show_vqc(String text, int time) {
  unsigned long startzeit = micros();  // Startzeit merken
  while ((unsigned long)(micros() - startzeit) < time * 1000000UL) {
    // fuer alle 16 Stellen:
    for (uint8_t i = 0; i < 32; i++) {
      LED.show(i, text[count + i]);
    }
    if (timerFlag) {
      LED.tick();
      // Timer Interrupt!
      timerFlag = false;
    }
  }
}


bool isDSTGermany(const tm& utc) {
  int year = utc.tm_year + 1900;
  int month = utc.tm_mon + 1;
  int day = utc.tm_mday;
  int hour = utc.tm_hour;

  if (month < 3 || month > 10) return false;
  if (month > 3 && month < 10) return true;

  int lastSunday;

  if (month == 3) {
    lastSunday = 31 - ((5 * year / 4 + 4) % 7);
    if (day > lastSunday) return true;
    if (day < lastSunday) return false;
    return hour >= 1;   // Umschaltung 01:00 UTC
  }

  lastSunday = 31 - ((5 * year / 4 + 1) % 7);
  if (day < lastSunday) return true;
  if (day > lastSunday) return false;
  return hour < 1;      // Rückschaltung 01:00 UTC
}


time_t germanyLocalTime(time_t utcEpoch) {
  tm utc;
  gmtime_r(&utcEpoch, &utc);

  long offset = isDSTGermany(utc) ? 7200 : 3600;
  return utcEpoch + offset;
}


void setup() {
  // Init:
  LED.begin();

  Serial.begin(115200);
  //while (!Serial);
  delay(500);

  Serial.print(F("\nStarting TimerInterruptTest on ")); Serial.println(BOARD_NAME);
  Serial.println(RPI_PICO_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  Serial.flush();

  //show_vqc("Warte auf WLAN..", 3);

  // WLAN & NTP
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.println("Warte auf WLAN Verbindung");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWLAN Verbindung OK");

  //show_vqc("WLAN Verbind. OK", 3);
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
  Serial.printf("Start VQC10\n");

  IPAddress ip = WiFi.localIP();
  WiFi.disconnect();

  text = String(ip[0]) + "." +
         String(ip[1]) + "." +
         String(ip[2]) + "." +
         String(ip[3]);

  // Aktivierung des IRQ Timers:
  // Interval in microsecunden
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_US, TimerHandler))
  {
    Serial.print(F("Start IRQ-Timer 0 OK, Timerintervall [Microsec.]: ")); Serial.println(TIMER_INTERVAL_US);
  }
  else
    Serial.println(F("Can't set IRQ-Timer 0. Select another freq. or timer"));

  // 5 Sekunden lang die IP-Adresse anzeigen:
  Serial.printf("IP: "); Serial.println(text);
  show_vqc("IP-Adresse:     " + text, 5);
}

void loop() {
  if (timerFlag) {
    LED.tick();
    // Timer Interrupt!
    timerFlag = false;
  }
  // RTC lesen
  datetime_t now;
  rtc_get_datetime(&now);
  const char* monthNames[] = {"", "Januar", "Februar", "Maerz", "April", "Mai", "Juni", 
                                  "Juli", "August", "September", "Oktober", "November", "Dezember"};
  sprintf(buffer, "Es ist %02d:%02d:%02d %02d. %s %04d", now.hour, now.min, now.sec, now.day, monthNames[now.month], now.year);
  //sprintf(buffer, "                %02d:%02d %02d.%02d.%04d", now.hour, now.min, now.day, now.month, now.year);
  //               12345678901234567890123456789012
  text = buffer;
  Serial.println(text);
  show_vqc(text, 1);
}
