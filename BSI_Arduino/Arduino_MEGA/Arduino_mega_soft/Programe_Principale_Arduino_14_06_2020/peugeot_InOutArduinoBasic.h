#pragma once

#include <Arduino.h>
#include "peugeot_config.h"

#ifndef InOutArduinoBasic_h
#define InOutArduinoBasic_h

#define inputPinOpt22 22 // Contact moteur
#define inputPinOpt24 24 // Marche arriere enclancher
#define inputPinOpt23 23 // Alimentation + accesoire
#define inputPinOpt25 25 // Moteur voiture en marche
#define inputPinOpt26 26 // +12 Bus van permet de savoir si le bus VAN et alimenter par le BSI
#define inputPinOpt27 27 // NO
#define inputPinOpt28 28 // NO
#define inputPinOptA0 A0 // NO

//ULN2003A (1)
#define outputPinUln41 41 //NO
#define outputPinUln40 40 //NO
#define outputPinUln38 38 //NO
#define outputPinUln37 4 // LED SOUS LES SIEGE CONDUCTEUR + PASSAGE 37 a etait modifier en pin 4 pour le PWM
#define outputPinUln36 36 //NO
#define outputPinUln35 35 // CAMMERA DE RECULE ARRIERE
//ULN2003A (2)
#define outputPinUln34 34 //NO
#define outputPinUln33 33 //NO
#define outputPinUln32 32 //NO
#define outputPinUln31 31 //NO
#define outputPinUln30 30 //NO
#define outputPinUln29 29 //NO

#define relais2Securite 2 // R2 relais de securite qui coupe l'alimnetation electrique 5V 
#define relais1Securite 3 // R1 relais de securite qui coupe l'alimnetation electrique 12V 

#define debugAfficherEtatEntre 0 //afficher dans le moniteur les etat des entre (defaut 0)

class InOutArduinoPin
{
public:
	InOutArduinoPin();
	~InOutArduinoPin();

	void inputBasicPin();
	void outputPWMledSiege();
	void cameraMarcheArriere();
	void phareAutomatique();

private:

	boolean contactMoteurIN; //inputPinOpt22
	boolean marcheArriereIN; // inputPinOpt24
	boolean AlimentationAccesoireIN; //inputPinOpt23
	boolean moteurEnmarcheIN; //inputPinOpt25
	boolean etatBusVanBsi; // inputPinOpt26
	byte ledSiegeEtat; // 0 = pas de PWM led etaint ; 1 = PWM mode feux de croisement ; 2 = PWM mode porte ouvert

};

InOutArduinoPin::InOutArduinoPin() // constructeur
{
	Serial.println(F("=> IN OUT ARDUINO PIN OK <="));
	//����������������������� ENTRE SORTIE BASIQUE ARDUINO MEGA ��������������������������������������������������������������������������������

	contactMoteurIN = 0; //inputPinOpt22
	marcheArriereIN = 0; // inputPinOpt24
	AlimentationAccesoireIN = 0; //inputPinOpt23
	moteurEnmarcheIN = 0; //inputPinOpt25
	etatBusVanBsi = 0; // inputPinOpt26
	ledSiegeEtat = 0; // 0 = pas de PWM led etaint ; 1 = PWM mode feux de croisement ; 2 = PWM mode porte ouvert

//OPTOCOUPLEUR
	pinMode(inputPinOpt22, INPUT);
	pinMode(inputPinOpt24, INPUT);
	pinMode(inputPinOpt23, INPUT);
	pinMode(inputPinOpt25, INPUT);
	pinMode(inputPinOpt26, INPUT);
	pinMode(inputPinOpt27, INPUT);
	pinMode(inputPinOpt28, INPUT);
	pinMode(inputPinOptA0, INPUT);

	pinMode(relais1Securite, OUTPUT); // R2 relais de securite qui coupe l'alimnetation electrique 12V
	pinMode(relais2Securite, OUTPUT); // R2 relais de securite qui coupe l'alimnetation electrique 5V

	//ULN2003A (1)
	pinMode(outputPinUln41, OUTPUT);
	pinMode(outputPinUln40, OUTPUT);
	pinMode(outputPinUln38, OUTPUT);
	pinMode(outputPinUln37, OUTPUT);
	pinMode(outputPinUln36, OUTPUT);
	pinMode(outputPinUln35, OUTPUT);

	//ULN2003A (2)
	pinMode(outputPinUln34, OUTPUT);
	pinMode(outputPinUln33, OUTPUT);
	pinMode(outputPinUln32, OUTPUT);
	pinMode(outputPinUln31, OUTPUT);
	pinMode(outputPinUln30, OUTPUT);
	pinMode(outputPinUln29, OUTPUT);
	//����������������������� ENTRE SORTIE BASIQUE ARDUINO MEGA ��������������������������������������������������������������������������������
}

