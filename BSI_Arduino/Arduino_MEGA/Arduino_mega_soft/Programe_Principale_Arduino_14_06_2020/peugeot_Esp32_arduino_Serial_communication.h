#pragma once

#include <Arduino.h>
#include "BigNumber.h"
#include "peugeot_config.h"


#define debugAfficherNombreOcteRecus 0
#define debugAfficheTramVerification 0
#define debugAfficherViderBuffer 0
#define debugAfficherDonneIDEN1 0
#define debugAfficherDonneIDEN2 0
// programme pour le esp32 M:\arduino_esp32_406_programe_v1.0\v1.1_26_04_2020\ESP32_arduino_communication_406

class Esp32_arduino_serial_communication
{
  public:
    Esp32_arduino_serial_communication();
    ~Esp32_arduino_serial_communication();
    void recoitDonne();
    void lectureDesOcteDuBuffer(byte receivedData[], byte tailleTableau);
    bool verificationDonneDebutFin(byte receivedData[], byte tailleTableau);
    void viderLeBufferSerie();
    void affichageDonneRecus(byte receivedData[]);

  private:

};

Esp32_arduino_serial_communication::Esp32_arduino_serial_communication()
{
  Serial3.begin(115200); // communicaion avec ESP32 74880

  Serial.println(F("=> ESP32 ARDUINO COMMUNICATION SERIAL <="));
}

Esp32_arduino_serial_communication::~Esp32_arduino_serial_communication()
{
}

//******************************************************************************************************************************************
void Esp32_arduino_serial_communication::recoitDonne() { // attend d'avoir assez de donne dans le buffer serie

  const byte tailleTableau = 23; // taille du tableau qui stoque les donne
  byte receivedData[tailleTableau]; // le tableau qui stoque les donne lus du buffer serie
  byte availableBytes; // stoque le nombre d'octe disponible dans le buffer

  availableBytes = Serial3.available(); // regarde le nombre d'octe disponible dans le buffer
  
 // Serial.print("availableBytes = "); Serial.println(availableBytes);

  if (availableBytes == tailleTableau) {

#if debugAfficherNombreOcteRecus
    Serial.print(F("nombre octe buffer recus = ")); Serial.println(tailleTableau);
#endif // debugAfficherNombreOcteRecus


    lectureDesOcteDuBuffer(receivedData, tailleTableau); // appeler la fonction et lui envoyer le nombre d'octe max que on doit recevoir

  }

  if (availableBytes > 50) // vider le buffer si ca depasse la taille
  {
      viderLeBufferSerie();
  }

}
//******************************************************************************************************************************************
void Esp32_arduino_serial_communication::lectureDesOcteDuBuffer(byte receivedData[], byte tailleTableau) { // lit les donne du buffer serie

#if debugAfficherNombreOcteRecus
  Serial.println(F("=> lectureDesOcteDuBuffer"));
#endif // debugAfficherNombreOcteRecus


  for (byte n = 0; n < tailleTableau; n++) // on lit les donne stoquer dans le buffer et on les stoque dans un tableau
  {
    receivedData[n] = Serial3.read(); // lit et stoque les donne dans un tableau qui se acremonte

    // Serial.print("Lecture des donne du buffer : "); Serial.println(n);
  }
  verificationDonneDebutFin(receivedData, tailleTableau); // appeler la fonction qui verifier les donne
}
//******************************************************************************************************************************************
//******************************************************************************************************************************************


bool Esp32_arduino_serial_communication::verificationDonneDebutFin(byte receivedData[], byte tailleTableau) { // verifie si les donne arrive sont bien

  /* DATA1 ; DATA2 ; DATA3 ....... ; DATA 10 ; DATA 11
    Verif debut tram ; IDEN ; Donne ....... ; Verif fin de tram 1 ; Verif fin de tram 2
  		0         ;      ; Donne ....... ;        13           ;       10
  */

#define debug1 0

  static byte caractereDebutTram = 0; // caractere a verifier pour le debut de la tram
  static byte caractereFinTram1 = 13; // caractere 1 a verifier fin de lma tram
  static byte caractereFinTram2 = 10; // caractere 2 a verifier fin de lma tram
  bool verifDonneCorrect = 0; // la fonction re envoye si les donne recus sont correct


#if debug1 == 1 // mode debugage
  Serial.print("Tram debut = ");
  Serial.println(receivedData[0]);
  Serial.print("Tram fin 1 = ");
  Serial.println(receivedData[tailleTableau - 2]);
  Serial.print("Tram fin 2 = ");
  Serial.println(receivedData[tailleTableau - 1]);
#endif


  if (caractereDebutTram == receivedData[0]) {

#if debugAfficheTramVerification
    Serial.println(F("DEBUT DE TRAM = BON"));
#endif // debugAfficheTramVerification


    if (caractereFinTram1 == receivedData[tailleTableau - 2] && caractereFinTram2 == receivedData[tailleTableau - 1]) {

#if debugAfficheTramVerification
      Serial.println(F("FIN DE TRAM = BON"));
#endif // debugAfficheTramVerification

      verifDonneCorrect = 1;
     affichageDonneRecus(receivedData); // appeler la fonction en lui envoyer en parametre le tableau des donne serie stoque
    }
    else {
#if debugAfficheTramVerification
      Serial.println(F("FIN DE TRAM = FAUX"));
#endif // debugAfficheTramVerification

      verifDonneCorrect = 0;
      viderLeBufferSerie(); // aller a la fonction vider buffer
    }
  }
  else {
#if debugAfficheTramVerification
    Serial.println(F("DEBUT DE TRAM = FAUX"));
#endif // debugAfficheTramVerification

    verifDonneCorrect = 0;
    viderLeBufferSerie(); // aller a la fonction vider buffer
  }


  return verifDonneCorrect;
}
//******************************************************************************************************************************************

