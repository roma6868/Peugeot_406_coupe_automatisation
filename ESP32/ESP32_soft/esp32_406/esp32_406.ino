/*
  Name:		esp32_406.ino
  Created:	14/10/2019 15:44:59
  Author:	ROMAN
*/
// modification pour la bonne envoye des donne sur le port serie fait pere avec
//M:\arduino_esp32_406_programe_v1.0\v1.1_26_04_2020\communicaion_esp32_arduino_mega

#include "esp32_arduino_rmt_van_rx.h"
#include "esp32_rmt_van_rx.h"
#include "BigNumber.h" // partie devElopPer par PETER PINTER pour envoyer des valeur grande port serie 

/*Autoriser quelle iden va etre lue ou plusieur iden: 0 pas lue 1 lue  */
#define iden824 1 // vitesse vehicule ; regime moteur 
#define iden4FC 1 //clignotant etat
#define iden4DC 0 //temperature interne habitacle ; etat de la clim ON ou OFF
#define iden664 0
#define iden8A4 1 //etat du neiman ; temperature exterieur ; temp eau ; km  total du vehicule
#define iden524 1 //voiture ferme ouvert ; frein a main ; essue glacer mode auto
#define iden564 1 //etat des porte ;
#define iden4EC 0
#define iden5E4 0
#define iden4D4 0
#define idenE24 1 //VIN voitrure permet aussi de afficher tout les valeur des iden que jai besoin
#define lecturToutIden 0 // autoriser la lecture en mode normal de tout les IDEN qui passe

#define affichierReulatatMoniteurSerie824 0 // 0 rien afficher sur le monituer 1 afficher sur le moniteur
#define affichierReulatatMoniteurSerie4FC 0 // 0 rien afficher sur le monituer 1 afficher sur le moniteur
#define affichierReulatatMoniteurSerie4DC 0 // 0 rien afficher sur le monituer 1 afficher sur le moniteur
#define affichierReulatatMoniteurSerie8A4 0 // 0 rien afficher sur le monituer 1 afficher sur le moniteur

#define afficherlesDonnePortSeriePC 1 /// 0 rien afficher sur le monituer 1 afficher sur le moniteur

//**********************************Lecture normal du bus*************************************************
ESP32_RMT_VAN_RX VAN_RX;

const uint8_t VAN_DATA_RX_RMT_CHANNEL = 0;
const uint8_t VAN_DATA_RX_PIN = 4; // pin de reception du esp32
const uint8_t VAN_DATA_RX_LED_INDICATOR_PIN = 2; // led etat lor que une tram envoyer par la voiture et lue

uint8_t vanMessageLength;
uint8_t vanMessage[34]; // defaut 34
//uint8_t vanMessage[12] = { 0x0E, 0x82, 0x48, 0x17, 0x23, 0x00, 0x00, 0xA4, 0x12, 0x00, 0x6F, 0xA6 };

uint32_t lastMillis = 0;
//**********************************Lecture normal du bus*************************************************

//--------------------------------------------------------------------------------------------------------------------------------

//iden_824//iden_824//iden_824//iden_824//iden_824//iden_824//iden_824//iden_824//iden_824
//
uint16_t rpmDecoded = 0; // variable qui stoque les 2 valeur hex combine pour le regime moteur
uint16_t speedDecoded = 0; // variable qui stoque les 2 valeur hex combine pour la vitesse voiture

//iden_824//iden_824//iden_824//iden_824//iden_824//iden_824//iden_824//iden_824

//--------------------------------------------------------------------------------------------------------------------------------

//iden_4CF//iden_4CF//iden_4CF//iden_4CF//iden_4CF//iden_4CF//iden_4CF//iden_4CF

uint8_t etatClignotant; //afficher letat desclignotant (1 clignotant droit) (2 clignotant gauche) (3 warning) (0 rien )


//*******************************************permet de lire que 4 bit dans 1 octe*************************
typedef struct {
  uint8_t upperHalf2 : 4;  // bit 0-4
  // uint8_t bit0   : 1; // pour lire chaque bit il fautr decocher les bit0 bit1 bit2 bit3
  //uint8_t bit1   : 1; // pour lire chaque bit dans un octe 1011 1001
  //uint8_t bit2   : 1;
  //uint8_t bit3   : 1;
  uint8_t upperHalf : 4;  // bit 3-7
} Byte0Struct;