InOutArduinoPin::~InOutArduinoPin() // destructeur
{
}


inline void InOutArduinoPin::inputBasicPin() {

	//INPUT
	contactMoteurIN = digitalRead(inputPinOpt22); //inputPinOpt22
	contactMoteurBasicPin = contactMoteurIN; // recupere valeur pour variable global

	marcheArriereIN = digitalRead(inputPinOpt24); // inputPinOpt24
	marcheArriereBasicPin = marcheArriereIN; // recupere valeur pour variable global

	AlimentationAccesoireIN = digitalRead(inputPinOpt23); //inputPinOpt23
	accesoireBasicPin = AlimentationAccesoireIN; // recupere valeur pour variable global

	moteurEnmarcheIN = digitalRead(inputPinOpt25); //inputPinOpt25
	moteurEnMarchePin = moteurEnmarcheIN; // recupere valeur pour variable global

	etatBusVanBsi = digitalRead(inputPinOpt26); //inputPinOpt26
	busVanAlimenterPin = etatBusVanBsi; // recupere valeur pour variable global

	if (etatBusVanBsi == 1) { // si le BSI alimente la voiture en 12v
  //mcp.begin(0);      // use default address 0 MCP23017 input output
		digitalWrite(relais1Securite, HIGH); // R2 relais de securite qui coupe l'alimnetation electrique 12V
		digitalWrite(relais2Securite, HIGH); // R2 relais de securite qui coupe l'alimnetation electrique 5V
		//Serial.println("BSI alimente la voiture en 12v");
		//lcd.backlight(); // Allumer LCD
	}
	else {
		digitalWrite(relais1Securite, LOW); // R2 relais de securite qui coupe l'alimnetation electrique 12V
		digitalWrite(relais2Securite, LOW); // R2 relais de securite qui coupe l'alimnetation electrique 5V a modificer le cablage pour que ca marche bien car ca coupe le BUS i2c
		//Serial.println("BSI alimente pas la voiture en 12v");
		//lcd.noBacklight(); // Eteindre LCD
	}


#if debugAfficherEtatEntre

	static unsigned long previousMillis = 0;
	const int interval = 1000;

	if (millis() - previousMillis >= interval) {
		previousMillis = millis();

		Serial.print(F("Contact moteur/ "));
		Serial.print(F("inputPinOpt22 : "));
		Serial.println(contactMoteurIN);

		Serial.print(F("Marche arriere/ "));
		Serial.print(F("inputPinOpt24 : "));
		Serial.println(marcheArriereIN);

		Serial.print(F("Alimentation + accesoire/ "));
		Serial.print(F("inputPinOpt23 : "));
		Serial.println(AlimentationAccesoireIN);

		Serial.print(F("Moteur voiture en marche/ "));
		Serial.print(F("inputPinOpt25 : "));
		Serial.println(moteurEnmarcheIN);

		Serial.print(F("BUS VAN alimenter BSI/ "));
		Serial.print(F("inputPinOpt26 : "));
		Serial.println(etatBusVanBsi);

		Serial.print(F("NO/ "));
		Serial.print(F("inputPinOpt27 : "));
		Serial.println(digitalRead(inputPinOpt27));

		Serial.print(F("NO/ "));
		Serial.print(F("inputPinOpt28 : "));
		Serial.println(digitalRead(inputPinOpt28));

		Serial.print(F("NO/ "));
		Serial.print(F("inputPinOptA0 : "));
		Serial.println(digitalRead(inputPinOptA0));


		Serial.println(F("-----------------------------"));

	}

#endif // debugAfficherEtatEntre

}


