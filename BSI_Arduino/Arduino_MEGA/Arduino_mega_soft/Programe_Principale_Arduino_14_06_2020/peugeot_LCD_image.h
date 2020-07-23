#pragma once

#include <Arduino.h>

//http://alx.github.io/deuligne-custom-char/#

byte BT_image[8] = {
  B00110,
  B00101,
  B10110,
  B01100,
  B01100,
  B10110,
  B00101,
  B00110,
};

byte FEUX_CROISEMENT_image[8] = {
  B00000,
  B11000,
  B10110,
  B10101,
  B10110,
  B10101,
  B11010,
  B00001,
};

byte FEUX_route_image[8] = {
  B00000,
  B11000,
  B10111,
  B10100,
  B10111,
  B10100,
  B11011,
  B00000,
};
