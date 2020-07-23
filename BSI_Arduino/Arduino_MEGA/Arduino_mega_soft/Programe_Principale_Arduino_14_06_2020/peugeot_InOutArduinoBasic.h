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
	void ClaxonVoitureFermeOuvert();

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

//==========================================================================================================

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

//==========================================================================================================

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

//==========================================================================================================

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

//==========================================================================================================

inline void InOutArduinoPin::phareAutomatique() {

	switch (etatDuNeiman)
	{
	case 0: // contact coupe

#if OPTION_11
		ClaxonVoitureFermeOuvert();
#endif //OPTION_11

		break;
	case 5 || 4: // moteur a etait coupe
#if OPTION_9

		// contact moteur coupe et feux etait allumer alor petit tempo puis on les eteint
		if (mcpOUTPUTstate[4] == 1 && mcpOUTPUTstate[3] == 1) { // 

	//Serial.println("Je suis la ");

			if (TIME10.DelayMillis() == 1)
			{
				Serial.println("Moteur etait allumer et phares etait allumer aussi ");
				mcp_23017->activationSortieMCP(3, 0); // eteindre Feux de position
				mcp_23017->activationSortieMCP(6, 0); // allumer Feux de croisement
			}
		}
#endif // OPTION_9

		break;
	case 3: // contact moteur

		break;
	case 7: // moteur en marche
#if OPTION_9
			//*********************************Pares automatique moteur en marche***************************
		if (luxEtatLumunosite == 1) {

			mcp_23017->activationSortieMCP(3, 1); // allumer Feux de position
			mcp_23017->activationSortieMCP(6, 1); // allumer Feux de croisement

			if (TIME4.DelayMillis(1000) == 1)
			{
				Serial.println("PHARES state moteur en marche : ON");
			}
		}
		if (luxEtatLumunosite == 0) {

			mcp_23017->activationSortieMCP(3, 0); // eteindre Feux de position
			mcp_23017->activationSortieMCP(6, 0); // allumer Feux de croisement

			if (TIME4.DelayMillis(1000) == 1)
			{
				Serial.println("PHARES state moteur en marche : OFF");
			}
		}
		/*
		if (delayPhareAutoOff.DelayMillis(10000) == 1)
		{
			Serial.println("Moteur etait allumer et phares etait allumer aussi ");
		}
		*/
		//*********************************Pares automatique moteur en marche***************************
#endif // OPTION_9
		break;
	default:
		break;
	}


	//--------------------------controle phare vouture ouvertfermer ou porte ouvert----------------------------------------
	static byte programePhareAutoMoteurCoupe = 0;

#if OPTION_10

	if (etatDuNeiman == 0 || etatDuNeiman == 1) { // si contact coupe ou contact accesoir

		if (etatVoiture == 0 && etatVoitureAVANT != etatVoiture) { // si letat de la voiture a changer et que la voiture et ouvert = voiture vient detre ouvert

				Serial.println("Voiture vient d'etre ouvert");

			etatVoitureAVANT = etatVoiture;
			programePhareAutoMoteurCoupe = 1;
		}

		if (etatVoiture == 1 && etatVoitureAVANT != etatVoiture) { // si letat de la voiture a changer et que la voiture et fermer = voiture vient detre fermer

				Serial.println("Voiture vient d'etre fermer");

			etatVoitureAVANT = etatVoiture;
			programePhareAutoMoteurCoupe = 1;
		}


		if (etatDesPorte == 0 && etatDesPorteAVANT != etatDesPorte) { // si letat des porte a changer et une porte femerer = une porte a etait fermer

				Serial.println("Une des porte vient d'etre fermer");

				etatDesPorteAVANT = etatDesPorte;
			programePhareAutoMoteurCoupe = 1;
		}
		// si une porte et ouvert (passage ou conducteur) la valeur va a 2 ou 1
		if (etatDesPorte != 0 && etatDesPorteAVANT != etatDesPorte) { // si letat des porte a changer et que les porte sont par femrer = une ou plusieur porte et ouver

				Serial.println("Une des porte vient d'etre ouvert");

				etatDesPorteAVANT = etatDesPorte;
			programePhareAutoMoteurCoupe = 1;
		}
	}

	if (etatDuNeiman == 3) { // si contact moteur
		programePhareAutoMoteurCoupe = 3;
		if (TIME4.DelayMillis(1000) == 1)
		{
			Serial.println("Phare automatique moteur arreter Contact moteur mit ");
		}
	}


	switch (programePhareAutoMoteurCoupe) { // structure le deroulement du programe des phare auto lorque le moteur et a l'arret
	case 1:

		if (luxEtatLumunosite == 1) { // si il fait nuit
			Serial.println("Il fait nuit alor on allume les :");

			if (etatDesPorte != 0)
			{ // si une des portes et ouvert
				Serial.println("Feux de croisement ON");
				Serial.println("Feux de brouillard Avant ON");
				mcp_23017->activationSortieMCP(3, 0); // eteindre feux de position
				mcp_23017->activationSortieMCP(6, 1); // allumer Feux de croisement
				mcp_23017->activationSortieMCP(4, 1); // eteindre Feux de brouillard AV
				ledSiegeEtat = 2; // allumer les LED sous les siege mode PWM
				programePhareAutoMoteurCoupe = 2;
			}

			else // alors pas de porte ouver
			{
				Serial.println("Feux de position ON");
				Serial.println("Feux de croisement ON");
				mcp_23017->activationSortieMCP(3, 1); // allumer Feux de position
				mcp_23017->activationSortieMCP(6, 1); // allumer Feux de croisement
				mcp_23017->activationSortieMCP(4, 0); // eteindre Feux de brouillard AV
				ledSiegeEtat = 2; // allumer les LED sous les siege mode PWM
				programePhareAutoMoteurCoupe = 2;
			}

		}
		else // alor il fait jours !
		{
			Serial.println("Il fait jours alor on allume les :");
			Serial.println("Feux de brouillard Avant ON");
			mcp_23017->activationSortieMCP(4,1);// allumer feux de brouillard avant
			ledSiegeEtat = 0; // allumer les LED sous les siege mode PWM
			programePhareAutoMoteurCoupe = 2;
		}
		break;

	case 2:

		static int i = 0;

		if (TIME5.DelayMillis(1000) == 1) {
			
			i++;
			Serial.println(i);
		}

	//	if (TIME6.DelayMillis(20000) == 1) { // si pas de changement d'etat constate apres alllumage des pharesen les etait apres une tempo
		//	programePhareAutoMoteurCoupe = 3;
			//Serial.println("Je suis ici DELAY");
		//}

		if (TIME11.DelayMillis() == 1)
		{
		programePhareAutoMoteurCoupe = 3;
		i = 0;
		Serial.println("Je suis ici DELAY");

		}
		break;

	case 3:

		// si un des feux et allumer on les etaint
		if (mcpOUTPUTstate[2] == 1 || mcpOUTPUTstate[3] == 1 || mcpOUTPUTstate[4] == 1 || mcpOUTPUTstate[5] == 1 || mcpOUTPUTstate[6] == 1)
		{

			Serial.println("Compteur delais des feux auto moteur arreter");
			Serial.println("Feux de position OFF");
			Serial.println("Feux de croisement OFF");
			Serial.println("Feux de brouillard Avant OFF");

			mcp_23017->activationSortieMCP(3, 0); // eteindre Feux de position
			mcp_23017->activationSortieMCP(6, 0); // eteindre Feux de croisement
			mcp_23017->activationSortieMCP(4, 0); // eteindre Feux de brouillard AV
			ledSiegeEtat = 0; // allumer les LED sous les siege mode PWM

		}


		programePhareAutoMoteurCoupe = 0;
		break;
	}
	//--------------------------controle phare vouture ouvertfermer ou porte ouvert----------------------------------------
#endif // OPTION_10


}
#endif // InOutArduinoBasic_h
//==========================================================================================================