typedef union UnionWithStruct {
  Byte0Struct Byte0;
  uint8_t data[sizeof(Byte0Struct)];
};
//*******************************************permet de lire que 4 bit dans 1 octe*************************



//iden_4CF//iden_4CF//iden_4CF//iden_4CF//iden_4CF//iden_4CF//iden_4CF//iden_4CF

//--------------------------------------------------------------------------------------------------------------------------------

//iden_4DC//iden_4DC//iden_4DC//iden_4DC//iden_4DC//iden_4DC//iden_4DC//iden_4DC

uint8_t temperatureInterneEvoporateur = 0; // valeur brut de la temperature
uint8_t temperatureInterneEvoporateurRecalcule; // temperature reel une fois re calculer
uint8_t etatDeLaClim; // 1 = climatisation on ; 0 = climatisation OFF

//iden_4DC//iden_4DC//iden_4DC//iden_4DC//iden_4DC//iden_4DC//iden_4DC//iden_4DC
//--------------------------------------------------------------------------------------------------------------------------------
//iden_8A4//iden_8A4//iden_8A4//iden_8A4//iden_8A4//iden_8A4//iden_8A4//iden_8A4

uint8_t tempExterieur = 0; // valeur brut dela tempExterieur
uint32_t kmTotal; // variable qui stoc le km total du vehicule
uint8_t tempEau = 0; // valeur brut de la temperature eau
uint8_t tempExterieurRecalcule; // temperature reel une fois re calculer
uint8_t tempsEauRecalculer; // temeprature eau une fois recalculer

uint8_t etatDuNeiman; // 0 = contact coupe ; 1 = contact accesoir ; 2 = demmarage moteur ; 3 = contact moteur ; 4 = ???? ; 5 = moteur coupe ; 7 = moteur en marche
uint8_t modeEconomiEnergie; // 0 = mode economie energie OFF ; 1 = mode economie energie ON


//iden_8A4//iden_8A4//iden_8A4//iden_8A4//iden_8A4//iden_8A4//iden_8A4//iden_8A4
//--------------------------------------------------------------------------------------------------------------------------------
//iden_524//iden_524//iden_524//iden_524//iden_524//iden_524//iden_524//iden_524
uint8_t etatVoiture; // 0 = voiture ouvert ; 1 = voiture fermer
uint8_t etatFreinAmain; // 0 = freina main OFF ; 1 = frein a main ON
uint8_t essueGlaceAuto; // 0 = mode auto OFF ; 1 = mode auto ON

//iden_524//iden_524//iden_524//iden_524//iden_524//iden_524//iden_524//iden_524
//--------------------------------------------------------------------------------------------------------------------------------
//iden_564//iden_564//iden_564//iden_564//iden_564//iden_564//iden_564//iden_564

uint8_t etatDesPorte; // 0 = porte ferme conducteur/passager ; 1 = porte ouvert cote conducteur ; 2 = porte ouvert cote passager ; 3 = porte ouver conducteur et passager

//iden_564//iden_564//iden_564//iden_564//iden_564//iden_564//iden_564//iden_564
//--------------------------------------------------------------------------------------------------------------------------------
//send data thive arduino mega port con 2
byte dataArrayArduinoMega[23];
uint32_t tempoEnvoyeDonne; // millis
uint8_t autorisationEnvoyeDonne;

//send data thive arduino mega port con 2
//--------------------------------------------------------------------------------------------------------------------------------

void setup() {
  //***********************initialisation la communication ESP32 > PC**************************

  Serial.begin(500000); // vitesse de communicatyion avec le PC 500000
  Serial2.begin(115200); // viteesse de communication avec arduino MEGA 74880
  printf("ESP32 Arduino VAN bus monitor\n");
  VAN_RX.Init(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN); //initialiser la communication avec la voiture

  //***********************initialisation la communication ESP32 > PC**************************
  
}