void Esp32_arduino_serial_communication::viderLeBufferSerie() { // vider les buffer si le debut et fin de tram sont pas bonne

#if debugAfficherViderBuffer
  Serial.println(F("=> viderLeBufferSerie"));
#endif // debugAfficherViderBuffer

  while (Serial3.available()) {
    byte inChar = Serial3.read(); // technique pour vider le buffer
  }
  recoitDonne(); // aller a la fonction
}

//******************************************************************************************************************************************

void Esp32_arduino_serial_communication::affichageDonneRecus(byte receivedData[]) { // affiche les donne arrive

 // byte dataIdentifier = 0; // quelle genre de donne sont disponible exemple dataIdentifier = 0 = donne moteur , dataIdentifier = 1 = donne climatisation


 // dataIdentifier = receivedData[1]; // lit quelle iden doit etre utilise

  if (receivedData[1] == 1) // si les donne recus commence par un 1
  {
#if debugAfficherDonneIDEN1
    Serial.println(F("***********IDEN data 1***********"));
#endif // debugAfficherDonneIDEN1
	

    //iden_824
    BigNumber rebuiltBigNumberRPM;
    rebuiltBigNumberRPM.bytes[0] = receivedData[2];
    rebuiltBigNumberRPM.bytes[1] = receivedData[3];
    rpmDecoded = rebuiltBigNumberRPM.value;
	
    speedDecoded = receivedData[4];
    //iden_4CF
    etatClignotant = receivedData[5];
    //iden_4DC
    temperatureInterneRecalcule = receivedData[6];
    etatDeLaClim = receivedData[7];
    //iden_8A4
    BigNumber rebuiltbigNumberkmTotal;
    rebuiltbigNumberkmTotal.bytes[0] = receivedData[8];
    rebuiltbigNumberkmTotal.bytes[1] = receivedData[9];
    rebuiltbigNumberkmTotal.bytes[2] = receivedData[10];
    rebuiltbigNumberkmTotal.bytes[3] = receivedData[11];
    kmTotal = rebuiltbigNumberkmTotal.value;
    kmTotalRecalculer = kmTotal / 10;

    tempExterieurRecalcule = receivedData[12];
    tempsEauRecalculer = receivedData[13];
    etatDuNeiman = receivedData[14];
    modeEconomiEnergie = receivedData[15];
    //iden_524
    etatVoiture = receivedData[16];
    etatFreinAmain = receivedData[17];
    essueGlaceAuto = receivedData[18];
    //iden_564
    etatDesPorte = receivedData[19];
	
#if debugAfficherDonneIDEN1



    //iden_824
    Serial.print(F("RPM: "));
    Serial.println(rebuiltBigNumberRPM.value);

    Serial.print(F("SPEED: "));
    Serial.println(speedDecoded, DEC);
    //iden_4CF
    Serial.print(F("Etat clignotant: "));
    Serial.println(etatClignotant, DEC);
    //iden_4DC
    Serial.print(F("Temperature Evopovarateur clim : "));
    Serial.println(temperatureInterneRecalcule, DEC);
    Serial.print(F("Etat de la climatisation: "));
    Serial.println(etatDeLaClim, DEC);
    //iden_8A4
    Serial.print(F("KM total vehicule: "));
    Serial.println(kmTotalRecalculer);
    Serial.print(F("Temperature exterieur: "));
    Serial.println(tempExterieurRecalcule, DEC);
    Serial.print(F("Temperature eau moteur: "));
    Serial.println(tempsEauRecalculer, DEC);
    Serial.print(F("Position de Neiman: "));
    Serial.println(etatDuNeiman, DEC);
    Serial.print(F("Economie energie : "));
    Serial.println(modeEconomiEnergie, DEC);
    //iden_524
    Serial.print(F("Voiture verouille ou pas: "));
    Serial.println(etatVoiture, DEC);
    Serial.print(F("Etat du frein a main: "));
    Serial.println(etatFreinAmain, DEC);
    Serial.print(F("Essue glace auto: "));
    Serial.println(essueGlaceAuto, DEC);
    //iden_564
    Serial.print(F("Etat des porte vehicule: "));
    Serial.println(etatDesPorte, DEC);

    Serial.print("\n");
    //  receivedData[0] = 0;
#endif // debugAfficherDonneIDEN1

  }
  else if (receivedData[1] == 2) // si les donne recus commence par un 2
  {
#if debugAfficherDonneIDEN2
    Serial.println(F("***********IDEN data 2***********"));
#endif // debugAfficherDonneIDEN2

    //  receivedData[0] = 0;
  }
  //Si iden recus nest pas dans la liste
  else {
   //Serial.print(F("Autre DATA iden recus!!! : "));
   Serial.println(receivedData[1]);
  }
}
