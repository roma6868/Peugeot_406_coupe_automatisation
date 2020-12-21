/*
  Name:		Arduino_read_OBD_v1.ino
  Created:	14/05/2020 15:54:28
  Author:	roma6868
  Arduino UNO
  MCZ33290 Pin: RX 9 TX 8
  Button Connect Pin: 4
  Codeur retatif Pin: A 7 B 6
  Codeur retatif button Pin: 5
*/
#include <SoftwareSerial.h>
#define rx_PIN_OBD 9
#define tx_PIN_OBD 8
#define button1 4 // button pour demmarer l'initialisation

SoftwareSerial mySerialOBD(rx_PIN_OBD, tx_PIN_OBD); // RX, TX


void setup() {

  Serial.begin(115200, SERIAL_8N1); //PC
  mySerialOBD.begin(10400); //, SERIAL_8N1 OBD

  //*************************tx_PIN_OBD HIGH LEVEL****************
  pinMode(tx_PIN_OBD, OUTPUT);
  digitalWrite(tx_PIN_OBD, HIGH); // HIGH LEVEL
  //*************************tx_PIN_OBD HIGH LEVEL****************
  pinMode(button1, INPUT_PULLUP); // button connexion

  Serial.println(F("=>SETUP<="));
}


void loop() {

  static bool a;

  if (a == 1) // Initialisation is OK
  {
    menuIdentification();
  }
  else {
    a = initialisationPortOBD();
  }

  //pingBSI();
}

