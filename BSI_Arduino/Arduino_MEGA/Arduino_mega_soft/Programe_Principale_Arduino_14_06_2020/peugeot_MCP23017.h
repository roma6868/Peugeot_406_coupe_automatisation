#pragma once
#ifndef peugeot_MCP23017_H
#define peugeot_MCP23017_H

#include <Arduino.h>
#include "peugeot_config.h"
#include <Adafruit_MCP23017.h> // module entre sortie I2C
#include <Wire.h>

#define GPA0 21 //21 //claxon
#define GPA1 22 //clignotant gauche
#define GPA2 23 //clignotant droit
#define GPA3 24 //feux de posiiton
#define GPA4 25 //feux de brouillard avant
#define GPA5 26 //feux de brouillard arriere
#define GPA6 27 //feux de croisement
#define GPA7 28 //feux de route

#define GPB0 1 //NO
#define GPB1 2 //NO
#define GPB2 3 //NO
#define GPB3 4 //fenetre conducteur monte
#define GPB4 5 //fenetre conducteur decente
#define GPB5 6 //fenetre pasagge monte
#define GPB6 7 //fenetre passage decente
#define GPB7 0 //NO

#define DebugMCP23017 0 // 0 mode debug desactive ; 1 mode debug active (activation des sortie depuis PC)

Adafruit_MCP23017 mcp;

class MCP_23017
{
public:
	MCP_23017();
	~MCP_23017();

	void recoitDonnePCpourMCP23017();
	void lectureDesOcteDuBuffer(byte nombreOcteMax);
	void decodeCommande(char receivedData[], byte tailleTableau);
	void activation(char dataCommade[], char dataState, String dataCommadeString);
	void testLED(char dataCommade[], char dataState, byte nomDesSortieTaille);
	void activationSortieMCP(byte position, boolean state);
	void activationDesSortie();

private:

};

MCP_23017::MCP_23017()
{

	Serial.println(F("=>P406_coupe_MCP23017_SETUP<="));

	Serial.print(F("=> Mode (1 = Debug ; 0 = Normal) : "));
	Serial.println(DebugMCP23017);

	//����������������������� MCP23017 input output ��������������������������������������������������������������������������������

	mcp.begin(0);      // use default address 0 MCP23017 input output

	delay(100);
	mcp.pinMode(GPA0, OUTPUT);
	mcp.digitalWrite(GPA0, HIGH);

	mcp.pinMode(GPA1, OUTPUT);
	mcp.digitalWrite(GPA1, HIGH);

	mcp.pinMode(GPA2, OUTPUT);
	mcp.digitalWrite(GPA2, HIGH);

	mcp.pinMode(GPA3, OUTPUT);
	mcp.digitalWrite(GPA3, HIGH);

	mcp.pinMode(GPA4, OUTPUT);
	mcp.digitalWrite(GPA4, HIGH);

	mcp.pinMode(GPA5, OUTPUT);
	mcp.digitalWrite(GPA5, HIGH);

	mcp.pinMode(GPA6, OUTPUT);
	mcp.digitalWrite(GPA6, HIGH);

	mcp.pinMode(GPA7, OUTPUT);
	mcp.digitalWrite(GPA7, HIGH);

	//******************************************

	mcp.pinMode(GPB0, OUTPUT);
	mcp.digitalWrite(GPB0, HIGH);

	mcp.pinMode(GPB1, OUTPUT);
	mcp.digitalWrite(GPB1, HIGH);

	mcp.pinMode(GPB2, OUTPUT);
	mcp.digitalWrite(GPB2, HIGH);

	mcp.pinMode(GPB3, OUTPUT);
	mcp.digitalWrite(GPB3, HIGH);

	mcp.pinMode(GPB4, OUTPUT);
	mcp.digitalWrite(GPB4, HIGH);

	mcp.pinMode(GPB5, OUTPUT);
	mcp.digitalWrite(GPB5, HIGH);

	mcp.pinMode(GPB6, OUTPUT);
	mcp.digitalWrite(GPB6, HIGH);

	mcp.pinMode(GPB7, OUTPUT);
	mcp.digitalWrite(GPB7, HIGH);


	//����������������������� MCP23017 input output ��������������������������������������������������������������������������������

//#define GPA0 13 // pour tester a enlever a la fin 
	pinMode(13, OUTPUT);

	int donnePortSeriePC;

	//byte etatDesSortieMCP[14] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


	/*
	byte Aclaxon = 3; //GPA0 // 0 = OFF ; 1 = ON
	byte AclignotantGauche = 3; //GPA1 // 0 = OFF ; 1 = ON
	byte AclignotantDroit = 3; // GPA2 // 0 = OFF ; 1 = ON
	byte AfeuxDePosition = 3; //GPA3 // 0 = OFF ; 1 = ON
	byte AfeuxDeBrouillardAvant = 3; //GPA4 // 0 = OFF ; 1 = ON
	byte AfeuxDeBrouillardArriere = 3; //GPA5 // 0 = OFF ; 1 = ON
	byte AfeuxDeCroisement = 3; //GPA6 // 0 = OFF ; 1 = ON
	byte AfeuxDeRoute = 3; //GPA7 // 0 = OFF ; 1 = ON

	//GPB0
	//GPB1
	//GPB2
	byte fenetreConducteur = 0; // GPB3 & GPB4 // 0 = OFF ; 1 = GO UP ; 2 = GO DOWN
	byte fenetrePassager = 0; // GPB5 & GPB6 // 0 = OFF ; 1 = GO UP ; 2 = GO DOWN
	//GPB7
	*/
}

