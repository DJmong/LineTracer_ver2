#include "Motor.h"

//매크로 정의
#define set_bit(value, bit) ( _SFR_BYTE(value) |=  _BV(bit))
#define clear_bit(value, bit) ( _SFR_BYTE(value) &= ~_BV(bit))


//모터 핀 매크로 정의
#define L_PWM OCR2A // pin11
#define R_PWM OCR0A // pin6
#define MOTOR PORTB
#define MOTOR_INIT DDRB
//모터 회전 방향 매크로 정의
#define L_BACK      (set_bit(MOTOR, 4));\
                    (clear_bit(MOTOR, 2));

#define L_FORWARD   (set_bit(MOTOR, 2));\
                    (clear_bit(MOTOR, 4));
                    
#define R_BACK      (set_bit(MOTOR, 1));\
                    (clear_bit(MOTOR, 0));

#define R_FORWARD   (set_bit(MOTOR, 0));\
                    (clear_bit(MOTOR, 1));

//진행 방향 매크로 정의
#define STOP (clear_bit(MOTOR, 0));\
             (clear_bit(MOTOR, 1));\
             (clear_bit(MOTOR, 2));\
             (clear_bit(MOTOR, 4));

#define GO L_FORWARD;\
           R_FORWARD;

//모터 초기설정
void MotorInit() {
  MOTOR_INIT |= 0x3F; // motor control, pwm output
  DDRD |= 0x40; // L pwm output

  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  TCCR0A |= (1 << COM0A1) | (0 << COM0A0);
  TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);

  TCCR2A |= (1 << WGM01) | (1 << WGM00);
  TCCR2A |= (1 << COM2A1) | (0 << COM2A0);
  TCCR2B |= (0 << CS02) | (0 << CS01) | (1 << CS00);

  //stb 신호
  set_bit(MOTOR, 5);
  GO;
}

//모터 입력
void MotorIn(int L_speed, int R_speed) {
  /*
  if(L_speed == 125) L_speed = 0;
  else if(L_speed > 255)L_speed = 255;
  else if(L_speed < 125){
    if(L_speed < 0) L_speed = 0;
    L_BACK;
    L_speed = 255 - L_speed;
  }
  else L_FORWARD;
  
  if(R_speed == 125) R_speed = 0;
  else if(R_speed > 255) R_speed = 255;
  else if(R_speed < 125){
    if(R_speed < 0) R_speed = 0;
    R_BACK;
    R_speed = 255 - R_speed;
  }
  else R_FORWARD;
  */
  L_PWM = L_speed;
  R_PWM = R_speed;
}