inline void InOutArduinoPin::ClaxonVoitureFermeOuvert() {

	static byte etape = 1;

	if (etatVoiture == 1 && etape == 1) { // voiture fermer

		mcp_23017->activationSortieMCP(0, 1); // allumer claxon

		if (TIME20.DelayMillis() == 1)
		{
			digitalWrite(LED_PIN_13, LOW);
			Serial.println("0");
			mcp_23017->activationSortieMCP(0, 0); // eteindre claxon
			//mcp_23017->activationSortieMCP(5, 0);
			etape = 2;
		}

		
	}
	if (etatVoiture == 0 && etape == 2) { // voiture ouvert

		static byte w = 0;

		//mcp_23017->activationSortieMCP(0, 1); // allumer claxon

		if (TIME20.DelayMillis() == 1 && w == 0)
		{
			digitalWrite(LED_PIN_13, LOW);
			Serial.println("00");
			//mcp_23017->activationSortieMCP(0, 0); // eteindre claxon
			//mcp_23017->activationSortieMCP(0, 1); // allumer claxon
			w = 1;
		
		}
		
		if (TIME30.DelayMillis() == 1 && w == 1)
		{
			digitalWrite(LED_PIN_13, LOW);
			Serial.println("000");
			//mcp_23017->activationSortieMCP(0, 0); // eteindre claxon
			w = 0;
			etape = 1;
		}


		
	}


	/*
	static bool BB = 0;

	//	if (essueGlaceAuto == 1 && BB == 0) {

			//Serial.println("Je suis ici");

	if (BB == 0)
	{
		mcp_23017->activationSortieMCP(0, 1); // allumer claxon
		//mcp_23017->activationSortieMCP(5, 1);
		digitalWrite(LED_PIN_13, HIGH);
		Serial.println("1");

	}

	if (TIME20.DelayMillis() == 1 && BB == 0)
	{
		digitalWrite(LED_PIN_13, LOW);
		Serial.println("0");
		mcp_23017->activationSortieMCP(0, 0); // eteindre claxon
		//mcp_23017->activationSortieMCP(5, 0);
		BB = 1;

	}
	*/
}