#include <Arduino.h>

#ifndef peugeot_delay_h
#define peugeot_delay_h

class timeR
{
public:
	timeR(); // constucteur 
	~timeR(); // destructeur
	bool DelayMillis_TRUE_FALSE(int delay);
	bool DelayMillis(int delay);
	

private:

	int State;
	unsigned long previousMillis;
	long interval;

};

timeR TIME1, TIME2, TIME3, TIME4; // TIME1 et TIME2 et l'objet de la classe time

timeR::timeR() // constructeur
{
	//Serial.println("Millis INIT");
	State = LOW;
	previousMillis = 0;
	interval = 0;
	
}

timeR::~timeR() // destructeur
{
	/* Rien à mettre ici car on ne fait pas d'allocation dynamique
	dans la classe Personnage. Le destructeur est donc inutile mais
	je le mets pour montrer à quoi cela ressemble.
	En temps normal, un destructeur fait souvent des delete et quelques
	autres vérifications si nécessaire avant la destruction de l'objet. */
} 

bool timeR::DelayMillis_TRUE_FALSE(int delay) { //envoyer un 1 ou 0 en fonction du delay demande

	interval = delay;

	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= interval) {

		previousMillis = currentMillis;

		if (State == LOW) {
			State = HIGH;
		}
		else {
			State = LOW;
		}
	}
	return State;
}

bool timeR::DelayMillis(int delay) { // envoye que un 1 lorsque le temp de tempo et arrive a la fin 

	interval = delay;

	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis;

		return 1;
	}
	return 0;
}
#endif // delay_h
