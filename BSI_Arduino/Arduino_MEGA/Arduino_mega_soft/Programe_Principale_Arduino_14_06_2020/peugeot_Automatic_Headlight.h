#pragma once
#ifndef peugeot_Automatic_Headlight_H
#define peugeot_Automatic_Headlight_H 

#include <Arduino.h>
#include "peugeot_config.h"

/*
valeur envoyer par la fonction phareAutoAllumage()
valeur 0 = il fait jours
valeur 1 = il fait nuit
valeur 2 = probleme de capteur de lumunosite ou de reglage

reglageDesValeurLumunosite(int f_valeurJour, int f_valeurJourMax, int f_valeurNuit, int f_valeurNuitMin, int f_valeurTunelle)
reglageDesValeurLumunosite(200, 900, 500, 50, 700); // envoyer les valeur de reglage a la fonction
*/
//https://www.youtube.com/watch?v=Bw0XL4qsFec

#define photoResistancePIN A1 // sur quelle pin ce trouve la photo resistance (defaut A1)
#define sunMinValue 150 // valeur lumunosite Min JOUR //150
#define sunMaxValue 10 //valeur lumunosite Max JOUR //50
#define nightValue 300 // valeur lumunosite NUIT //300
#define tunelValue 1020 // valeur lumunosite TUNEL capteur HS //1020
#define tunelMaxValue 700 // valeur lumunosite MAX //700

#define debugAfficherReglage 0 // affichier les reglage dans le moniteur deserie (defaut 0)
#define debugAffigerConteur 0 // affiche le conteur de changement de lumunosite (defaut 0)
#define debugValeurDucapteurAna 0 // valeur lue par le capteur analogique Lux (defaut 0)
#define debugAffichierEtatFeux 0 //(defaut 0)

class Automatic_Headlight
{
public:
	Automatic_Headlight();
	~Automatic_Headlight();
	int valeurNuit = 0; // valeur a partir de la quelle les phare s'allume
	int valeurJour = 0; // valeur a partie de la quelle les phare s'etaigne
	int valeurTunelle = 0; // veleur a partir de la quelle les phare s'allume passege dans un tunel
	int valeurJourMax = 0; // si depasse cette valeur cest quil a y un problem avec le capteur 
	int valeurTunelleMax = 0; // si depasse cette valeur cest quil a y un problem avec le capteur 

	void reglageDesValeurLumunosite(int f_valeurJour, int f_valeurJourMax, int f_valeurNuit, int f_valeurTunelleMax, int f_valeurTunelle);
	byte phareAutoGestionTemps(byte quelleTempoPrendre);
	byte phareAutoDetectionLumunositeExterieur(int valeurLumunosite);
	byte phareAutoAllumage(int valeurEntreAna);
	void phareAutolireLux();

private:

};

Automatic_Headlight::Automatic_Headlight() // fonctionne comme un setup
{
	Serial.println(F("=> Phare automatique OK <="));
	pinMode(13, OUTPUT);

	// light value : (sun min value , sun max value, nigth value, tunel value, tunel max value)
	reglageDesValeurLumunosite(sunMinValue, sunMaxValue, nightValue, tunelValue, tunelMaxValue); // envoyer les valeur de reglage a la fonction
}

Automatic_Headlight::~Automatic_Headlight()
{
	//delete automatic_headlight;
}

// recoit les reglage des lux a travers la fonction
inline void Automatic_Headlight::reglageDesValeurLumunosite(int f_valeurJour, int f_valeurJourMax, int f_valeurNuit, int f_valeurTunelleMax, int f_valeurTunelle) {

	valeurJour = f_valeurJour;
	valeurJourMax = f_valeurJourMax;
	valeurNuit = f_valeurNuit;
	valeurTunelleMax = f_valeurTunelleMax;
	valeurTunelle = f_valeurTunelle;

#if debugAfficherReglage

	Serial.print(F("Reglage valeurJour = ")); Serial.println(valeurJour);
	Serial.print(F("Reglage valeurJourMax recus= ")); Serial.println(valeurJourMax);
	Serial.print(F("Reglage valeurNuit recus= ")); Serial.println(valeurNuit);
	Serial.print(F("Reglage valeurNuitMin recus= ")); Serial.println(valeurTunelleMax);
	Serial.print(F("Reglage valeurTunelle recus= ")); Serial.println(valeurTunelle);
	Serial.print(F("\n"));

#endif // debugAfficherReglage

}