void loop() {

	//-----------------------------------------------------------------------------------------
	//autorisationEnvoyeDonne = 1; // force lenvoyer des donne a arduino mega pour les essais 
	//kmTotal = tempoEnvoyeDonne; // pour les essais envoyer le temps millis

	if (millis() - tempoEnvoyeDonne > 100) { // envoye des donne a arduino mega tout les x seconde
		tempoEnvoyeDonne = millis(); // recuperer la valeur d emillis apress 

		if (autorisationEnvoyeDonne == 1) {
			sentArduinoPortDATA(); // serial write sor arduino
			//Serial.println("autorisation envoye donne a arduino mega");
		}

		
	}
	//-----------------------------------------------------------------------------------------
	


  //*******************************Lecture de tout les IDEN*************************************
  if (lecturToutIden == 1) {
    if (millis() - lastMillis > 10)
    {
      lastMillis = millis();
      VAN_RX.Receive(&vanMessageLength, vanMessage);

      if (vanMessageLength > 0)
      {
        if (VAN_RX.IsCrcOk(vanMessage, vanMessageLength))
        {
          for (size_t i = 0; i < vanMessageLength; i++)
          {
            if (i != vanMessageLength - 1)
            {
              printf("%02X ", vanMessage[i]);
            }
            else
            {
              printf("%02X", vanMessage[i]);
            }
          }
          printf("\n");
        }
        else
        {
          printf("CRC ERROR!\n");
        }
      }
    }

  }
  //*******************************Lecture de tout les IDEN*************************************

  if (lecturToutIden == 0) {
    if (millis() - lastMillis > 10)
    {
      lastMillis = millis();
      VAN_RX.Receive(&vanMessageLength, vanMessage);

      if (vanMessageLength > 0)
      {
        if (VAN_RX.IsCrcOk(vanMessage, vanMessageLength))
        {
          choixIden(); // appeler la fonction choix des ide qui va regarde rquelle iden lire
        }
        else
        {
          printf("CRC ERROR!\n");
        }

        Serial.println("  ");

		
		autorisationEnvoyeDonne = 1; // autorisation de envoye des donne port serie arduino

		//delay(100);
		if (afficherlesDonnePortSeriePC == 1)
		{
			if (iden4FC == 1) {
				switch (etatClignotant) { // etat des clignotant
				case 2:
					Serial.println("Clignotant : Gauche");
					break;

				case 1:
					Serial.println("Clignotant : Droit");
					break;

				case 3:
					Serial.println("Clignotant : Warning");
					break;

				case 0:
					Serial.println("Clignotant : Rien");
					break;
				}//switch 
			}//iden4FC == 1

			if (iden824 == 1) {
				Serial.print("Regime moteur : ");
				Serial.println(rpmDecoded, DEC);

				Serial.print("Vitesse vehicule : ");
				Serial.println(speedDecoded, DEC);
			} //iden824 == 1

			if (iden4DC == 1) {
				Serial.print("Temperature Evopovarateur clim : ");
				Serial.println(temperatureInterneEvoporateurRecalcule, DEC);
				Serial.print("Etat de la climatiation : ");
				Serial.println(etatDeLaClim);
			}// iden4DC == 1

			if (iden8A4 == 1) {
				//&&&&&&&iden_8A4
				Serial.print("Temperature exterieur : ");
				Serial.println(tempExterieurRecalcule, DEC);
				Serial.print("Temperature Eau : ");
				Serial.println(tempsEauRecalculer, DEC);
				Serial.print("Distance total parcourue vehicule : ");
				Serial.println(kmTotal, DEC);

				switch (etatDuNeiman) { // posiiton du neiman
				case 0:
					Serial.println("Position neiman : Contact coupe");
					break;
				case 1:
					Serial.println("Position neiman : Contact accesoir");
					break;
				case 2:
					Serial.println("Position neiman : demmatage moteur"); // 2 = demmarage moteur
					break;
				case 3:
					Serial.println("Position neiman : Contact moteur"); // 3 = contact moteur
					break;
				case 4:
					Serial.println("Position neiman : ?????? valeur 4"); // 4 = ????
					break;
				case 5:
					Serial.println("Position neiman : moteur coupe"); //  5 = moteur coupe
					break;
				case 7:
					Serial.println("Position neiman : Moteur en marche"); // 7 = moteur en marche
					break;
				} //switch 



				switch (modeEconomiEnergie) { // mode economie energie on/off
				case 0:
					Serial.println("Mode economie energie : OFF"); // 0 = mode economie energie OFF
					break;
				case 1:
					Serial.println("Mode economie energie : ON"); // 1 = mode economie energie ON
					break;
				}//switch 
			}//den8A4 == 1

			//&&&&&&&
			if (iden524 == 1) {
				switch (etatVoiture) { // etat de la voiture
				case 0:
					Serial.println("Voiture : Deverouiller");
					break;
				case 1:
					Serial.println("Voiture  : Verouiller");
					break;
				}
				//&&&&&&
				switch (etatFreinAmain) { // etat du frein a main
				case 0:
					Serial.println("Frein a main : Enlever");
					break;
				case 1:
					Serial.println("Frein a main : Mis");
					break;
				}

				//&&&&&&
				switch (essueGlaceAuto) { // etat du frein a main
				case 0:
					Serial.println("Essuie glace AUTO : NON");
					break;
				case 1:
					Serial.println("Essuie glace AUTO : OUI");
					break;
				}//switch 
			} //iden524 == 1

			if (iden564 == 1) {
				//&&&&&&&iden_564
				switch (etatDesPorte) {
				case 0:
					Serial.println("Etat porte : conducteur passager FERMER");  // 0 = porte ferme conducteur/passager
					break;
				case 1:
					Serial.println("Etat porte : conducteur OUVERT"); // 1 = porte ouvert cote conducteur
					break;
				case 2:
					Serial.println("Etat porte : passager OUVERT"); // 2 = porte ouvert cote passager
					break;
				case 3:
					Serial.println("Etat porte : conducteur passager OUVERT"); // 3 = porte ouver conducteur et passager
					break;
				} //switch 
			}// iden564 == 1

		} // afficherlesDonnePortSeriePC
      }
    }
  }

} //****************************************void loop******************