//*****************************************FONCTION INITIALISE COMMUNICATION*****************************************************************************
bool initialisationPortOBD() {
  byte tableauRecevoir[3] = { 0 }; // array received data

  byte tableauEnvoye[3] = { 0x70 };
  const byte initRepenseBSI[3] = { 0x55, 0xD0, 0x8F }; // reponse du BSI si l'initialisation c'est bien pass�

  static byte etape = 0; // defilement du programe etape par etape


  switch (etape)
  {
    case 0:
      if (buttonStateRead(button1) == 1) { // read change state buttom
        fiveBaudInit(); // send 5 baud init
        viderBufferReception(); // clear buffer
        etape = 1; // goto etape 1
      }

      break;
    case 1:
      byte a = readMySerialObdDATA(tableauRecevoir, 3);  // read data buffer

      if (a == 1) { // si la fonction returne un 1

        Serial.print("Etape 1 Answer : ");
        printData(tableauRecevoir, 3); // print data answer

        // teste si la reponse du BSI et correct
        if (memcmp(tableauRecevoir, initRepenseBSI, sizeof(tableauRecevoir)) == 0) { // http://www.cplusplus.com/reference/cstring/memcmp/

          Serial.println(F("Answer BSI OK"));
          etape = 2; // goto etape 2
        }
        else {
          Serial.println(F("Answer ECHEC (reponse incorect)"));
          etape = 0; // goto etape 0
        }
      }
      break;
    default:
      break;
  }
  //------------------------
  switch (etape)
  {
    case 2:
      sendMySerialObdDATA(tableauEnvoye, 1);
      viderBufferReception(); // clear buffer
      etape = 3; // goto etape 3
      break;
    case 3:

      byte a = readMySerialObdDATA(tableauRecevoir, 1); // read data buffer

      if (a == 1) {

        Serial.print("Etape 3 Answer : ");
        printData(tableauRecevoir, 1); // print data answer
        Serial.println("BSI Connection OK");
        etape = 0;
        return 1;
      }
      break;
    default:
      break;
  }
  return 0;
}
//******************************************************************************************************************************************************
//**********************************Fonction print DATA*******************************************************
bool printData(byte* pointeurTableauPrintln, byte tailleTableauPrintln) {

  char buffer[30];
  for (size_t i = 0; i < tailleTableauPrintln; i++)
  {
    sprintf(buffer, "%02x", pointeurTableauPrintln[i]); Serial.print(buffer);
    //Serial.print(pointeurTableauPrintln[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}


//***********************************************************************************************
//****************************************Fonction PING*******************************************************
bool pingBSI() {
  const byte tailleTableauPingOBDbsi = 5;
  static byte pingOBDbsi[tailleTableauPingOBDbsi] = { 0x81, 0xD0, 0xF1, 0x3E, 0x80 }; // envoye demande
  static byte pingOBDbsiRepense[5] = { 0x81, 0xD1, 0xD0, 0x7E, 0xC0 }; // reponse du BSI

  const int pingOBDbsiDELAY = 3000;
  static unsigned long previousMillisPing = 0;
  byte i = 0;

  if (millis() - previousMillisPing >= pingOBDbsiDELAY) { // delay pour envoye des byte pour le message

    previousMillisPing = millis();
    Serial.println(F("Ping send byte !"));
    sendMySerialObdDATA(pingOBDbsi, tailleTableauPingOBDbsi);

  }
}

//***********************************************************************************************
//********************************************VIDER LE BUFFER RECEPTION***************************************************
bool viderBufferReception() {

  while (mySerialOBD.available()) {
    char inChar = mySerialOBD.read();  // vider le buffer la l'emision se fait sur le meme pin que la reception
  }
}
//***********************************************************************************************
//****************************************FONCTION FIVE BAUD INIT*******************************************************
bool fiveBaudInit() {

  //************************************** 5 baud INIT start ****************************
  Serial.println("5 baud INIT start");
  Serial.print("-");
  digitalWrite(8, LOW);
  delay(1000);
  Serial.print("--");
  digitalWrite(8, HIGH);
  delay(200);
  Serial.print("---");
  digitalWrite(8, LOW);
  delay(200);
  Serial.println("----");
  digitalWrite(8, HIGH);
  Serial.println("5 baud INIT end");
  //************************************** 5 baud INIT start ****************************
  return 1;
}
//***********************************************************************************************
//*****************************************FONCTION BUTTON STATE READ****************************************************
bool buttonStateRead(byte pinButtonRead) {

  static byte button1State = 0;

  //demmarer l'initialisation lorque le button a etait appuier
  if (digitalRead(button1) == LOW && digitalRead(button1) != button1State) {
    Serial.println("Button appuier");
    button1State = digitalRead(button1);
    return 1;
  }
  return 0;
}
//***********************************************************************************************
//*****************************************FONCTION READ******************************************************
bool readMySerialObdDATA(byte *pointeurTableauRecevoir, byte tailleTableauRecevoir) {

  static int a = 0;


  while (mySerialOBD.available() > 0)
  {

    if (mySerialOBD.available() == tailleTableauRecevoir || a == 1) {

      a = 1;

      for (size_t i = 0; i < tailleTableauRecevoir; i++) // lire les donnes du buffer et les stoque dans le tableau
      {
        delay(10); // petit pause si non ca bug
        pointeurTableauRecevoir[i] = mySerialOBD.read();
        //	Serial.print("DEBUG readMySerialObdDATA : ");
        //Serial.println(pointeurTableauRecevoir[i], HEX);
      }
      /*
        Serial.print("Repence : ");
        Serial.print(pointeurTableauRecevoir[0], HEX);
        Serial.print(" ");
        Serial.print(pointeurTableauRecevoir[1], HEX);
        Serial.print(" ");
        Serial.print(pointeurTableauRecevoir[2], HEX);
      */
      a = 0;
      return 1;
    }
  }
  return 0;
}
//***********************************************************************************************
//*******************************************SONCTION SEND****************************************************
bool sendMySerialObdDATA(byte *pointeurTableauEnvoye, byte tailleTableauEnvoye) {

  static unsigned long previousMillisSendBit = 0; // pour millis
  const int pingOBDbsiDELAY = 6; // delais entre envois de chaque bit
  static byte i = 0;
  char buffer[30];

  Serial.print(F("Byte send : "));

  while (i < tailleTableauEnvoye)
  {
    if (millis() - previousMillisSendBit >= pingOBDbsiDELAY) { // delay pour envoye des bit pour le message

      previousMillisSendBit = millis();
      mySerialOBD.write(pointeurTableauEnvoye[i]);
      sprintf(buffer, "%02x", pointeurTableauEnvoye[i]); Serial.print(buffer);
      //printf("%02X", pointeurTableauEnvoye[i]);
      //Serial.print(pointeurTableauEnvoye[i], HEX);
      Serial.print(" ");
      i++;

      if (i == tailleTableauEnvoye) { // une fois tout les bit envoyer
        Serial.println("");
        i = 0;
        break;
      }
    }
  }
}
//***********************************************************************************************
//*********************************************FONCTION TIME OUT**************************************************
bool timeOUT(int delayTimeOut) {

  static unsigned long previousMillisTimeOut = 0; // pour millis
  int conteurTimeOut = 0;

  if (millis() - previousMillisTimeOut >= 1000) {

    conteurTimeOut++;
    previousMillisTimeOut = millis();
    Serial.print("Conteur : ");
    Serial.println(conteurTimeOut);
  }

  if (conteurTimeOut == delayTimeOut) {
    Serial.println("TIME OUT");
    conteurTimeOut = 0;
    return 1;
  }
  return 0;
}
//***********************************************************************************************

void sendCommandeBSI(byte numeroCommande) {

}

void commandeReadDATABSI(byte numeroCommande) {

}

//***********************************************************Menu identification***********************************************
void menuIdentification() {
  byte DataMenuIdentification[6] = { 0x82, 0xD0, 0xF1, 0x21, 0x81, 0xE5 }; // answer byte 14
  byte DataMenuIdentification1[6] = { 0x82, 0xD0, 0xF1, 0x21, 0x80, 0xE4 }; // answer byte 28
  byte readAnswerCommande[30] = { 0 };

  sendMySerialObdDATA(DataMenuIdentification, 6); // send data k-line
  //viderBufferReception();
  readMySerialObdDATA(readAnswerCommande, 14); // read data k-line
  Serial.println("");
  Serial.println("");

  Serial.print("Date of manufacture : ");
  Serial.print(readAnswerCommande[8], HEX);
  Serial.print("/");
  Serial.print(readAnswerCommande[9], HEX);
  Serial.print("/");
  Serial.print(readAnswerCommande[10], HEX);

  Serial.println("");

  Serial.print("Serial number : ");
  Serial.print(readAnswerCommande[5], HEX);
  Serial.print(readAnswerCommande[6], HEX);
  Serial.print(readAnswerCommande[7], HEX);


  Serial.println("");
  Serial.println("");
  Serial.print("Answer BSI : ");
  printData(readAnswerCommande, 14); // print data answer
  delay(1000);
}
//***********************************************************Menu identification***********************************************
