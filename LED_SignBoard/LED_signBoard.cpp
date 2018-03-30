//For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <LED_signBoard.h>

LED_signBoard::LED_signBoard(void) {
	//default
	buttonCounter = 0;
}


LED_signBoard::~LED_signBoard(void) {

}

void LED_signBoard::LED_Mode_initial_set(uint8_t greenPin, uint8_t redPin, uint8_t buttonPin) {
		
		LED_greenPIN = greenPin;
		LED_redPIN = redPin;
		LED_buttonPIN = buttonPin;
		pinMode(LED_greenPIN, OUTPUT);
		pinMode(LED_redPIN, OUTPUT);
		pinMode(LED_buttonPIN, INPUT);
}

void LED_signBoard::LED_ON_G() {	
		digitalWrite(LED_greenPIN,LOW);	
}

void LED_signBoard::LED_OFF_G() {	
		digitalWrite(LED_greenPIN,HIGH);	
}

void LED_signBoard::LED_ON_R() {	
		digitalWrite(LED_redPIN,LOW);	
}

void LED_signBoard::LED_OFF_R() {	
		digitalWrite(LED_redPIN,HIGH);	
}

void LED_signBoard::LED_ON_all() {	
		digitalWrite(LED_redPIN,LOW);	
		digitalWrite(LED_greenPIN,LOW);	
}

void LED_signBoard::LED_OFF_all() {	
		digitalWrite(LED_redPIN,HIGH);	
		digitalWrite(LED_greenPIN,HIGH);	
}

void LED_signBoard::LED_Breathe_G(uint8_t PWM_NUM) {
	analogWrite(LED_greenPIN,PWM_NUM);	
}

void LED_signBoard::LED_Breathe_R(uint8_t PWM_NUM) {
	analogWrite(LED_redPIN,PWM_NUM);	
}

void LED_signBoard::LED_Breathe_all(uint8_t PWM_NUM) {
	analogWrite(LED_greenPIN,PWM_NUM);	
	analogWrite(LED_redPIN,PWM_NUM);	
}

void LED_signBoard::LED_Blink_G(bool change) {
	if (change) {
		LED_ON_G();
	} else {
		LED_OFF_G();
	}
}			


void LED_signBoard::LED_Blink_R(bool change) {
	if (change) {
		LED_ON_R();
	} else {
		LED_OFF_R();
	}
}

void LED_signBoard::LED_Blink_all(bool change) {
	if (change) {
		LED_ON_G();
		LED_ON_R();
	} else {
		LED_OFF_G();
		LED_OFF_R();
	}
}

