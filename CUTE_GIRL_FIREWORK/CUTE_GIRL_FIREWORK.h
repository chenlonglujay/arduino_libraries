//control CLP MOTOR by button
//hetting by electric control 
//hardware:
//MEGA 2560
//CLP MOTOR AND DRIVER
//function
//(1)CW (2)CCW
//using TIMER4 and TIMER5 to detect and create pulse
//CLPMotor driver(Hybird Servo Drive) Pulse needs to set 1600
//divide 1600,the smallest pulse is 8(5steps),so at least run once needs 5 steps
//go_step*pulseChange/10 
//5 * 16 /10=8step

#ifndef CUTE_GIRL_FIREWORK_h
#define CUTE_GIRL_FIREWORK_h

#include <CLP_MOTOR.h>
#define timer5set 5
#define delay_1s 1000

typedef enum MTRDIR {CW = 0,CCW};

typedef enum BTN_HEAT_ST {
    no_press = 0,
    pressed
};

typedef enum HEAT_ACTION {
    no_heating = 0,
    heating,
    no_heating_temporarily
};

struct interrupt_state {
    bool limit_sensor_state;     //true: sensing switch on
    bool prevent_startup_into_ISR;
};

struct motor_control {
    bool arrive;
    bool DIR_state;      //0 CW ,1 CCW
    uint8_t set_speed;
    int pulse_count;
    int pulse_goal;
};

struct  heat_control {
    HEAT_ACTION heat_action;
    int heat_time;
    int no_heat_time;
    int heat_count;
    int no_heat_count;
};

struct action_control {
    uint8_t action_seq;
    bool do_once;
};

struct timer_control {
    bool timerSW;
    int pulse_count;
    bool delay_ok;
};

class CUTE_GIRL_FIREWORK : protected CLPMTR
{
private: 
    uint8_t limit_sensor_pin;
    uint8_t riseHand_heat_pin;
    uint8_t relay_heat_pin;
    uint8_t CLPMTR_PUL;
    uint8_t CLPMTR_DIR;
    void motor_control_initial(int step_goal);
    void heat_control_initial(int heat, int no_heat);
    void action_control_initial();
    void timer_control_initial();
    unsigned int Timer4CountSet[10];
    unsigned int Timer5CountSet[7];
    void add_motor_pulse_count();
    void add_timer_count();
    void add_heat_count();
    void add_no_heat_count();
public:    
    CUTE_GIRL_FIREWORK();
    ~CUTE_GIRL_FIREWORK();
    typedef void(*interrupt_callback)(void); 
    void cute_girl_firework_initial(uint8_t PUL_pin, uint8_t DIR_pin, 
                               uint8_t relay_heat, 
                               uint8_t riseHand_heat, uint8_t limit_sensor,
                               int heat_time, int no_heat_time,
                               int step_goal);
    void interrupt_initial(interrupt_callback function);
    BTN_HEAT_ST BHST;
    void set_BHST(BTN_HEAT_ST val);
    BTN_HEAT_ST get_BHST();
    struct interrupt_state IS_state;
    struct motor_control MC;	
    struct heat_control HC;
    struct action_control AC;
    struct timer_control TC;
    void Timer4_initial();
    void Timer5_initial();
    void set_motor_speed(uint8_t val);
    void set_motor_direction(MTRDIR val);
    void timer4_ISR(); 
    void timer5_ISR();
    void timer4_start();
    void timer4_stop();
    void timer5_start();
    void timer5_stop();
    void set_prevent_startup_into_ISR(bool val);
    bool get_prevent_startup_into_ISR();
    uint8_t get_action_seq();
    void set_action_seq(uint8_t);
    void set_limit_sensor_state(bool val);
    bool get_limit_sensor_state();
    bool get_do_once();
    void set_do_once(bool val);
    void clear_motor_pulse_count();
    int get_motor_pulse_count();
    void CLPMTR_setting(MTRDIR DIR_val, bool CLPM_arrive);
    void clear_timer_count();
    int get_timer_count();
    bool get_delay_ok();
    void set_delay_ok(bool val);
    bool get_riseHand_heat_BTN_state();
    void turn_on_relay_heat(bool val);
    HEAT_ACTION get_heat_action();
    void set_heat_action(HEAT_ACTION val);
    int get_heat_count();
    int get_no_heat_count();
    void clear_heat_count();
    void clear_no_heat_count();
};

#endif
