#pragma once
#ifndef peugeot_HC5_H
#define peugeot_HC5_H

#include <Arduino.h>
#include "peugeot_config.h"
// Code pin du HC-05 4027 !!
/*
https://www.electrodragon.com/w/CC2541
01234 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21
00000 0 0 0 0 0 0  0  0  0  0  0  0  0  0  0  \r \n

------------------------
0 = tout les seconde le telephone doit envoyer une tram avec le bit qui change d'etat pour indiquer qu'il a y une connection
------------------------
1234 == IDEN au cas ou que je doit rajouter d'autre option par la suite ( sa fait 16 code possible)
1001 le code qui sera attribuer pour l'application telephone (en decimal ca fait 9)

------------------------
5 = clignotant gauche
6 = clignotant droit
7 = feux de route
8 = feux de croisement
9 = feux de position
10 = Brouillard avant
11 = Brouillard arriere
12 = Fenetre conducteur monte
13 = Fenetre conducteur decente
14 = Fenetre passager monte
15 = Fenetre passager decente
16 = Claxon
17 = Ouverture Fermeture voiture
18 = Super condanation
19 = Fermer ouvrir tout les fenetre
20 = \r
21 = \n RETOUR A LA LIGNE (fin de la tram)
*/

//*********DEBUG*********
// Affiche les donne qui arrive dans le moniteur de serie
#define DEBUG_DATA_RECEIVE_DISPLAY 0 //defaut 0
// Affiche si la tram commence par le bon caracte et fini par le bon aussi dans le moniteur serie
#define DEBUG_AFFICHE_SI_TRAM_COMPLET 0
#define DEBUG_AFFICHER_TOUT_LES_BIT 0 // afficher les bit recus et stoque dans le tableau
#define DEBUG_AFFICHER_CLICK 0 
//*********DEBUG*********

//*********PARAMETRE*********
 // le caractere de debut de la tram
#define caracterStartData 13
// le caractere de fin de tram
#define caracterEndData 10 
// taille maximal des donne recus A REGLER ne pas oubliller que ca commne a ZERO !!! donc 22 - 1 = 21
#define lengthDATAreceive 21 //21
//*********PARAMETRE*********



class HC_05
{
public:
	HC_05();
	~HC_05();
	void serialEVENT();
	void dataOk();
	void afficherSiDebutEtFinTramCorrect();
	void sendDataWithBleutooth();

	void click();

private:

	byte arrayDataInput[25] = { 0 }; // le tableau qui va stoquer les donne recus
	byte btMessageLength = 0; // longeur de la tram recus
	bool etape = 0;

	byte arrayDonneIDEN[5] = { 0 }; // tableau qui stoque juste le IDEN debut de tram de 0 a 4
	//const byte tailleArrayDonneEtatButton = 15;
	byte arrayDonneEtatButton[15] = { 0 }; // tableau qui stoque tout les etat des button de byte 5 a byte 19
	//char
};

HC_05::HC_05() // fonctionne comme un setup
{
	Serial1.begin(9600);
	Serial.println(F("=>Bleutooth SETUP<="));
}

HC_05::~HC_05()
{
}

//-----------------------------------------------------------Received DATA-----------------------------------
void HC_05::serialEVENT() {

	if (Serial1.available() == (lengthDATAreceive + 1)) // si 23 octe disponible dans le buffer
	{
#if DEBUG_DATA_RECEIVE_DISPLAY 
		Serial.println(F("-----------")); //	Serial.print("Serial1 available : ");  Serial.println(Serial1.available());
#endif // DEBUG_DATA_RECEIVE_DISPLAY
		etape = 1; // go to etape 1
	}


	if (etape == 1) {
		while (Serial1.available()) {

#if DEBUG_DATA_RECEIVE_DISPLAY 
			Serial.print(F("btMessageLength : ")); Serial.print(btMessageLength);
#endif // DEBUG_DATA_RECEIVE_DISPLAY

			arrayDataInput[btMessageLength] = Serial1.read(); // lire les donne et les stoquer dans le tableau

#if DEBUG_DATA_RECEIVE_DISPLAY
			Serial.print(F("\t arrayDataInput <= value received : ")); Serial.println(arrayDataInput[btMessageLength]);
#endif // DEBUG_DATA_RECEIVE_DISPLAY

			//	delay(10); // si pas ce delay le program bug !!

			if (arrayDataInput[0] == caracterStartData) { // verifier si le premier caracte et bien ce quil faut 

				if (arrayDataInput[lengthDATAreceive] == caracterEndData && btMessageLength == lengthDATAreceive) { // si le caracter de fin et bon e le nombre de donne recus ateint 

					dataOk(); // appeler la fonction

				}

				btMessageLength++; // incrementer le taleau
			}
			else {

				arrayDataInput[0] = 0; // SET 0 ARRAY start and End data
				arrayDataInput[5] = 0; // SET 0 ARRAY  and End data
			}

		}
		//afficherSiDebutEtFinTramCorrect();

		btMessageLength = 0;
		etape = 0;
	}
}

