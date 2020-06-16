//system
#include <avr/interrupt.h>

#include "Encoder.h"
//단위 정의
#define rad 40 //바퀴반지름 (단위 mm)
#define PI 3.14159265359 //원주율

// 1/4 주기
#define DIST_CAL rad * PI * 2 / 96

//엔코더 핀 매크로 정의 
#define ENCODER_INIT DDRD
#define ENCODER PIND

//거리
float dist = 0;

void EncoderInit(){
  
  ENCODER_INIT &= ~0x0F; // 핀 0,1,2,3

  EICRA |= 1 << ISC01 | 1 << ISC00 | 1 << ISC11 | 1 << ISC10; // INT0,1
  EICRA |= 1 << ISC21 | 1 << ISC20 | 1 << ISC31 | 1 << ISC30; // INT2,3
  EIMSK |= 1 << INT0 | 1 << INT1 | 1 << INT2 | 1 << INT3; //INT0,1,2,3
}

float get_dist(){
  return dist;
}

//**************A모터*************
// A 모터 a상
bool phase_A = false;
ISR(INT0_vect){
  //정회전
// EICRA ^= 1 << ISC00; // 상승, 하강 에지 순차적으로 인터럽트할 수 있게 해줌
  if(phase_A == true){
    dist += ENCODER == 0x01? 1 : -1;  
    phase_A = false;
  }
  EIFR &= ~0x01;
  
}
// A 모터 b상
ISR(INT1_vect){
  phase_A = true;
  EIFR &= ~0x02;
}



//***************B모터************
// B모터 a상
ISR(INT2_vect){
  int phase_b = ENCODER & 0x08;
  EIFR &= ~0x04;
}
// B모터 b상
ISR(INT3_vect){
  int phase_a = ENCODER & 0x04; 
  EIFR &= ~0x08;
}