MCP_23017::~MCP_23017()
{
}


inline void MCP_23017::recoitDonnePCpourMCP23017() { // attend d'avoir assez de donne dans le buffer serie

	//mcp.digitalWrite(GPA3, HIGH);
	//delay(1000);
	//mcp.digitalWrite(GPA3, LOW);
	//delay(1000);

	const byte nombreOcteMax = 7;
	byte availableBytes; // stoque le nombre d'octe disponible dans le buffer

	availableBytes = Serial.available(); // regarde le nombre d'octe disponible dans le buffer

	//Serial.print("availableBytes = "); Serial.println(availableBytes);

	if (availableBytes == nombreOcteMax) {

		Serial.print(F("nombre octe buffer recus = ")); Serial.println(nombreOcteMax);

		lectureDesOcteDuBuffer(nombreOcteMax); // appeler la fonction et lui envoyer le nombre d'octe max que on doit recevoir
	}
	//else if (availableBytes > 0 )
	//{
	//	while (Serial.available()) { char inChar = Serial.read(); } // vider le buffer
	//}

}

inline void MCP_23017::lectureDesOcteDuBuffer(byte nombreOcteMax) { // lit les donne du buffer serie

	const byte tailleTableau = 7;//nombreOcteMax; // avoir le tableau de la meme tail que les donne recus
	char receivedData[tailleTableau] = { 0 }; // le tableau qui stoque les donne lus du buffer serie

	Serial.println(F("=> lectureDesOcteDuBuffer"));

	for (byte n = 0; n < nombreOcteMax; n++) // on lit les donne stoquer dans le buffer et on les stoque dans un tableau
	{
		receivedData[n] = Serial.read(); // lit et stoque les donne dans un tableau qui se acremonte

		Serial.println(receivedData[n]);
		//Serial.print("Lecture des donne du buffer : "); Serial.println(n);
	}
	decodeCommande(receivedData, tailleTableau); // appeler la fonction

}

inline void MCP_23017::decodeCommande(char receivedData[], byte tailleTableau) {

	char dataCommade[5] = { 0 }; // variable qui stoque la commande
	char dataState = 0; // variable qui stoque l'etat
	String dataCommadeString;

	byte espaceEntreDonne = 4; // les donne commande et etat sont separer par un ; donc faut definir la position de ce caractere (defaut 4 )


	if (receivedData[espaceEntreDonne] == ';') { // detection du caractere ( ; )

		Serial.print(F("Commande Decode : "));
		for (size_t i = 0; i < espaceEntreDonne; i++)
		{
			dataCommade[i] = receivedData[i];
			Serial.print(dataCommade[i]);
			//Serial.print(i);
			dataCommadeString = String(dataCommadeString + dataCommade[i]);
		}

		Serial.println("");
		//Serial.println(dataCommadeString);

		if (receivedData[espaceEntreDonne + 2] == '\n') { // detection de fin de ligne 
			// atoi( input ) // convertion char to INT
			dataState = receivedData[espaceEntreDonne + 1];
			Serial.print(F("State Decode : "));
			Serial.println(dataState);

			activation(dataCommade, dataState, dataCommadeString); // envoyer a la fonction la commande et l'etat
			return;
		}
	}
}