void HC_05::dataOk() { // affiche les donne stoque dans le tableau

	if (arrayDataInput[0] == caracterStartData && arrayDataInput[lengthDATAreceive] == caracterEndData) {
		Serial.println(F("DATA COMPLET"));
		//afficherDonneRecus();

#if DEBUG_AFFICHER_TOUT_LES_BIT
		for (size_t i = 0; i < (lengthDATAreceive + 1); i++)
		{
			char affiche = arrayDataInput[i]; // convertir un DEC en ACCI
			Serial.println(affiche);
		}
#endif
		// Permet de separer les donne du IDEN DE DEBUT 
		for (size_t i = 0; i < lengthDATAreceive; i++)
		{
			if (i >= 1 && i <= 5) // Garde juste lle IDEM
			{
				arrayDonneIDEN[i] = arrayDataInput[i];

				//Serial.print("Donne IDEN = ");
				//char affiche = arrayDonneIDEN[i];
				//Serial.println(affiche);
			}
			if (i >= 6 && i <= 20) // Garde juste les Donne etat des button
			{
				byte message = arrayDataInput[i]; // conversion byte to char
				byte a = i - 6 ;// -6 pour avoir lautre tableau qui commence a zero!!
				arrayDonneEtatButton[a] = message;

				//Serial.print("Donne state button = ");
				//char affiche = arrayDonneEtatButton[a];
				//Serial.println(affiche);
				//Serial.println(a);
				//Serial.println(message);
				//delay(1);
			}
		}

		click();
		//mcpEtbleutooth(arrayDonneEtatButton, 15); // envoye les etat des button et la longeur des donne

		sendDataWithBleutooth(); // envoyer les donne au telephone

		//Serial.println(arrayDataInput[0]);
		//Serial.println(arrayDataInput[1]);
		//Serial.println(arrayDataInput[2]);
		//Serial.println(arrayDataInput[3]);
		//Serial.println(arrayDataInput[4]);
		//Serial.println(arrayDataInput[5]);
	}
}
//-----------------------------------------------------------Received DATA-----------------------------------

void HC_05::afficherSiDebutEtFinTramCorrect() { // Screen information

#if DEBUG_AFFICHE_SI_TRAM_COMPLET
	if (arrayDataInput[0] == caracterStartData) {
		Serial.println(F("START caractere : TRUE"));
	}
	else {
		Serial.println(F("START caractere : FALSE"));
	}
	if (arrayDataInput[lengthDATAreceive] == caracterEndData) {
		Serial.println(F("END caractere : TRUE"));
	}
	else {
		Serial.println(F("END caractere : FALSE"));
	}
#endif // DEBUG_AFFICHE_SI_TRAM_COMPLET
}

