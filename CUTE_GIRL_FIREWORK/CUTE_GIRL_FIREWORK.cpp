// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <CUTE_GIRL_FIREWORK.h>


CUTE_GIRL_FIREWORK::CUTE_GIRL_FIREWORK(void){
}

CUTE_GIRL_FIREWORK::~CUTE_GIRL_FIREWORK(void){

}


void CUTE_GIRL_FIREWORK::cute_girl_firework_initial(uint8_t PUL_pin, uint8_t DIR_pin,uint8_t riseHand_heat, 
                                                    uint8_t relay_heat, uint8_t limit_sensor, int heat_time,
                                                    int no_heat_time, int step_goal) 
{
    set_BHST(no_press);
	CLPMTR_DIR = DIR_pin;
	CLPMTR_PUL = PUL_pin;
    relay_heat_pin = relay_heat;
    limit_sensor_pin = limit_sensor;
    riseHand_heat_pin = riseHand_heat;
	pinMode(CLPMTR_DIR, OUTPUT);
	pinMode(CLPMTR_PUL, OUTPUT);
	pinMode(relay_heat_pin, OUTPUT);
    turn_on_relay_heat(false);
    CLP_MOTOR_Initial(PUL_pin, DIR_pin);
    setCLPMTR_LOW();
    setCLPMTR_CW();

    pinMode(riseHand_heat, INPUT);
    pinMode(limit_sensor, INPUT);
    motor_control_initial(step_goal);
    heat_control_initial(heat_time, no_heat_time);
    action_control_initial();
    timer_control_initial();
    Timer4_initial();
    Timer5_initial();
}

void CUTE_GIRL_FIREWORK::interrupt_initial(interrupt_callback function) {
    pinMode(limit_sensor_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(limit_sensor_pin), function, FALLING);
    IS_state.limit_sensor_state = false;
    IS_state.prevent_startup_into_ISR = false; 
}


void CUTE_GIRL_FIREWORK::motor_control_initial(int step_goal) { 
    MC.arrive = false;
    MC.DIR_state = CW;      //0 CW ,1 CCW
    MC.set_speed = 5;              //for timer4 that creates pulse to driver    
    MC.pulse_count = 0;
    MC.pulse_goal = step_goal * 16 / 10;
}

void CUTE_GIRL_FIREWORK::heat_control_initial(int heat, int no_heat) { 
    set_heat_action(no_heating);
    HC.heat_time = heat;
    HC.no_heat_time = no_heat;
    HC.heat_count = 0;
    HC.no_heat_count = 0;
}

void CUTE_GIRL_FIREWORK::action_control_initial() { 
    AC.action_seq = 0;
    AC.do_once = false;
}

void CUTE_GIRL_FIREWORK::timer_control_initial() { 
    TC.timerSW = false;
    TC.pulse_count = 0;
    TC.delay_ok = false;
}

void CUTE_GIRL_FIREWORK::Timer4_initial() {
  Timer4CountSet[0] = 65531; //0.02ms 20us
  Timer4CountSet[1] = 65524; //0.048ms 48us
  Timer4CountSet[2] = 65511; //0.1ms 100us
  Timer4CountSet[3] = 65473; //0.25ms 250us
  Timer4CountSet[4] = 65411; //0.5ms 500us
  Timer4CountSet[5] = 65286; //1ms 1000us
  Timer4CountSet[6] = 64286; //5ms 5000us
  Timer4CountSet[7] = 63036; //10ms
  Timer4CountSet[8] = 53036; //50ms
  Timer4CountSet[9] = 40536; //100ms
  TCCR4A =  0x00;
  TCCR4B =  0X03;          //設定 除頻=3 16Mhz/64=0.25Mhz
  //1/0.25Mhz=4us,每4us計數一次,假設設定為65531,
  //共65536-65531=5,第5次發生計時中斷,共經時間5*4us=20us
  TCCR4C =  0x00;
  TIMSK4 =  0x00;      //timer4 stop
}