inline void MCP_23017::activation(char dataCommade[], char dataState, String dataCommadeString) {

	const byte nomDesSortieTaille = 16;
	char* nomDesSortie[nomDesSortieTaille] = { "GPA0", "GPA1", "GPA2", "GPA3", "GPA4", "GPA5", "GPA6", "GPA7", "GPB0", "GPB1", "GPB2", "GPB3", "GPB4", "GPB5", "GPB6", "GPB7" }; // valeur axepte comme nom du pin de sortie
	byte numeroDesPinMCP[nomDesSortieTaille] = { GPA0 , GPA1 , GPA2 , GPA3 , GPA4 , GPA5 , GPA6 , GPA7 , GPB0 , GPB1 , GPB2 , GPB3 , GPB4 , GPB5 , GPB6 , GPB7 };


	const byte stateTaille = 3;
	char state[3] = { '0','1' }; // valeur acepte comme etat
	bool a = 0;

	//Serial.println(dataState);
	//Serial.println(dataCommade);
	Serial.println("");
	for (size_t i = 0; i < nomDesSortieTaille; i++)
	{

		//Serial.print("Commande recus : "); Serial.println(dataCommade);
		//Serial.print("Teste donne autorise : "); Serial.println(nomDesSortie[i]);
		//dataCommadeString
		if (strcmp(dataCommade, nomDesSortie[i]) == 0) { //https://forum.arduino.cc/index.php?topic=279822.0

			Serial.println(F("=> Commande dans la liste"));
			a = 1;
			break;
		}
		if (i == nomDesSortieTaille - 1) {
			Serial.println(F("=> ERREUR Commande exite pas ! (Ex : GPB1;1)"));
		}
		//mcp.digitalWrite(GPA0, HIGH);
	//mcp.digitalWrite(GPA0, LOW);
	}

	if (a == 1)
	{
		for (size_t i = 0; i < stateTaille; i++)
		{
			if (dataState == state[i])
			{
				Serial.println(F("=> VALEUR Etat OK"));
				a = 0;
				testLED(dataCommade, dataState, nomDesSortieTaille);
				return;
			}
			if (i == stateTaille - 1) {
				Serial.println(F("=> ERREUR VALEUR etat accepte 0 ou 1"));
			}
		}
	}
}

// fonction mode debug PC
inline void MCP_23017::testLED(char dataCommade[], char dataState, byte nomDesSortieTaille)
{
	//mcp.digitalWrite(GPA3, LOW);
	//int a = atoi(dataState);

	String commandeEnString;

	int commandeApresConvertion = 0; // converti les donne du tableau "dataCommade" en pin de sortie a laide de String

	//Serial.print("Commande = ");
	//Serial.println(dataCommade);

	//Serial.print("Etat = ");
	//Serial.println(dataState);

	//****************************Convertion char to int*****************
	if (dataState == '0')
	{
		dataState = 1;
	}
	if (dataState == '1')
	{
		dataState = 0;
	}
	//****************************Convertion char to int*****************
	commandeEnString = String(commandeEnString + dataCommade);

	// déclaration d'un tableau de pointeurs de chaîne de caractère
	char* myString[] = { "GPA0", "GPA1", "GPA2", "GPA3", "GPA4", "GPA5", "GPA6", "GPA7", "GPB0", "GPB1", "GPB2", "GPB3", "GPB4", "GPB5", "GPB6", "GPB7" }; // valeur axepte comme nom du pin de sortie
	byte numero[] = { GPA0 , GPA1 , GPA2 , GPA3 , GPA4 , GPA5 , GPA6 , GPA7 , GPB0 , GPB1 , GPB2 , GPB3 , GPB4 , GPB5 , GPB6 , GPB7 };


	for (size_t i = 0; i < nomDesSortieTaille; i++) // lit le string recus par le port serie et le compart avec un char disponible dans la liste 
	{
		if (commandeEnString == myString[i])
		{
			commandeApresConvertion = numero[i];
			//Serial.print("Conteur I : ");
			//Serial.println(i);
			activationSortieMCP(i, dataState);
		}
	}
	

	Serial.print(F("MCP pin OUT  = "));
	Serial.println(commandeApresConvertion);

	//mcp.digitalWrite(commandeApresConvertion, dataState);
	digitalWrite(commandeApresConvertion, dataState);
}