void HC_05::click() {

	//char avantArrayDonneEtatButton[15] = { 0 };
	boolean a = 0;
	//static boolean a_avant = 0;

	// positionement dans le tableau arrayDonneEtatButton ou ce trouve les button qui sont lue comme poussoir
	byte buttonPuch[15] = { 7, 8, 9, 10, 11, 12, 13, 14 };
	// positionement dans le tableau arrayDonneEtatButton ou ce trouve les button qui sont lue comme click
	byte buttonClic[15] = { 0, 1, 2, 3, 4, 5, 6};
	
	/*
				La position des sorties dans le tableau
		mcpOUTPUTstate[0] = ClignotantGauche
		mcpOUTPUTstate[1] = ClignotantDroit
		mcpOUTPUTstate[2] = FeuxDeRoute
		mcpOUTPUTstate[3] = FeuxDeCroisement
		mcpOUTPUTstate[4] = FeuxDePosition
		mcpOUTPUTstate[5] = BrouillardAV
		mcpOUTPUTstate[6] = BrouillardAR
		mcpOUTPUTstate[7] = FenetreConducteurMonte 
		mcpOUTPUTstate[8] = FenetreConducteurDecente 
		mcpOUTPUTstate[9] = FenetrePassagerMonte
		mcpOUTPUTstate[10] = FenetrePassagerDecente
		mcpOUTPUTstate[11] = Claxon 
		mcpOUTPUTstate[12] = Ouvrir/fermer la voiture
		mcpOUTPUTstate[13] = Super condanation
		mcpOUTPUTstate[14] = Fermer fenetre
		mcpOUTPUTstate[15] = NO
		*/
	//Permet davoir un clic en fonction du etat avant de la variable global
	for (size_t i = 0; i < 9; i++)
	{
			if (arrayDonneEtatButton[buttonClic[i]] == '1' && LASTEmcpOUTPUTstate[buttonClic[i]] == 0) {
				LASTEmcpOUTPUTstate[buttonClic[i]] = 1;

				mcpOUTPUTstate[buttonClic[i]] = !mcpOUTPUTstate[buttonClic[i]];
#if DEBUG_AFFICHER_CLICK
				Serial.print(F("Etat d'un button dans l'aplication : "));
				Serial.println(F("1111111111111111111"));
#endif // DEBUG_AFFICHER_CLICK

			}
			if (arrayDonneEtatButton[buttonClic[i]] == '0' && LASTEmcpOUTPUTstate[buttonClic[i]] == 1) {
				LASTEmcpOUTPUTstate[buttonClic[i]] = 0;
#if DEBUG_AFFICHER_CLICK
				Serial.print(F("Etat d'un button dans l'aplication : "));
				Serial.println(F("0000000000000000000"));
#endif // DEBUG_AFFICHER_CLICK

			}

		//Serial.print("Les sortie Click  : ");
		//Serial.println(buttonClic[i]);

	}

	for (size_t i = 0; i < 7; i++)
	{

		if (arrayDonneEtatButton[buttonPuch[i]] == '1') {
			mcpOUTPUTstate[buttonPuch[i]] = 1;
			//Serial.println("bbb");
		}
		if (arrayDonneEtatButton[buttonPuch[i]] == '0') {
			mcpOUTPUTstate[buttonPuch[i]] = 0;
			//Serial.println("aaa");
		}

	}
		//https://www.carnetdumaker.net/articles/pour-lamour-du-c-nutilisez-pas-les-fonctions-atoi-atol-atof-et-derivees/
		

		//int valeur;
		//sscanf(&arrayDonneEtatButton[7], "%s", &valeur);
	/*
		mcpOUTPUTstate[7] = c;//arrayDonneEtatButton[7]; // Fenetre conducteur UP
		mcpOUTPUTstate[8] = 0;//arrayDonneEtatButton[8]; // Fenetre conducteur DOWN
	     mcpOUTPUTstate[9] = 0;//arrayDonneEtatButton[9]; // Fenetre passage UP
		 mcpOUTPUTstate[10] = 0;//arrayDonneEtatButton[10]; // Fenetre passage DOWN
		 mcpOUTPUTstate[11] = 0;//arrayDonneEtatButton[11]; // Claxon
		 mcpOUTPUTstate[12] = 0;//arrayDonneEtatButton[12]; // fermer ouvrir a voiture
		 mcpOUTPUTstate[13] = 0;//arrayDonneEtatButton[13]; // Super condanation
		 mcpOUTPUTstate[14] = 0;//arrayDonneEtatButton[14]; // Fermer Fenetre
*/

}






//-----------------------------------------------------------Send DATA-----------------------------------
void HC_05::sendDataWithBleutooth() {

	byte dataArray[20] = { 0 };
	static byte a = 0;
	//uint8_t dataArray[10];
	//Serial.println("Envoye donne vers BT");

	if (a == 1)
	{
		a = 0;
	}
	else {
		a = 1;
	}

	dataArray[0] = '\n'; // Nouvelle ligne 

	dataArray[1] = a; // a chaque envoie cette valeur doit changer 
	dataArray[2] = 1;
	dataArray[3] = 0;
	dataArray[4] = 1;
	dataArray[5] = 1;
	dataArray[6] = mcpOUTPUTstate[0]; //stateOUTPUTClignotantGauche;
	dataArray[7] = mcpOUTPUTstate[1]; //stateOUTPUTClignotantDroit;
	dataArray[8] = mcpOUTPUTstate[4]; //stateOUTPUTFeuxDePosition;
	dataArray[9] = mcpOUTPUTstate[3]; //stateOUTPUTFeuxDeCroisement;
	dataArray[10] = mcpOUTPUTstate[2]; // varEtatDemandeFeuxDeRoute;
	dataArray[11] = mcpOUTPUTstate[5]; //varEtatDemandeBrouillardAV;
	dataArray[12] = mcpOUTPUTstate[6]; //varEtatDemandeBrouillardAR;
	dataArray[13] = mcpOUTPUTstate[12]; // voiture ferme ou ouvert
	dataArray[14] = mcpOUTPUTstate[13]; // voitur supercondanation
	dataArray[15] = mcpOUTPUTstate[11]; //stateOUTPUTClaxon;
	dataArray[16] = 0;
	dataArray[17] = 0;
	dataArray[18] = 0;
	dataArray[19] = '\r'; //'\r'retour chario
	//dataArray[10] = 11; //'\n'

	Serial1.write(dataArray, 20);
	//Serial.write(dataArray, 20);

//	delay(100);
}
//-----------------------------------------------------------Send DATA-----------------------------------
#endif // !peugeot_HC5_H