// choix IDEN****************************************************************************
int choixIden() {

  if (iden824 == 1) {
    iden_824();
  }

  if (iden4FC == 1) {
    iden_4FC();

  }
  if (iden4DC == 1) {
    iden_4DC();

  }

  if (iden664 == 1) {
    iden_664();

  }

  if (iden8A4 == 1) {
    iden_8A4();

  }

  if (iden524 == 1) {
    iden_524();

  }

  if (iden564 == 1) {
    iden_564();

  }

  if (iden4EC == 1) {
    iden_4EC();

  }

  if (iden5E4 == 1) {
    iden_5E4();

  }
  if (iden4D4 == 1) {
    iden_4D4();

  }

  if (idenE24 == 1) {
    iden_E24();

  }




} // choix IDEN****************************************************************************


/*
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*/

int iden_824() {
  //Serial.println("iden_824");

  if (vanMessage[1] == 0x82 && vanMessage[2] == 0x48) //filtrer IDEN 824 0x82 0x48

  {

    // lit la valeur sur le bus en HEX et les assemble puis les convertir en DECIMAL pour lecture sur moniteur serie
    // combine the 2 rpm bytes together
    uint16_t rpmCombined = vanMessage[3] << 8 | vanMessage[4]; // la position ou sont stoquer les valeur pour le regime moteur

    // divide the value by 8 to get the actual rpm data
    rpmDecoded = rpmCombined / 8;

    // print rpm data
    if (affichierReulatatMoniteurSerie824 == 1) {
      Serial.print("rpm: ");
      Serial.println(rpmDecoded, DEC);
    }

    // combine the 2 speed bytes together
    uint16_t speedCombined = vanMessage[5] << 8 | vanMessage[6];

    // divide the value by 100 to get the actual speed data
    speedDecoded = speedCombined / 100;

    // print speed data
    if (affichierReulatatMoniteurSerie824 == 1) {
      Serial.print("speed: ");
      Serial.println(speedDecoded, DEC);
      printf("\n");
    }

    //printf("%02X", vanMessage[3]);
    //delay(1000);
    // lit la valeur sur le bus en HEX et les assemble puis les convertir en DECIMAL pour lecture sur moniteur serie


  }

}//****************************************iden_824()******************

/*
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*/

