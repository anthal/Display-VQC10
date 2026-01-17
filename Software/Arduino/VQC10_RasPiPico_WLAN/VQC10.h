// © Kay Sievers <kay@versioduo.com>, 2022
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <Arduino.h>
#include "Font5x7.h"

// As single device has four digits, every digit needs a separate latch pin.
// Mutliple devices can share the same column and row pins.
template <uint8_t n_digits = 16> class VQC10 {
public:
  struct Pins {
    uint8_t digits[6];  // Stellen ueber Dekoder 
    uint8_t columns[5];  // Spalten
    uint8_t rows[4];  // Zeilen ueber Dekoder 
  };

  uint8_t bit0;  // (LSB)
  uint8_t bit1;  
  uint8_t bit2; 
  uint8_t bit3; 

  constexpr VQC10(Pins pins) : _pins{pins} {}

  // Init der Pins:
  void begin() {
    for (uint8_t i = 0; i < 6; i++) {
      pinMode(_pins.digits[i], OUTPUT);
      digitalWrite(_pins.digits[i], LOW);
    }

    for (uint8_t i = 0; i < 5; i++) {
      pinMode(_pins.columns[i], OUTPUT);
      digitalWrite(_pins.columns[i], LOW);
    }

    for (uint8_t i = 0; i < 4; i++) {
      pinMode(_pins.rows[i], OUTPUT);
      digitalWrite(_pins.rows[i], LOW);
    }
  }

  // Called if tick() is not run periodically from timer.
  void loop() {
    if ((unsigned long)(micros() - _usec) < 200)
      return;
    _usec = micros();

    tick();
  }

  // Draw the next row of LED dots. It is called from loop() or directly with
  // with a ~5kHz timer/software interrupt.
  void tick() {
    // Disable the power of the current row.
    // Deaktivierung Zeilendekoder:
    digitalWrite(_pins.rows[3], LOW);
    digitalWrite(_pins.digits[4], HIGH);
    // Zeilenzaehler: 
    _row++;
    if (_row == 7)
      _row = 0;

    // Aktualisierung der nächste Datenzeile für alle Stellen:
    for (uint8_t digit = 0; digit < n_digits; digit++) {
      const uint8_t *bitmap = Font5x7::getBitmap(_character[digit]);

      // 5 Spalten:
      // Data input, enable/disable the five dots / bits per digit.
      for (uint8_t column = 0; column < 5; column++) {
        const bool bit = bitmap[column] & (1 << _row);
        digitalWrite(_pins.columns[column], bit);
        //Serial.printf("Spalte: %1d: Bit: %1d\n", column, bit);
      }

      // 16 Stellen:
      // Latch / commit the data to the digit.
      //digitalWrite(_pins.digits[5], LOW);
      bit0 = bitRead(digit, 0);  // (LSB)
      bit1 = bitRead(digit, 1);  // 
      bit2 = bitRead(digit, 2);  // 
      bit3 = bitRead(digit, 3);  //  
      //Serial.printf("Digit: %1d: Bits: %1d %1d %1d %1d\n", digit, bit3, bit2, bit1, bit0);
      digitalWrite(_pins.digits[0], bit0);
      digitalWrite(_pins.digits[1], bit1);
      digitalWrite(_pins.digits[2], bit2);
      digitalWrite(_pins.digits[3], bit3);
      // Speichern und Durchschalten der Daten pro Stelle mit der L/H Flanke:
      digitalWrite(_pins.digits[4], LOW);
      digitalWrite(_pins.digits[4], HIGH);
    }

    // 7 Zeilen:
    // Enable the power for the updated row.
    // Deaktivierung Zeilendekoder:
    //digitalWrite(_pins.rows[3], LOW);
    bit0 = bitRead(_row, 0);  // (LSB)
    bit1 = bitRead(_row, 1);  // 
    bit2 = bitRead(_row, 2);  // 
    digitalWrite(_pins.rows[0], bit0);
    digitalWrite(_pins.rows[1], bit1);
    digitalWrite(_pins.rows[2], bit2);
    delay(1);
    // Aktivierung Zeilendekoder:
    //digitalWrite(_pins.rows[3], HIGH);
    // Deaktivierung der Zeilen:
    digitalWrite(_pins.rows[0], HIGH);
    digitalWrite(_pins.rows[1], HIGH);
    digitalWrite(_pins.rows[2], HIGH);    
  }

  // Show an ASCII character.
  void show(uint8_t position, uint8_t character) {
    //Serial.printf("Position: %1d - %c\n", position, character);
    if (position >= n_digits)
      return;

    _character[position] = character;
  }

private:
  const Pins _pins;
  uint8_t _character[n_digits]{};
  unsigned long _usec{};
  uint8_t _row{};
};
