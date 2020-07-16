#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "peugeot_config.h"

#define DEBUG_affiche_donne_envoyer 0 

LiquidCrystal_I2C lcd(0x27, 20, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display


class Screen_LCD_i2c_16x2
{
public:
	Screen_LCD_i2c_16x2();
	~Screen_LCD_i2c_16x2();
	int afficheDonneLCD();
	void envoyerDonneAuLCD();

private:

};

Screen_LCD_i2c_16x2::Screen_LCD_i2c_16x2()
{
	lcd.init(); // initaliser arduino LCD
	lcd.backlight(); // Allumer LCD
	//lcd.noBacklight(); // Eteindre LCD
	Serial.println(F("=> LCD i2c 16x2 <="));
	
}

Screen_LCD_i2c_16x2::~Screen_LCD_i2c_16x2()
{
}

void Screen_LCD_i2c_16x2::envoyerDonneAuLCD() {
	
	if (TIME1.DelayMillis(100) == 1) // envoyer les donne au LCD tout les x seconde
	{
#if DEBUG_affiche_donne_envoyer
		Serial.println("Envoye des donne au LCD");
#endif // DEBUG_affiche_donne_envoyer

		afficheDonneLCD();
	}
	
	//TIME1.DelayMillis(1000);
}

int Screen_LCD_i2c_16x2::afficheDonneLCD()
{

	if (etatVoiture == 0 && etatDuNeiman == 0) { // voiture ouvert et pas de contact

		lcd.setCursor(0, 0);
		lcd.print("    BONJOUR !!  ");

		lcd.setCursor(0, 1);
		lcd.print("406 coupe V1.02  ");
		/*
			switch (etatDesPorte) {
			  case 0: // 0 = porte ferme conducteur/passager
				lcd.setCursor(0, 1);
				lcd.print("Tout porte ferme");
				break;
			  case 1: // 1 = porte ouvert cote conducteur
				lcd.setCursor(0, 1);
				lcd.print("Port cond ouvert");
				break;
			  case 2:// 2 = porte ouvert cote passager
				lcd.setCursor(0, 1);
				lcd.print("Port pass ouvert");
				break;
			  case 3:// 3 = porte ouver conducteur et passager
				lcd.setCursor(0, 1);
				lcd.print("Tout porte ouvert");
				break;

			}
		*/
	}


	if (etatVoiture == 1 && etatDuNeiman == 0) { // fermer
		lcd.setCursor(4, 0);
		lcd.print("AUREVOIR !!      ");
		lcd.setCursor(0, 1);
		lcd.print("                 ");
	}

	if (etatDuNeiman == 1) { // position accesoir
		lcd.setCursor(0, 0);
		lcd.print("Temp ext: ");
		lcd.setCursor(10, 0);
		lcd.print(tempExterieurRecalcule, DEC);
		lcd.setCursor(12, 0);
		lcd.print("    ");

		lcd.setCursor(0, 1);
		lcd.print("Etat Port: ");

		switch (etatDesPorte) {
		case 0: // 0 = porte ferme conducteur/passager
			lcd.setCursor(11, 1);
			lcd.print("F | F");
			break;
		case 1: // 1 = porte ouvert cote conducteur
			lcd.setCursor(11, 1);
			lcd.print("O | F");
			break;
		case 2:// 2 = porte ouvert cote passager
			lcd.setCursor(11, 1);
			lcd.print("F | O");
			break;
		case 3:// 3 = porte ouver conducteur et passager
			lcd.setCursor(11, 1);
			lcd.print("O | O");
			break;

		}
	}
	if (etatDuNeiman == 7) { // position accesoir
		lcd.setCursor(0, 0);
		lcd.print("RPM:           ");
		lcd.setCursor(5, 0);
		lcd.print(rpmDecoded, DEC);

		lcd.setCursor(0, 1);
		lcd.print("SPEED:          ");
		lcd.setCursor(7, 1);
		lcd.print(speedDecoded, DEC);


		switch (etatClignotant) { // etat des clignotant
		case 2:
			lcd.setCursor(15, 0);
			lcd.print("<");

			break;

		case 1:
			lcd.setCursor(15, 0);
			lcd.print(">");
			break;

		case 3:
			lcd.setCursor(15, 0);
			lcd.print("=");
			break;

		case 0:
			lcd.setCursor(15, 0);
			lcd.print(" ");
			break;
		}//switch



	}

	if (etatDuNeiman == 3) { //contact moteur

		lcd.setCursor(0, 1);
		lcd.print("Contact Moteur   ");
		lcd.setCursor(0, 0);
		lcd.print("Temp moteur: ");
		lcd.setCursor(13, 0);
		lcd.print(tempsEauRecalculer, DEC);
		lcd.setCursor(15, 0);
		lcd.print("  ");
	}

}