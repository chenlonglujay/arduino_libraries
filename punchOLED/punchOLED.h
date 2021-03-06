/***************************************************************************
* for punchCounter project
***************************************************************************/

#ifndef PUNCHOLED_H
#define PUNCHOLED_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "OLED_SSD1306.h"

#define ENC_SW 1
#define ENS_SW 0
#define picture8x16 0
#define picture128x64 0

typedef struct punch_digit{
	uint8_t THD;	//thousand
	uint8_t HUD;	//hundred
	uint8_t TEN;	
	uint8_t ONE;	
}punch_count_digit;

typedef struct timeSaveOLED{
	uint8_t day_h;
	uint8_t day_l;
	uint8_t hour_h;
	uint8_t hour_l;
	uint8_t minute_h;
	uint8_t minute_l;
	uint8_t second_h;
	uint8_t second_l;
}time_save_OLED;

typedef enum goalClear{clear_N=0, clear_Y}goal_clear;

class punchOLED:protected OLED_SSD1306
{
 private:
	void showNUM(unsigned char x, unsigned char y, unsigned char N);
#if ENC_SW
  void showENC(unsigned char x, unsigned char y, unsigned char N);
#endif
#if ENS_SW
  void showENS(unsigned char x, unsigned char y, unsigned char N);
#endif
	void showSSYM(unsigned char x, unsigned char y, unsigned char N);
#if picture8x16
	void showPicture8x16(unsigned char x, unsigned char y, unsigned char N);
#endif
	void showPicture16x16(unsigned char x, unsigned char y, unsigned char N);
	void showCN(unsigned char x, unsigned char y, unsigned char N);
    goal_clear GC;
 public:
  punchOLED();
  ~punchOLED();
	void punchOLED_initial();	
	void showPicture128x64_lab();
	void showPicture128x64_fist();
	void show_watch_page1(punch_count_digit *pcd, time_save_OLED *tso);
	void show_watch_page2(punch_count_digit *power, bool left_right,bool mode, punch_count_digit *sensitivity);
	void show_watch_page3(punch_count_digit *goal, punch_count_digit *pcd);
	void clear_screen();
	void show_watch_reset();
};
#endif
