//------------------------
//Arduino use pc817 to control LED(12V)
//choose LED mode by button
//------------------------
#ifndef LED_SIGNBOARD_h
#define LED_SIGNBOARD_h

class LED_signBoard
{
private:
	uint8_t LED_buttonPIN;
	uint8_t LED_greenPIN;
	uint8_t LED_redPIN;
public:
	LED_signBoard();
	~LED_signBoard();
	uint8_t buttonCounter;
	void LED_Mode_initial_set(uint8_t greenPin, uint8_t redPin, uint8_t buttonPin);
	void LED_ON_G();
	void LED_OFF_G();
	void LED_ON_R();
	void LED_OFF_R();
	void LED_Breathe_G(uint8_t PWM_NUM);
	void LED_Breathe_R(uint8_t PWM_NUM);
	void LED_Blink_G(bool change);
	void LED_Blink_R(bool change);
	void LED_Breathe_all(uint8_t PWM_NUM);
	void LED_Blink_all(bool change);
	void LED_ON_all();
	void LED_OFF_all();
};

#endif
	

