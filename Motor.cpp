/*
 * 사용 MCU : Arduino NANO
 * Prescale : 1
 * 설명 : 모터 드라이버를 통해 DC 모터를 제어하는 모듈
 * 
 */


#include "Motor.h"

//매크로 정의
#define set_bit(value, bit) ( _SFR_BYTE(value) |=  _BV(bit))
#define clear_bit(value, bit) ( _SFR_BYTE(value) &= ~_BV(bit))


//모터 핀 매크로 정의
#define L_PWM OCR0A
#define R_PWM OCR2A

//모터 회전 방향 매크로 정의
#define L_FORW (set_bit(PORTB, 4));\
               (clear_bit(PORTB, 5));

#define L_BACK (set_bit(PORTB, 5));\
               (clear_bit(PORTB, 4));

#define R_FORW (set_bit(PORTB, 0));\
               (clear_bit(PORTB, 1));

#define R_BACK (set_bit(PORTB, 1));\
               (clear_bit(PORTB, 0));

static int SpdExc(int spd, char dir);

//모터 초기설정
void MotorInit() {
  //OCR
  set_bit(DDRB, 3);
  set_bit(DDRD, 6);
  
  //모터 핀
  set_bit(DDRB, 0x33);
  

  //STBY 핀 설정
  DDRB |= 0x04;
  PORTB |= 0x04;
  
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  TCCR0A |= (1 << COM0A1) | (0 << COM0A0);
  TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);

  TCCR2A |= (1 << WGM01) | (1 << WGM00);
  TCCR2A |= (1 << COM2A1) | (0 << COM2A0);
  TCCR2B |= (0 << CS02) | (0 << CS01) | (1 << CS00);

  L_FORW;
  R_FORW;
}

//모터 입력
void MotorIn(int L_spd, int R_spd) {

  L_spd = SpdExc(L_spd, 'L');
  R_spd = SpdExc(R_spd, 'R');
  
  L_PWM = L_spd;
  R_PWM = R_spd;
}

//예외처리 함수
static int SpdExc(int spd, char dir){
  const int min_spd = -30;

  //음수 속도값 보정
  if(spd < min_spd){
    spd =  -spd;
    if(dir == 'L' || dir == 'l'){
      L_BACK;    
    }
    else if(dir == 'R' || dir == 'r'){
      R_BACK;
    }
  }
  else{
    if(dir == 'L' || dir == 'l'){
      L_FORW;
    }
    else if(dir == 'R' || dir == 'r'){
      R_FORW;
    }
  }

  //속도 최대 값 제한
  if(spd > 255){
    spd = 255;
  }

  //에러 방지
  if(spd < 0){
    spd = 0;
  }

  return spd;
}