inline void MCP_23017::activationSortieMCP(byte position,boolean state) {

	switch (position)
	{
	case 0: // Claxon
		mcpOUTPUTstate[11] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[11], state);
		break;

	case 1: // Clignotant gauche
		mcpOUTPUTstate[0] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[0], state);
		break;

	case 2: // clignotant droit
		mcpOUTPUTstate[1] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[2], state);
		break;
	case 3: // Feux de position
		mcpOUTPUTstate[4] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[4], state);
		break;
	case 4: // Brouillard AV
		mcpOUTPUTstate[5] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[5], state);
		break;
	case 5: // Brouillard AR
		mcpOUTPUTstate[6] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[6], state);
		break;
	case 6: // Feux de croisement
		mcpOUTPUTstate[3] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[3], state);
		break;
	case 7: // Feux de route
		mcpOUTPUTstate[2] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[2], state);
		break;
	case 8:

		break;
	case 9:

		break;
	case 10:

		break;
	case 11: //Fenetre conducteur UP
		mcpOUTPUTstate[7] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[7], state);
		break;
	case 12: // Fenete conducteur DOWN
		mcpOUTPUTstate[8] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[8], state);
		break;
	case 13: // Fenetre passage UP
		mcpOUTPUTstate[9] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[9], state);
		break;
	case 14: // Fenetre passage DOWN
		mcpOUTPUTstate[10] = state;
		//mcp.digitalWrite(mcpOUTPUTstate[10], state);
		break;
	default:
		break;
	}
}

inline void MCP_23017::activationDesSortie() {
	//mcpOUTPUTstate
	//	LASTEmcpOUTPUTstate
	static unsigned long previousMillis = 0;
	const int interval = 10; // interval to send DATA 

	if (millis() - previousMillis >= interval) {
		previousMillis = millis();
		//Serial.println("Envoye des donne au MCP!");

		//GPA
		mcp.digitalWrite(GPA0, !mcpOUTPUTstate[11]); // Claxon
		//Serial.println(!mcpOUTPUTstate[1]);
		mcp.digitalWrite(GPA1, !mcpOUTPUTstate[0]);	// Clignotant gauche
		mcp.digitalWrite(GPA2, !mcpOUTPUTstate[1]);	// clignotant droit
		mcp.digitalWrite(GPA3, !mcpOUTPUTstate[4]);	// Feux de position
		mcp.digitalWrite(GPA4, !mcpOUTPUTstate[5]);  // Brouillard AV
		mcp.digitalWrite(GPA5, !mcpOUTPUTstate[6]);	// Brouillard AR
		mcp.digitalWrite(GPA6, !mcpOUTPUTstate[3]);	//	Feux de croisement
		mcp.digitalWrite(GPA7, !mcpOUTPUTstate[2]);	// Feux de route
		//GPB
		mcp.digitalWrite(GPB3, !mcpOUTPUTstate[7]);	//Fenetre conducteur UP
		mcp.digitalWrite(GPB4, !mcpOUTPUTstate[8]);	// Fenete conducteur DOWN
		mcp.digitalWrite(GPB5, !mcpOUTPUTstate[9]);	// Fenetre passage UP
		mcp.digitalWrite(GPB6, !mcpOUTPUTstate[10]);	// Fenetre passage DOWN
	
	}

}
#endif // !peugeot_MCP23017_H