int iden_4FC() {
  //Serial.println("iden_4FC");

  if (vanMessage[1] == 0x4F && vanMessage[2] == 0xCC) //filtrer IDEN 0x4F 0xCC

  {
    // Serial.println(vanMessage[8], HEX);
    // printf("%02X" , vanMessage[8]);

    UnionWithStruct strEtatClignotant; // etat des clignotant
    strEtatClignotant.data[0] = vanMessage[8]; // recupere l'octe qui contien l'etat des clignotant

    //Serial.print("Upper half: ");
    // Serial.println(strEtatClignotant.Byte0.upperHalf, HEX); // afficher juste la valeur 8
    //Serial.print("Upper half2: ");
    //Serial.println(strEtatClignotant.Byte0.upperHalf2, HEX); // afficher juste la valeur 1

    if (strEtatClignotant.Byte0.upperHalf2 == 0x4) {

      if (affichierReulatatMoniteurSerie4FC == 1) {
        Serial.println("Clignotant gauche ");
      }

      etatClignotant = 2; //2 = clignotant gauche
    }

    if (strEtatClignotant.Byte0.upperHalf2 == 0x8) {

      if (affichierReulatatMoniteurSerie4FC == 1) {
        Serial.println("Clignotant Droit");
      }
      etatClignotant = 1; //1 = clingotant droit
    }

    if (strEtatClignotant.Byte0.upperHalf2 == 0xC) {
      if (affichierReulatatMoniteurSerie4FC == 1) {
        Serial.println("WARNING");
      }
      etatClignotant = 3; //3 = warning
    }
    if (strEtatClignotant.Byte0.upperHalf2 == 0x0) {
      if (affichierReulatatMoniteurSerie4FC == 1) {
        Serial.println("Rien");
      }
      etatClignotant = 0; //0 = rien nest active

    }

  }


}//****************************************iden_4FC()******************

/*
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*/


int iden_4DC() {
  //Serial.println("iden_4DC");

  if (vanMessage[1] == 0x4D && vanMessage[2] == 0xCC) //filtrer IDEN 824 0x82 0x48

  {

    UnionWithStruct strEtatClim; // etat de la climatisation
    strEtatClim.data[0] = vanMessage[3]; //lit les 4 bit dans un octe qont veut

    switch (strEtatClim.Byte0.upperHalf) {
      case 0x8:
        etatDeLaClim = 0; // climatisation OFF

        break;
      case 0xC:
        etatDeLaClim = 1; // climatisation ON
        break;

    }

    temperatureInterneEvoporateur = vanMessage[7]; // stoquer la valeur de la temperature

    temperatureInterneEvoporateurRecalcule = temperatureInterneEvoporateur / 10;// diviser pour trover la bonne valeur

    if (affichierReulatatMoniteurSerie4DC == 1) {

      Serial.println(vanMessage[7], HEX);
      printf("%02X", vanMessage[7]);
      Serial.println(temperatureInterneEvoporateurRecalcule, DEC); // afficher la temperature interne du habitacle
    }



  }

}//****************************************iden_4DC()******************
/*

*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*/

int iden_664() {
  Serial.println("iden_664");
}//****************************************iden_664()******************

int iden_8A4() {
  //Serial.println("iden_8A4");
  if (vanMessage[1] == 0x8A && vanMessage[2] == 0x48)  //filtrer IDEN 824 0x82 0x48

  {
    //Serial.println(vanMessage[8], HEX);
    //printf("%02X", vanMessage[8]);

    tempExterieur = vanMessage[9]; // recuperer valeur de la tram
    tempEau = vanMessage[5]; // recuperer valeur de la tram

	tempsEauRecalculer = tempEau - 39;
    tempExterieurRecalcule = (tempExterieur - 80) / 2; // calcule de la temperature
    //tempsOilRecalculer = tempOil / 2; // calcule de la temperature

	kmTotal = vanMessage[6] << 16 | vanMessage[7] << 8 | vanMessage[8]; // rassembler les bit

	

    UnionWithStruct strEtatNeiman;
    strEtatNeiman.data[0] = vanMessage[4];

    UnionWithStruct strEtatModeEco; // Mode economie energie ON OFF
    strEtatModeEco.data[0] = vanMessage[4]; //lit les 4 bit dans un octe qont veut


    switch (strEtatModeEco.Byte0.upperHalf) {
      case 0x0:
        modeEconomiEnergie = 0; // 0 = mode economie energie OFF

        break;
      case 0x1:
        modeEconomiEnergie = 1; // 1 = mode economie energie ON
        break;
    }
    //&&&&&&&&&&&&&&&&&&&&&&

    switch (strEtatNeiman.Byte0.upperHalf2) {
      case 0x0:
        etatDuNeiman = 0; //  0 = contact coupe
        break;
      case 0x1:
        etatDuNeiman = 1; // 1 = contact accesoir
        break;
      case 0x2:
        etatDuNeiman = 2; // 2 = demmarage moteur
        break;
      case 0x3:
        etatDuNeiman = 3; // 3 = contact moteur
        break;
      case 0x4:
        etatDuNeiman = 4; // 4 = ????
        break;
      case 0x5:
        etatDuNeiman = 5; //  5 = moteur coupe
        break;
      case 0x7:
        etatDuNeiman = 7; // 7 = moteur en marche
        break;

    }
    //&&&&&&&&&&&&&&&&&&&&&&


    if (affichierReulatatMoniteurSerie8A4 == 1) {
      Serial.print("Temperature exterieur : ");
      Serial.println((tempExterieur - 80) / 2, DEC);

      Serial.print("Temperature Eau : ");
      Serial.println(tempEau - 39, DEC);

	  Serial.print("Kilometrage vehicule : ");
	  Serial.println(kmTotal, DEC);
    }
  }

}//****************************************iden_8A4()******************