void CUTE_GIRL_FIREWORK::Timer5_initial() {
  Timer5CountSet[0] = 65531; //20us
  Timer5CountSet[1] = 65521;    //0.06ms 60us
  Timer5CountSet[2] = 65511; //0.1ms 100us
  Timer5CountSet[3] = 65473;  //0.25ms 250us
  Timer5CountSet[4] = 65411;  //0.5ms 500us
  Timer5CountSet[5] = 65286;   //1ms 1000us
  Timer5CountSet[6] = 65036;   //2ms 2000us
  TCCR5A =  0x00;
  TCCR5B =  0X03;          //除頻=5 16Mhz/1024 除頻=3 16Mhz/64
  TCCR5C =  0x00;
  TIMSK5 =  0x00;      //timer5 stop
}

void CUTE_GIRL_FIREWORK::set_BHST(BTN_HEAT_ST val) {
    BHST = val;
}

BTN_HEAT_ST CUTE_GIRL_FIREWORK::get_BHST() {
    return BHST;
}

void CUTE_GIRL_FIREWORK::set_motor_speed(uint8_t val) {
//for timer4 to create pulse   
    int speed = val;
    if( val<0 ) {
        speed = 0;
    }
    if( val>6 ) {
        speed = 6;
    }    
    MC.set_speed = speed;
}

void CUTE_GIRL_FIREWORK::set_motor_direction(MTRDIR val) {
    MC.DIR_state = val;
    if( val == CW ) {    
        setCLPMTR_CW();
    } else {
        setCLPMTR_CCW();
    }
}

void CUTE_GIRL_FIREWORK::timer4_ISR() {
 timer4_stop();
 TC.timerSW = !TC.timerSW;
 TCNT4 = Timer4CountSet[MC.set_speed];
  if (!MC.arrive) {
    if ( TC.timerSW ) {
      setCLPMTR_HIGH();
    }  else {
      setCLPMTR_LOW();
      add_motor_pulse_count();
    }
    timer4_start();
  } else {
    setCLPMTR_LOW();
    timer4_stop();
  }
}

void CUTE_GIRL_FIREWORK::timer5_ISR() {
    timer5_stop();
    TCNT5 = Timer5CountSet[timer5set];
    if(get_action_seq() == 1) {
        add_timer_count();
        if(get_timer_count() == delay_1s) {
            clear_timer_count();
            set_delay_ok(true);
            timer5_stop();
        } else {
            timer5_start();
        }
    } else if(get_action_seq() == 2 && MC.arrive == 0) {
        if(get_motor_pulse_count() == MC.pulse_goal) {
            clear_motor_pulse_count();
            set_heat_action(heating);
            MC.arrive = 1;
            timer5_start();
        } else {
            timer5_start();
        }
    } else if(get_action_seq() == 2 && get_heat_action() == heating) {
        add_heat_count();
        //Serial.println("heating");
        //Serial.println(get_heat_count());
        if(get_heat_count() == HC.heat_time) {
            clear_heat_count();
            set_heat_action(no_heating_temporarily);
            timer5_start();
        } else {
            timer5_start();
        }  
    } else if(get_action_seq() == 2 && get_heat_action() == no_heating_temporarily) {
        add_no_heat_count();
        //Serial.println("no heating temporarily");
        //Serial.println(get_no_heat_count());
        if(get_no_heat_count() == HC.no_heat_time) {
            clear_no_heat_count();
            set_heat_action(heating);
            timer5_start();
        } else {
            timer5_start();
        }
    } 


}

void CUTE_GIRL_FIREWORK::timer4_start() {
  TIMSK4 = 0x01;       //timer4 start
}

void CUTE_GIRL_FIREWORK::timer4_stop() {
  TIMSK4 = 0x00;     //timer4 stop
}

