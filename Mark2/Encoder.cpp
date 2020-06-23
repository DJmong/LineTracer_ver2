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

float dist_L = 0;
float dist_R = 0;
void EncoderInit() {

  int now_a, now_b;

  ENCODER_INIT &= ~0x0F; // 핀 0,1,2,3

  now_a = ENCODER & 0x03;
  now_b = ENCODER & 0x0C;
  
  // 현재 값에 따른 인터럽트 조건 설정
  // 1일 경우 하강 엣지, 0일 경우 상승 엣지로 작동
  if (!(now_a & 0x01)) {
    EICRA |= 1 << ISC00;
  }

  if (!(now_a & 0x02)) {
    EICRA |= 1 << ISC10;
  }

  if(!(now_b & 0x04)){
    EICRA |= 1 << ISC20;
  }

  if(!(now_b & 0x08)){
    EICRA |= 1 << ISC30;
  }
  
  //기본 인터럽트 설정
  //엔코더 A 인터럽트 설정
  EICRA |= 1 << ISC01 | 1 << ISC11; // INT0,1 : rising edge
  EIMSK |= 1 << INT0 | 1 << INT1; // use INT0,1,

  //엔코더 B 인터럽트 설정
  EICRA |= 1 << ISC21| 1 << ISC31;
  EIMSK |= 1 << INT2 | 1 << INT3;

}

float EncoderGetDist(char direc) {
  if (direc == 'L' || direc == 'l') {
    return dist_L;
  }
  else if (direc == 'R' || direc == 'r') {
    return dist_R;
  }
  return -26856565;
}

//**************A모터*************
// A 모터 a상
ISR(INT0_vect) {
  int ISCbit = 1 << ISC00;
  if (EICRA & ISCbit) { //상승 에지일 경우
    dist_R += (ENCODER & 0x02) ? -0.25 : 0.25;
  }
  else { //하강 에지 확인일 경우
    dist_R += (ENCODER & 0x02) ? 0.25 : -0.25;
  }


  EICRA ^= 1 << ISC00;
  EIFR &= ~0x01;

}
// A 모터 b상
ISR(INT1_vect) {
  int ISCbit = 1 << ISC10;
  if (EICRA & ISCbit) { //상승 에지일 경우
    dist_R += (ENCODER & 0x01) ? 0.25 : -0.25;
  }
  else { //하강 에지 확인일 경우
    dist_R += (ENCODER & 0x01) ? -0.25 : 0.25;
  }

  EICRA ^= 1 << ISC10;
  EIFR &= ~0x02;
}



//***************B모터************
// B모터 a상
ISR(INT2_vect) {

  int ISCbit = 1 << ISC20;
  
  if (EICRA & ISCbit) { //상승 에지일 경우
    dist_L += (ENCODER & 0x08) ? 0.25 : -0.25;
  }
  else { //하강 에지 확인일 경우
    dist_L += (ENCODER & 0x08) ? -0.25 : 0.25;
  }

  EICRA ^= 1 << ISC20;
  EIFR &= ~0x04;
}

ISR(INT3_vect) {
  
  int ISCbit = 1 << ISC30;
  if (EICRA & ISCbit) { //상승 에지일 경우
    dist_L += (ENCODER & 0x04) ? -0.25 : 0.25;
  }
  else { //하강 에지 확인일 경우
    dist_L += (ENCODER & 0x04) ? 0.25 : -0.25;
  }

  EICRA ^= 1 << ISC30;
  EIFR &= ~0x08;
}