inline byte Automatic_Headlight::phareAutoGestionTemps(byte quelleTempoPrendre)
{
	//static = variable n'est plus supprime a la fin de la fonction la prochaine fois qu'on appellera la fonction
	//la variable aura conserve sa valeur elle et plus declrer une 2 eme fois 
	static unsigned long previousMillis = 0;        // will store last time LED was updated
	const long interval = 1000;           // interval at which to blink (milliseconds)
	static byte conteurSeconde = 0; // conteur qui compte les seconde
	const byte conteurTempoJourNuit = 8; // tempo a partir de quand lordre de eteindre ou d'allumer les feux arrive
	const byte conteurTempsTunelle = 3; // tempo a part dequand on sais qont passe sous un tunel
	byte valeurRetourneTemps = 0; //0 = rien , 1 = tempo il va faire jour ou nuit , 2 tempo passage dans un tunel

	if (quelleTempoPrendre == 1 || quelleTempoPrendre == 2 || quelleTempoPrendre == 4) {

		if (millis() - previousMillis >= interval) { // millis gestion de temps 

			previousMillis = millis(); // soquer la valeu de millis dans previousMillis
			conteurSeconde++; // incrementer le compteur

#if debugAffigerConteur
			Serial.print(F("conteur seconde = ")); Serial.println(conteurSeconde);
#endif // debugAffigerConteur

		}
	}

	if (conteurSeconde == conteurTempoJourNuit && quelleTempoPrendre == 1) { // attend que le compteur a ateint la valeur voulue pour nuit ou jours

		valeurRetourneTemps = 1; // retourne valeur il va faire jour ou nuit
#if debugAffigerConteur
		Serial.println(F("conteurTempoJourNuit"));
#endif // debugAffigerConteur
	}

	if (conteurSeconde == conteurTempsTunelle && quelleTempoPrendre == 2) { // attend que le compteur a ateint la valeur voulue pour nuit ou jours

		valeurRetourneTemps = 2; // retourne valeur passage dans un tunel
#if debugAffigerConteur
		Serial.println(F("conteurTempsTunelle"));
#endif // debugAffigerConteur
	}

	if (quelleTempoPrendre == 3) { // reset du conteur par ordre

		conteurSeconde = 0; // reset du compteur (Sert peux tetre a rien A VERIFIER)
#if debugAffigerConteur
		Serial.println(F("reset conteur"));
#endif // debugAffigerConteur
	}

	return valeurRetourneTemps;
}
//***************************************************************************************
//***************************************************************************************

inline byte Automatic_Headlight::phareAutoDetectionLumunositeExterieur(int valeurLumunosite)
{
	//const int valeurNuit = 500; // valeur a partir de la quelle les phare s'allume
	//const int valeurJour = 200; // valeur a partie de la quelle les phare s'etaigne
	//const int valeurTunelle = 700; // veleur a partir de la quelle les phare s'allume passege dans un tunel
	//const int valeurJourMax = 900; // si depasse cette valeur cest quil a y un problem avec le capteur 
	//const int valeurNuitMin = 50; // si depasse cette valeur cest quil a y un problem avec le capteur 
	byte valeurRetourneLum = 0; //0 rien , 1 = nuit, 2 = jours, 3 = tunelle,

#if debugValeurDucapteurAna
	Serial.print(F("sensor phare auto = ")); Serial.println(valeurLumunosite);
#endif // debugValeurDucapteurAna



	if (valeurLumunosite > valeurTunelle&& valeurLumunosite < valeurTunelleMax) // si la lumunosite et inferieur a la valeur tunel
	{
		valeurRetourneLum = 3; // valeur a retourne tunel
#if debugAffichierEtatFeux
		Serial.print(F("TUNEL = ")); Serial.println(valeurRetourneLum);
#endif // debugAffichierEtatFeux

		
	}
	else if (valeurLumunosite > valeurNuit&& valeurLumunosite < valeurTunelle) // si la lumunosite et inferieur a veleur nuit && supperieur a valeur tunel
	{
		valeurRetourneLum = 1; // valeur a retourne nuit
#if debugAffichierEtatFeux
		Serial.print(F("NUIT = ")); Serial.println(valeurRetourneLum);
#endif // debugAffichierEtatFeux

		
	}
	else if (valeurLumunosite < valeurJour && valeurLumunosite > valeurJourMax) // si la lumunocite et supperieur a valeur jours 
	{
		valeurRetourneLum = 2; // valeur a retourne jour
#if debugAffichierEtatFeux
		Serial.print(F("JOUR = ")); Serial.println(valeurRetourneLum);
#endif // debugAffichierEtatFeux

		
	}

	else if (valeurLumunosite > valeurJour&& valeurLumunosite < valeurNuit) {
		valeurRetourneLum = 4; // valeur a retourne jour
#if debugAffichierEtatFeux
		Serial.print(F("ENTRE jour/nuit = ")); Serial.println(valeurRetourneLum);
#endif // debugAffichierEtatFeux

		
	}
	else if (valeurLumunosite < valeurJourMax || valeurLumunosite > valeurTunelleMax) {
		valeurRetourneLum = 5; // valeur a retourne jour
#if debugAffichierEtatFeux
		Serial.println(F("ERREUR depassement de valeur de lumunosite fixe verifier les reglage ou le capteur"));
#endif // debugAffichierEtatFeux

		
	}

	return valeurRetourneLum; // retourne la valeur calcule par la fonction
}
//***************************************************************************************
//***************************************************************************************
inline void Automatic_Headlight::phareAutolireLux() {

	phareAutoAllumage(analogRead(photoResistancePIN));
	
}