int iden_524() {
  //Serial.println("iden_524");


  if (vanMessage[1] == 0x52 && vanMessage[2] == 0x48) //filtrer IDEN 824 0x82 0x48

  {
    //&&&&&&&&&&&&&&&&&&&&&&
    UnionWithStruct strEtatVoiture; // etat de la voiture ferme / ouvert
    strEtatVoiture.data[0] = vanMessage[11]; //lit les 4 bit dans un octe qont veut


    switch (strEtatVoiture.Byte0.upperHalf2) {
      case 0x0:
        etatVoiture = 0; // voiture ouvert 0

        break;
      case 0x1:
        etatVoiture = 1; // voiture ferme 1
        break;
    }
    //&&&&&&&&&&&&&&&&&&&&&&

    UnionWithStruct strEtatFreinAmain; // etat du frein a main
    strEtatFreinAmain.data[0] = vanMessage[8]; //lit les 4 bit dans un octe qont veut


    switch (strEtatFreinAmain.Byte0.upperHalf2) {
      case 0x0:
        etatFreinAmain = 0; // 0 = freina main OFF

        break;
      case 0x1:
        etatFreinAmain = 1; // 1 = frein a main ON
        break;
    }
    //&&&&&&&&&&&&&&&&&&&&&&

    UnionWithStruct strEtatEssueGlace; // etat du frein a main
    strEtatEssueGlace.data[0] = vanMessage[11]; //lit les 4 bit dans un octe qont veut


    switch (strEtatEssueGlace.Byte0.upperHalf) {
      case 0x0:
        essueGlaceAuto = 0; // 0 = mode auto OFF

        break;
      case 0x2:
        essueGlaceAuto = 1; // 1 = mode auto ON
        break;
    }
    //&&&&&&&&&&&&&&&&&&&&&&

  }



}//****************************************iden_524()******************

int iden_564() {
  //Serial.println("iden_564");
  if (vanMessage[1] == 0x56 && vanMessage[2] == 0x4E) //filtrer IDEN 0x56 0x4E

  {
    //&&&&&&&&&&&&&&&&&&&&&&
    UnionWithStruct strEtatPort; // etat de la voiture ferme / ouvert
    strEtatPort.data[0] = vanMessage[10]; //lit les 4 bit dans un octe qont veut

   // Serial.print("Upper half2: ");
   // Serial.println(strEtatPort.Byte0.upperHalf, HEX); // afficher juste la valeur 8

    switch (strEtatPort.Byte0.upperHalf) {
      case 0x0:
        etatDesPorte = 0; // 0 = porte ferme conducteur/passager
        break;
      case 0x4:
        etatDesPorte = 1; // 1 = porte ouvert cote conducteur
        break;
      case 0x8:
        etatDesPorte = 2; // 2 = porte ouvert cote passager
        break;
      case 0xC:
        etatDesPorte = 3; // 3 = porte ouver conducteur et passager
        break;

    }
  }



}//****************************************iden_564()******************

int iden_4EC() {
  Serial.println("iden_4EC");
}//****************************************iden_4EC()******************

