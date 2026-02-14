// © Kay Sievers <kay@versioduo.com>, 2022
// © Andreas Thalmann, 2026
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <stdint.h>

class Font5x7 {
public:
  static const uint8_t *getBitmap(uint8_t character);
};
