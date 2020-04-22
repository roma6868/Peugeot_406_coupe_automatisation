/*
 Name:		teste_communication_bleutooth_2.ino
 Created:	27/02/2020 16:03:39
 Author:	r.shabat
*/

#include "Arduino.h"
#include"Header_bleutooth_communication_2.h"

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200); // communication avec le PC
	setup_bleutooth_communication();
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
