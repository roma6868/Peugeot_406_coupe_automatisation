/*
 Name:		Programe_Principale_Arduino_14_06_2020.ino
 Created:	14/06/2020 17:56
 Author:	Roman
*/

#include <Arduino.h>
//#include <Wire.h>
#include <Adafruit_MCP23017.h>
//#include "config.h"

#define OPTION_1 1 //Camera de recul
#define OPTION_2 0 //Phare automatique
#define OPTION_3 0 //Eclerage d'ambiance d'interieur
#define OPTION_4 1 //Communication avec ESP32 
#define OPTION_5 1 //Communication avec module Bleutooth
#define OPTION_6 1 //Communication avec le MCP23017 module controle relais
#define OPTION_7 1 //Ecran LCD aide a la conduite
#define OPTION_8 1 //genstion entre sortie


#if OPTION_5
	#include "peugeot_HC_05.h"
#endif // OPTION_5

#if OPTION_6
	#include "peugeot_MCP23017.h"
#endif // OPTION_6

#if OPTION_2
	#include "peugeot_Automatic_Headlight.h"
#endif // OPTION_2

#if OPTION_8
	#include "peugeot_InOutArduinoBasic.h"
#endif // OPTION_8

#if OPTION_4
	#include "peugeot_Esp32_arduino_Serial_communication.h"
#endif // OPTION_4

#if OPTION_7
	#include "peugeot_screen_LCD_i2c_16x2.h"
#endif // OPTION_7

//#include "peugeot_delay.h" // Gestion millis timer

/*Peter Pinter
Serial.begin(115200)
mySerial = new HardwareSerial();
mySerial->begin(115200);
*/


#if OPTION_5
	HC_05* hc05; // pointeur vers une instance de classe (si on utilise un pointeur void devez utilise -> a la place)
#endif // OPTION_5

#if OPTION_6
	MCP_23017* mcp_23017;
#endif // OPTION_6

#if OPTION_2
	Automatic_Headlight* automatic_headlight;
#endif // OPTION_2

#if OPTION_8
	InOutArduinoPin* in_out_arduino_pin;
#endif // OPTION_8

#if OPTION_4
	Esp32_arduino_serial_communication* esp32_arduino_serial_communication;
#endif // OPTION_4

#if OPTION_7
	Screen_LCD_i2c_16x2* screen_lcd_i2c_16x2;
#endif // OPTION_7


void setup() {

	//Serial.begin(500000); // vitesse de communication avec le pc
	Serial.begin(250000); // vitesse de communication avec le pc ARDUINO DUE

	Serial.println(F("Programe v1.2 : Programe_Principale_Arduino_14_06_2020.ino"));

	//https://openclassrooms.com/fr/courses/1894236-programmez-avec-le-langage-c/1898260-associez-les-classes-et-les-pointeurs
	//initialize the libraries

#if OPTION_5
	hc05 = new HC_05(); // allocation dynamique 
#endif // OPTION_5

#if OPTION_6
	mcp_23017 = new MCP_23017(); // ca fonctionne comme un setup
#endif // OPTION_6

#if OPTION_2
	automatic_headlight = new Automatic_Headlight(); // ca fonctionne comme un setup
#endif // OPTION_2

#if OPTION_8
	in_out_arduino_pin = new InOutArduinoPin(); // ca fonctionne comme un setup
#endif // OPTION_8

#if OPTION_4
	esp32_arduino_serial_communication = new Esp32_arduino_serial_communication(); // ca fonctionne comme un setup
#endif // OPTION_4

#if OPTION_7
	screen_lcd_i2c_16x2 = new Screen_LCD_i2c_16x2;
#endif // OPTION_7
}

// the loop function runs over and over again until power down or reset
void loop() {


	TIME1.DelayMillis(1000);
	/*
	if (TIME1.DelayMillis(1000) == 1)
	{
		Serial.println("1");
	}
	*/

#if OPTION_6
	mcp_23017->recoitDonnePCpourMCP23017(); // Mode debug
	mcp_23017->activationDesSortie(); // Envoye des donnes sequescielement au mcp via I2C

#endif // OPTION_6

#if OPTION_8
	in_out_arduino_pin->inputBasicPin();
#endif // OPTION_8

#if OPTION_1
	in_out_arduino_pin->cameraMarcheArriere();
#endif // OPTION1

#if OPTION_2
	automatic_headlight->phareAutolireLux(); // Lecture analogique de la lumunosite pour le sphare automatique
	in_out_arduino_pin->phareAutomatique(); //gere allumage des phares lorsque moteur en marche ou porte ouvert
#endif //OPTION_2

#if OPTION_3
	in_out_arduino_pin->outputPWMledSiege();
#endif // OPTION_3

#if OPTION_4 // gere la communication avec le ESP32
	esp32_arduino_serial_communication->recoitDonne();
#endif // OPTION_4
	
#if OPTION_7
	//screen_lcd_i2c_16x2 -> afficheDonneLCD();
	screen_lcd_i2c_16x2->envoyerDonneAuLCD();
#endif

	//delay(1000);
	/*
	if (luxEtatLumunosite == 1) {
		digitalWrite(13, HIGH);
		Serial.println("LED state : ON");
	}
	if (luxEtatLumunosite == 0) {
		digitalWrite(13, LOW);
		Serial.println("LED state : OFF");
	}
	if (luxEtatLumunosite == 2) { // si valeur recus et un 2 cest quil a y un probleme dans le capteur ou dans les reglage de lumunosit
		Serial.println("CAPTEUR DE LUMUNOSITE HS");
		digitalWrite(13, LOW); // eteindre absolument les LED
	}

	delay(1000);
	*/
}

#if OPTION_5

void serialEvent1() { // Pour le module Bleutooth
	hc05->serialEVENT();
	//Serial.println("Serial even");
}
#endif // OPTION_5


