#pragma once

#include <Arduino.h>
#include "peugeot_delay.h" // Gestion millis timer
#include "TEST_peugeot_delay.h" // TEST!!!!!!

#define OPTION_1 1 //Camera de recul
#define OPTION_2 1 //Phare automatique
#define OPTION_3 0 //Eclerage d'ambiance d'interieur
#define OPTION_4 1 //Communication avec ESP32 
#define OPTION_5 1 //Communication avec module Bleutooth
#define OPTION_13 0 //Communication avec le BSI via prise OBD 
#define OPTION_6 1 //Communication avec le MCP23017 module controle relais
#define OPTION_7 1 //Ecran LCD 16X2 aide a la conduite
#define OPTION_8 1 //Genstion entre sortie
#define OPTION_9 1 //Allumages des phares automatiquement moteur en marche
#define OPTION_10 1 //Allumages des phares lorquil fait nuit et la voiture a etait ouvert ou une porte
#define OPTION_11 1 //Claxoné quand la voiture a etait ouvertou lorsequelle a etait ferme
#define OPTION_12 0 //Verouiller les porte quand la vitesse et superieur a 30km/h



#ifndef peugeot_config_h
#define peugeot_config_h

int LED_PIN_13 = 13;

//----------------------------------------------------mcp23017 OUTPUT ARRAY---------------
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
const byte mcpOUTPUTstateTAILLE = 15; // taille des tableau 
boolean mcpOUTPUTstate[mcpOUTPUTstateTAILLE] = { 0 }; // tableau qui stoque les etat de tout les sortie du MCP23017
boolean LASTEmcpOUTPUTstate[mcpOUTPUTstateTAILLE] = { 0 }; // tabeau qui stoque l'etat avant de tout les sortie du MCP23017
boolean unEtatOUTPUTacHANGE = 0; //unEtatOUTPUTacHANGE = !unEtatOUTPUTacHANGE // utiliser ca dire au program si une valeur de sortie a change de relais
//----------------------------------------------------mcp23017 OUTPUT ARRAY---------------
//------------------------------------------Photo Resistance----------------------------------------------
/*
valeur envoyer par la fonction phareAutoAllumage()
valeur 0 = il fait jours
valeur 1 = il fait nuit
valeur 2 = probleme de capteur de lumunosite ou de reglage defaut
*/
byte luxEtatLumunosite = 2; // variable qui contien dans quelle etat doive se trouver les feux d'apres la lumunosite exterieur (defaut 2)
//------------------------------------------Photo Resistance----------------------------------------------
//----------------------------------------INPUT OUTPUT------------------------------------------------
//INPUT OUTPUT
boolean contactMoteurBasicPin = 0; // neiman position contact moteur
boolean marcheArriereBasicPin = 0; // contact feux arriere detection marche arrier
boolean accesoireBasicPin = 0; // neiman position contact accesoir
boolean moteurEnMarchePin = 0; // moteur en marche
boolean busVanAlimenterPin = 0; // si le bus van et alimenter (BSI reveille)

boolean cameraDeReculEtat = 0; // variable qui controle l'activation ou desactivation de la camera de recul

//----------------------------------------INPUT OUTPUT------------------------------------------------
//-------------------------------------------ESP 32 ARDUINO communication---------------------------------------------

	// tout les variable qui stoque les donne recus sur le port serie
	//iden_824
uint16_t rpmDecoded;
uint16_t speedDecoded;
//iden_4CF
uint8_t etatClignotant;
//iden_4DC
uint8_t temperatureInterneRecalcule;
uint8_t etatDeLaClim;
//iden_8A4
uint32_t kmTotal;
uint32_t kmTotalRecalculer;
uint8_t tempExterieurRecalcule;
uint8_t tempsEauRecalculer;
uint8_t etatDuNeiman;
uint8_t modeEconomiEnergie;
//iden_524
uint8_t etatVoiture;
uint8_t etatFreinAmain;
uint8_t essueGlaceAuto;
//iden_564
uint8_t etatDesPorte;

// tout les variable qui stoque les donne recus sur le port serie

uint8_t etatDesPorteAVANT; // stoque l'etat avant pour voir le changement d'etat
uint8_t etatVoitureAVANT; // stoque l'etat avant pour voir le changement d'etat
//-------------------------------------------ESP 32 ARDUINO communication---------------------------------------------

uint8_t telephoneConnecteBLEUTOOTH = 0; // variable qui dit si un telephne et connecte en bleutoot<

/*
//********************************************************Variable GLOBAL pour les sortie*************************************************************************
boolean stateOUTPUTClignotantGauche = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTClignotantDroit = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTFeuxDeRoute = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTFeuxDeCroisement = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTFeuxDePosition = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTBrouillardAV = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTBrouillardAR = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTFenetreConducteurMonte = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTFenetreConducteurDecente = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTFenetrePassagerMonte = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTFenetrePassagerDecente = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTClaxon = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTOuvertureFermetureVoiture = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTSuperCondanation = 0; //0 = eteint; 1 = allume
boolean stateOUTPUTFermerOuvrirToutLesFenetre = 0; //0 = eteint; 1 = allume

//*******************************************L'encian etat de la sortie***************************
boolean LASTEstateOUTPUTClignotantGauche = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTClignotantDroit = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTFeuxDeRoute = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTFeuxDeCroisement = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTFeuxDePosition = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTBrouillardAV = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTBrouillardAR = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTFenetreConducteurMonte = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTFenetreConducteurDecente = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTFenetrePassagerMonte = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTFenetrePassagerDecente = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTClaxon = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTOuvertureFermetureVoiture = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTSuperCondanation = 0; //0 = eteint; 1 = allume
boolean LASTEstateOUTPUTFermerOuvrirToutLesFenetre = 0; //0 = eteint; 1 = allume
//*******************************************L'encian etat de la sortie***************************
*/




//********************************************************Variable GLOBAL pour les sortie*************************************************************************
#endif // confi_h

#if OPTION_6
#include "peugeot_MCP23017.h"
MCP_23017* mcp_23017;
#endif // OPTION_6


#if OPTION_5
#include "peugeot_HC_05.h"
HC_05* hc05; // pointeur vers une instance de classe (si on utilise un pointeur void devez utilise -> a la place)
#endif // OPTION_5

#if OPTION_2
#include "peugeot_Automatic_Headlight.h"
Automatic_Headlight* automatic_headlight;
#endif // OPTION_2

#if OPTION_8
#include "peugeot_InOutArduinoBasic.h"
InOutArduinoPin* in_out_arduino_pin;
#endif // OPTION_8

#if OPTION_4
#include "peugeot_Esp32_arduino_Serial_communication.h"
Esp32_arduino_serial_communication* esp32_arduino_serial_communication;
#endif // OPTION_4

#if OPTION_7
#include "peugeot_screen_LCD_i2c_16x2.h"
Screen_LCD_i2c_16x2* screen_lcd_i2c_16x2;
#endif // OPTION_7