inline void InOutArduinoPin::outputPWMledSiege() {

# define debugPWMout 0

	#define ledSiegeFeuxCroisement 5 // l'intensite lumineuse des siege lorsque les feux sont allume
	#define ledSiegePorteOuvert 255 // l'intensite lumineuse des siege lorsque les porte sont ouvert

	

#if debugPWMout

		int pinPWMaTester = outputPinUln37; // PIN LED SOUS LES SIEGE BLEUT

		for (int brightness = 0; brightness < 255; brightness++) { // allumage progresif
			analogWrite(pinPWMaTester, brightness);
			delay(2);
		}
		for (int brightness = 255; brightness >= 0; brightness--) { // extinction progresif
			analogWrite(pinPWMaTester, brightness);
			delay(2);
		}
#endif // debugPWMout

	switch (ledSiegeEtat)
	{
	case 0: // PWM off
		digitalWrite(outputPinUln37, LOW);
		//ledSiegeEtat = 3;
		//analogWrite(outputPinUln37, 0); // PIN LED SOUS LES SIEGE BLEUT
		break;
	case 1: // PWM mode feux de croisement allumer
		analogWrite(outputPinUln37, ledSiegeFeuxCroisement); // PIN LED SOUS LES SIEGE BLEUT ledSiegeFeuxCroisement
		break;
	case 2: // PWM mode porte ouvert
		analogWrite(outputPinUln37, ledSiegePorteOuvert); // PIN LED SOUS LES SIEGE BLEUT ledSiegePorteOuvert
		break;
	case 3:
		break;
	}

}

inline void InOutArduinoPin::cameraMarcheArriere() {

	#define tempMaxCammeraAllume  10 // la camera reste allumer 10 seconde apres que l marche arriere a etait enlever
	#define debugAfficherInformation 0

	static unsigned long previousMillisCameraArriere = 0;        // will store last time LED was updated
	const long intervalTempsCameraArriere = 1000;           // interval at which to blink (milliseconds)
	static byte aCameraArriere = 0; // variable qui verifie si l'etat de la marche arriere a change
	static int conteur_temps_marche_arriere = 0; // conteur pour la tempo marche arriere
	
	//bool cameraRecule = 0; //outputPinUln35 0 = OFF ; 1 = ON

	if (marcheArriereBasicPin == 1)  // si la marche arriere et enclancher
	{
		cameraDeReculEtat = 1; // allumer la camera arriere
		aCameraArriere = 1; // on memorise que la marche arriere a etait bien mise
		conteur_temps_marche_arriere = 0; // remise a zero du compteur
#if debugAfficherInformation
		Serial.println("marche arriere mise");
#endif // debugAfficherInformation

	}

	if (aCameraArriere == 1 && marcheArriereIN == 0) { // si la marche arriere a etait mise puis enlever

		unsigned long currentMillis = millis(); // on demmare une tempo pour garder la camera actif X temps

		if (currentMillis - previousMillisCameraArriere >= intervalTempsCameraArriere) {

			conteur_temps_marche_arriere++; // conteur qui augmente

#if debugAfficherInformation
			Serial.print(F("conteur = "));
			Serial.println(conteur_temps_marche_arriere);
#endif // debugAfficherInformation

			previousMillisCameraArriere = currentMillis;  // on recupere la nouvelle valeur de milis

		}

		if (conteur_temps_marche_arriere == tempMaxCammeraAllume) { // une fois que le conteur et arriver a x seconde on etaint la camera

			cameraDeReculEtat = 0; // eteindre la camera arriere
			aCameraArriere = 0;
			conteur_temps_marche_arriere = 0; // remise a zero du compteur

#if debugAfficherInformation
			Serial.println(F("remise a zero"));
#endif // debugAfficherInformation

		}
	}

	if (cameraDeReculEtat == 1) {
		digitalWrite(outputPinUln35, HIGH);
	}
	else
	{
		digitalWrite(outputPinUln35, LOW);
	}
}


inline void InOutArduinoPin::phareAutomatique() {

	if (luxEtatLumunosite == 1) {

		//activationSortieMCP(3, 1);

		//Serial.println("LED state : ON");
	}
	if (luxEtatLumunosite == 0) {

		//Serial.println("LED state : OFF");
	}

}
#endif // InOutArduinoBasic_h