int iden_5E4() {
  Serial.println("iden_5E4");
}//****************************************iden_5E4()******************

int iden_4D4() {
  Serial.println("iden_4D4");
}//****************************************iden_4D4()******************

/*
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*/

int iden_E24() {
  //Serial.println("iden_E24");

  if (vanMessage[1] == 0xE2 && vanMessage[2] == 0x48) { //filtrer IDEN 0x4F 0xCC
    //Serial.println("Vin detecter");


  }

}//****************************************iden_E24()******************
/*
*******************************************************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************
*/


int sentArduinoPortDATA() // Envoyer les donne sur le port serie 2 en destination de arduino mega les donne sont envoyer tout les x seconde
{

	/*
	//Test pourenvoyerdes done sur le portserie ARDUINO
	//iden_824
	uint16_t rpmDecoded = 6891;
	uint16_t speedDecoded = 120;
	//iden_4CF
	uint8_t etatClignotant = 2;
	//iden_4DC
	uint8_t temperatureInterneRecalcule = 20;
	uint8_t etatDeLaClim = 1;
	//iden_8A4
	uint32_t kmTotal = 2601542;
	uint8_t tempExterieurRecalcule = 15;
	uint8_t tempsOilRecalculer = 15;
	uint8_t tempsEauRecalculer = 30;
	uint8_t etatDuNeiman = 2;
	uint8_t modeEconomiEnergie = 1;
	//iden_524
	uint8_t etatVoiture = 0;
	uint8_t etatFreinAmain = 1;
	uint8_t essueGlaceAuto = 2;
	//iden_564
	uint8_t etatDesPorte = 1;
	*/

	BigNumber bigNumberRPM;
	bigNumberRPM.value = rpmDecoded; // convertir le regime moteur en 2 bit separer carvaleur maxpossible envoyer sur un bit et 255

	BigNumber bigNumberkmTotal;
	bigNumberkmTotal.value = kmTotal; // convertir le rkm total du vehicule en 4 bit

	dataArrayArduinoMega[0] = 0; // 0 pour le debut de la tram
	dataArrayArduinoMega[1] = 1; // IDEN
	//dataArrayArduinoMega[3] = 15;
	//dataArrayArduinoMega[4] = 6;
	
	//iden_824
	dataArrayArduinoMega[2] = bigNumberRPM.bytes[0];
	dataArrayArduinoMega[3] = bigNumberRPM.bytes[1];
	dataArrayArduinoMega[4] = speedDecoded;
	//iden_4CF
	dataArrayArduinoMega[5] = etatClignotant;
	//iden_4DC
	dataArrayArduinoMega[6] = temperatureInterneEvoporateurRecalcule;
	dataArrayArduinoMega[7] = etatDeLaClim;
	//iden_8A4
	dataArrayArduinoMega[8] = bigNumberkmTotal.bytes[0];
	dataArrayArduinoMega[9] = bigNumberkmTotal.bytes[1];
	dataArrayArduinoMega[10] = bigNumberkmTotal.bytes[2];
	dataArrayArduinoMega[11] = bigNumberkmTotal.bytes[3];
	
	dataArrayArduinoMega[12] = tempExterieurRecalcule;
	dataArrayArduinoMega[13] = tempsEauRecalculer;
	dataArrayArduinoMega[14] = etatDuNeiman;
	dataArrayArduinoMega[15] = modeEconomiEnergie;
	//iden_524
	dataArrayArduinoMega[16] = etatVoiture;
	dataArrayArduinoMega[17] = etatFreinAmain;
	dataArrayArduinoMega[18] = essueGlaceAuto;
	//iden_564
	dataArrayArduinoMega[19] = etatDesPorte;
    dataArrayArduinoMega[20] = 1;
    dataArrayArduinoMega[21] = 13; // 13 pour la fin de tram 1
    dataArrayArduinoMega[22] = 10; // 10 pour la fin de tram 2

	Serial2.write(dataArrayArduinoMega, 23); // envoyer tout les bit !
	// Serial.print("Donne envoyer = ");
	//  Serial.println(dataArrayArduinoMega); // afficher les donne envoyer
	autorisationEnvoyeDonne = 0; // arreter l'autorisation de envoye des donne sur le port serie arduino
	//Serial.println("Donne envoyer a arduino mega");
}