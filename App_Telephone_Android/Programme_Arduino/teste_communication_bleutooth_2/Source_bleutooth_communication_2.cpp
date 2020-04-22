#include "Arduino.h"
#include"Header_bleutooth_communication_2.h"

//*********DEBUG*********
// Affiche les donne qui arrive dans le moniteur de serie
#define DEBUG_DATA_RECEIVE_DISPLAY 0
// Affiche si la tram commence par le bon caracte et fini par le bon aussi dans le moniteur serie
#define DEBUG_AFFICHE_SI_TRAM_COMPLET 0 
//*********DEBUG*********

//*********PARAMETRE*********
 // le caractere de debut de la tram
#define caracterStartData 13
// le caractere de fin de tram
#define caracterEndData 10 
// taille maximal des donne recus A REGLER ne pas oubliller que ca commne a ZERO !!! donc 22 - 1 = 21
#define lengthDATAreceive 21 //21
//*********PARAMETRE*********

byte arrayDataInput[25] = { 0 }; // le tableau qui va stoquer les donne recus
byte btMessageLength = 0; // longeur de la tram recus
bool etape = 0;


void setup_bleutooth_communication() {

	Serial3.begin(9600);
	Serial.println("Bleutooth initialise");
}


/*
  1 - Réception des données
  SerialEvent est déclenchée quand de nouvelles données sont reçues.
  Cette routine tourne entre chaque loop(), donc utiliser un
  delay la fait aussi attendre.
 */

 //-----------------------------------------------------------Received DATA-----------------------------------
void serialEvent3() {

		if (Serial3.available() == (lengthDATAreceive +1) ) // si 6 octe disponible dans le buffer
		{
			#if DEBUG_DATA_RECEIVE_DISPLAY 
			Serial.println("-----------"); //	Serial.print("Serial3 available : ");  Serial.println(Serial3.available());
			#endif // DEBUG_DATA_RECEIVE_DISPLAY
			etape = 1; // go to etape 1
		}


		if (etape == 1) {
			while (Serial3.available()) {

				#if DEBUG_DATA_RECEIVE_DISPLAY 
				Serial.print("btMessageLength : "); Serial.print(btMessageLength);
				#endif // DEBUG_DATA_RECEIVE_DISPLAY

				arrayDataInput[btMessageLength] = Serial3.read(); // lire les donne et les stoquer dans le tableau

				#if DEBUG_DATA_RECEIVE_DISPLAY
				Serial.print("\t arrayDataInput <= value received : "); Serial.println(arrayDataInput[btMessageLength]);
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
			afficherSiDebutEtFinTramCorrect();
			
			btMessageLength = 0;
			etape = 0;
		}

}

void dataOk() { // affiche les donne stoque dans le tableau

	if (arrayDataInput[0] == caracterStartData && arrayDataInput[lengthDATAreceive] == caracterEndData) {
		Serial.println("DATA COMPLET");
		//afficherDonneRecus();

		for (size_t i = 0; i < (lengthDATAreceive+1); i++)
		{
			char affiche = arrayDataInput[i]; // convertir un DEC en ACCI
			Serial.println(affiche);
		}

		sendDataWithBleutooth(); // envoyer les donne au telephone

		//Serial.println(arrayDataInput[0]);
		//Serial.println(arrayDataInput[1]);
		//Serial.println(arrayDataInput[2]);
		//Serial.println(arrayDataInput[3]);
		//Serial.println(arrayDataInput[4]);
		//Serial.println(arrayDataInput[5]);
	}
}

void afficherSiDebutEtFinTramCorrect() { // Screen information

#if DEBUG_AFFICHE_SI_TRAM_COMPLET
	if (arrayDataInput[0] == caracterStartData) {
		Serial.println("START caractere : TRUE");
	}
	else {
		Serial.println("START caractere : FALSE");
	}
	if (arrayDataInput[lengthDATAreceive] == caracterEndData) {
		Serial.println("END caractere : TRUE");
	}
	else {
		Serial.println("END caractere : FALSE");
	}
#endif // DEBUG_AFFICHE_SI_TRAM_COMPLET
}
//-----------------------------------------------------------Received DATA-----------------------------------



//-----------------------------------------------------------Send DATA-----------------------------------
void sendDataWithBleutooth() {

	byte dataArray[20] = { 0 };
	static byte a = 0;
	//uint8_t dataArray[10];

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

	dataArray[6] = 0; 
	dataArray[7] = 0; 
	dataArray[8] = 0;
	dataArray[9] = 0; 
	dataArray[10] = 0;
	dataArray[11] = 0;
	dataArray[12] = 0;
	dataArray[13] = 0;
	dataArray[14] = 1;
	dataArray[15] = 0;
	dataArray[16] = 0;
	dataArray[17] = 0;
	dataArray[18] = 0; 
	dataArray[19] = '\r'; //'\r'retour chario
	//dataArray[10] = 11; //'\n'

	Serial3.write(dataArray, 20);
	//Serial.write(dataArray, 20);

//	delay(100);


}

//-----------------------------------------------------------Send DATA-----------------------------------