void CUTE_GIRL_FIREWORK::timer5_start() {
  TIMSK5 = 0x01;       //timer5 start
}

void CUTE_GIRL_FIREWORK::timer5_stop() {
  TIMSK5 = 0x00;     //timer5 stop
}

void CUTE_GIRL_FIREWORK::set_prevent_startup_into_ISR(bool val) {
    IS_state.prevent_startup_into_ISR = true;
}

bool CUTE_GIRL_FIREWORK::get_prevent_startup_into_ISR() {
    return IS_state.prevent_startup_into_ISR = true;
}

uint8_t CUTE_GIRL_FIREWORK::get_action_seq() {
    return AC.action_seq;
}

void CUTE_GIRL_FIREWORK::set_action_seq(uint8_t val) {
    AC.action_seq = val;
}

void CUTE_GIRL_FIREWORK::set_limit_sensor_state(bool val) {
    IS_state.limit_sensor_state = val;
}

bool CUTE_GIRL_FIREWORK::get_limit_sensor_state() {
    return IS_state.limit_sensor_state;
}

bool CUTE_GIRL_FIREWORK::get_do_once() {
    return AC.do_once;
}

void CUTE_GIRL_FIREWORK::set_do_once(bool val) {
    AC.do_once = val;
}

void CUTE_GIRL_FIREWORK::clear_motor_pulse_count() {
    MC.pulse_count = 0;
}

void CUTE_GIRL_FIREWORK::add_motor_pulse_count() {
    MC.pulse_count = MC.pulse_count + 1;
}

int CUTE_GIRL_FIREWORK::get_motor_pulse_count() {
   return MC.pulse_count;
}

void CUTE_GIRL_FIREWORK::CLPMTR_setting(MTRDIR DIR_val, bool CLPM_arrive) {
  set_motor_direction(DIR_val);
  TCNT4 = Timer4CountSet[MC.set_speed];
  TCNT5 = Timer5CountSet[timer5set];
  MC.arrive = CLPM_arrive;
  timer4_start();
  timer5_start();
}

void CUTE_GIRL_FIREWORK::add_timer_count() {
    TC.pulse_count = TC.pulse_count+1; 
}

void CUTE_GIRL_FIREWORK::clear_timer_count() {
    TC.pulse_count = 0;
}

int CUTE_GIRL_FIREWORK::get_timer_count() {
    return TC.pulse_count;
}

bool CUTE_GIRL_FIREWORK::get_delay_ok() {
    return TC.delay_ok;
}

void CUTE_GIRL_FIREWORK::set_delay_ok(bool val) {
    TC.delay_ok = val;
}

bool  CUTE_GIRL_FIREWORK::get_riseHand_heat_BTN_state() {
    return digitalRead(riseHand_heat_pin);
}

void CUTE_GIRL_FIREWORK::turn_on_relay_heat(bool val) {
    //false:relay off, true:relay on
    digitalWrite(relay_heat_pin, val);
}

HEAT_ACTION CUTE_GIRL_FIREWORK::get_heat_action() {
    return HC.heat_action;
}

void CUTE_GIRL_FIREWORK::set_heat_action(HEAT_ACTION val) {
    HC.heat_action = val;
}

void CUTE_GIRL_FIREWORK::add_heat_count() {
    HC.heat_count = HC.heat_count + 1;
}

void CUTE_GIRL_FIREWORK::add_no_heat_count() {
    HC.no_heat_count = HC.no_heat_count + 1;
}

int CUTE_GIRL_FIREWORK::get_heat_count() {
    return HC.heat_count;
}

int CUTE_GIRL_FIREWORK::get_no_heat_count() {
    return HC.no_heat_count;
}

void CUTE_GIRL_FIREWORK::clear_heat_count() {
    HC.heat_count = 0;
}

void CUTE_GIRL_FIREWORK::clear_no_heat_count() {
    HC.no_heat_count = 0;
}
