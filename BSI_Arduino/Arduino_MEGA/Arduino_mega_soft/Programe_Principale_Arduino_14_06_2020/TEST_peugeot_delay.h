#include <Arduino.h>
#include "peugeot_config.h"

#ifndef peugeot_delay_h
#define peugeot_delay_h

class timeR2
{
public:
	timeR2(int d, bool a); // constucteur 
	~timeR2(); // destructeur
	//bool DelayMillis_TRUE_FALSE(int delay);
	bool DelayMillis();
	//bool DelayMillis();

private:

	int State;
	unsigned long previousMillis;
	long interval;
	unsigned long currentMillis;
	bool A;
	

};

//https://fr.coursera.org/lecture/programmation-orientee-objet-cpp/constructeurs-par-defaut-en-c-ecTcJ

timeR2 TIME10(5000, 0); // phares automatiqu OFF
timeR2 TIME11(10000, 0); // TIME1 et TIME2 et l'objet de la classe time
timeR2 TIME20(50,0); // claxon
timeR2 TIME30(1000, 0); // claxon
//timeR2 TIME30(100); // TIME1 et TIME2 et l'objet de la classe time
//timeR2 TIME40(1000); // TIME1 et TIME2 et l'objet de la classe time
//timeR2 TIME50(5000); // TIME1 et TIME2 et l'objet de la classe time

timeR2::timeR2(int d = 1000, bool a = 0) // constructeur par defaut 
	: State(LOW), previousMillis(0), interval(d), currentMillis(0), A(a)
{
	Serial.println("CONSTRUCTEUR");
}

timeR2::~timeR2() // destructeur

{
	Serial.println("DESTRUCTEUR");
} 
/*
bool timeR2::DelayMillis_TRUE_FALSE(int delay) { //envoyer un 1 ou 0 en fonction du delay demande

	interval = delay;

	currentMillis = millis();

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
*/
	bool timeR2::DelayMillis() { // envoye que un 1 lorsque le temp de tempo et arrive a la fin 
	//bool timeR::DelayMillis() { // envoye que un 1 lorsque le temp de tempo et arrive a la fin 
	//interval = delay;

		if (A == 0)  // <- technique NOOOB :/
		{
			previousMillis = millis();
			A = 1;
		}


	currentMillis = millis();
	//Serial.println(interval);
	//Serial.println(currentMillis);
	//Serial.println(previousMillis);

	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis;

		A = 0;
		return 1;
	}
	// = millis();
	return 0;
}
#endif // delay_h