//***************************************************************************************
//***************************************************************************************

//inline byte Automatic_Headlight::phareAutoAllumage(int valeurEntreAna) //byte valeurRetourneLum byte valeurRetourneTemps
inline byte Automatic_Headlight::phareAutoAllumage(int valeurEntreAna) //byte valeurRetourneLum byte valeurRetourneTemps
{

	static byte jourOUnuit = 0; // 0 = feux etaint , 1= feux allumer , 2 = feux etaint 
	//byte valeurRetourneLum = 0; //0 rien , 1 = nuit, 2 = jours, 3 = tunelle,
	static byte quelleTempoPrendre = 0;
	byte valeurRetourneTemps = 0; //0 = rien , 1 = tempo il va faire jour ou nuit , 2 tempo passage dans un tunel
	static int valeurRetourneLum = 0;
	static int valeurRetourneLumApres = 0;


	valeurRetourneLum = phareAutoDetectionLumunositeExterieur(valeurEntreAna);
#if debugAffichierEtatFeux
	Serial.print(F("valeurRetourneLum  = ")); Serial.println(valeurRetourneLum); // dans quelle cituation de lumunoste on se trouve
#endif // debugAffichierEtatFeux

	
	//Serial.print("valeurRetourneTemps  = "); Serial.println(valeurRetourneTemps);


		//------------------------------------------------------------
		//----------------------Controle jour nuit-------------------------------
		//------------------------------------------------------------

	if (valeurRetourneLum == 1 && jourOUnuit == 0) { // si il fait nuit et phare etait eteint

		quelleTempoPrendre = 1; // tempo a prendre 1 car cest quil fait jour ou nuit
		valeurRetourneTemps = phareAutoGestionTemps(quelleTempoPrendre); // envoyer a la fonction quelle tempo elle doit utilise (tempo pont ou nuit et jors)

		if (valeurRetourneTemps == 1) { // tempo arrive a la fin il va faire jours ou nuit
			jourOUnuit = 1; // allumer les feux
			quelleTempoPrendre = 3;
			phareAutoGestionTemps(quelleTempoPrendre); // envoyer la valeur a la fonction
		}

	}

	else if (valeurRetourneLum == 2 && jourOUnuit == 1) { //si il commence a faire jours et les feux etait allumer

		quelleTempoPrendre = 1; // tempo a prendre 1 car cest quil fait jour ou nuit
		valeurRetourneTemps = phareAutoGestionTemps(quelleTempoPrendre); // envoyer a la fonction quelle tempo elle doit utilise (tempo pont ou nuit et jors)

		if (valeurRetourneTemps == 1) { // tempo arrive a la fin il va faire jours ou nuit
			jourOUnuit = 0; // eteindre les feux
			quelleTempoPrendre = 3;
			phareAutoGestionTemps(quelleTempoPrendre); // envoyer la valeur a la fonction
		}

	}

	if (valeurRetourneLumApres != valeurRetourneLum) { // reset du compteur s'il et pas arrive a la fin avant un changement d'etat

		quelleTempoPrendre = 3; // prend la vealeu pour metre le conteur a zero sil a commencer a marcher
		phareAutoGestionTemps(quelleTempoPrendre); // envoyer a la fonction la valeur
#if debugAffichierEtatFeux
		Serial.println(F("RESET CONTEUR"));
#endif // debugAffichierEtatFeux

		
		valeurRetourneLumApres = valeurRetourneLum; // recuperer la nouvelle valeur
	}


	//------------------------------------------------------------
	//----------------------Controle jour nuit-------------------------------
	//------------------------------------------------------------

	//------------------------------------------------------------
	//----------------------Controle tunel-------------------------------
	//------------------------------------------------------------


	if (valeurRetourneLum == 3 && jourOUnuit == 0) { // si il tunel et phare etait eteint

		quelleTempoPrendre = 2; // tempo a prendre 2 car cest quil a y tunel
		valeurRetourneTemps = phareAutoGestionTemps(quelleTempoPrendre); // envoyer a la fonction quelle tempo elle doit utilise (tempo pont ou nuit et jors)

		if (valeurRetourneTemps == 2) { // tempo arrive a la fin il va faire jours ou nuit
			jourOUnuit = 1; // allumer les feux
			quelleTempoPrendre = 3;
			phareAutoGestionTemps(quelleTempoPrendre); // envoyer la valeur a la fonction
		}
	}
	//------------------------------------------------------------
	//----------------------Controle tunel-------------------------------
	//------------------------------------------------------------
	if (valeurRetourneLum == 5) { //si il a y un problem avec le capteur ou les reglage
		jourOUnuit = 2;
	}
#if debugAffichierEtatFeux
	Serial.print(F("etatDesFeux  = ")); Serial.println(jourOUnuit); Serial.println("  ");
#endif // debugAffichierEtatFeux

	luxEtatLumunosite = jourOUnuit;

	return jourOUnuit; // retourne l'etat dans les quelle doive se trouver les feux
}
//***************************************************************************************
#endif // !AUTOMATICHEADLIGHT_H