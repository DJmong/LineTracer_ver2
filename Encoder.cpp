//system
#include <avr/interrupt.h>

#include "Motor.h"
//단위 정의
#define rad 40 //바퀴반지름 (단위 mm)
#define PI 3.14159265359 //원주율

// 1/4 주기
#define DIST_CAL rad * PI * 2 / 960 // 단위 mm

//엔코더 핀 매크로 정의
#define ENCODER_A PORTD&0x14
#define ENCODER_B PORTD&0x28
#define ENCODER_INIT DDRD &= ~(0x14 | 0x28)
#define ENCODER PIND

volatile float dist_L = 0;
volatile float dist_R = 0;
void EncoderInit() {

  int now_a, now_b;

  ENCODER_INIT; // PD 2,4, PD 3,5

  now_a = ENCODER_A;
  now_b = ENCODER_B;

  // 현재 값에 따른 인터럽트 조건 설정
  // 1일 경우 하강 엣지, 0일 경우 상승 엣지로 작동
  if (!(now_a & 0x04)) {
    EICRA |= 1 << ISC00;
  }

  if (!(now_a & 0x08)) {
    EICRA |= 1 << ISC10;
  }

  //기본 인터럽트 설정
  //엔코더 A 인터럽트 설정
  EICRA |= 1 << ISC01 | 1 << ISC11; // INT0,1 ISC set
  EIMSK |= 1 << INT0 | 1 << INT1; // use INT0,1,

}

float EncoderGetDist(char direc) {
  if (direc == 'L' || direc == 'l') {
    return dist_L * DIST_CAL;
  }
  else if (direc == 'R' || direc == 'r') {
    return dist_R * DIST_CAL;
  }
  return -26856565;
}

void EncoderClear(char dir){
  if (dir == 'L' || dir == 'l'){
    dist_L = 0;
  }
  else if(dir == 'R' || dir == 'r'){
    dist_R = 0;
  }
}


  //인터럽트 코드
  //**************A모터*************
  // A 모터 a상
ISR(INT0_vect) {
  int ISCbit = 1 << ISC00;
  if (EICRA & ISCbit) { //상승 에지일 경우
    dist_R += (ENCODER & 0x10) ? -0.5 : 0.5;
  }
  else { //하강 에지 확인일 경우
    dist_R += (ENCODER & 0x10) ? 0.5 : -0.5;
  }
  EICRA ^= 1 << ISC00;
  EIFR &= ~0x01;
}


  //***************B모터************
  // B모터 a상
ISR(INT1_vect) {

  int ISCbit = 1 << ISC10;

  if (EICRA & ISCbit) { //상승 에지일 경우
    dist_L += (ENCODER & 0x20) ? 0.5 : -0.5;
  }
  else { //하강 에지 확인일 경우
    dist_L += (ENCODER & 0x20) ? -0.5 : 0.5;
  }
  EICRA ^= 1 << ISC10;
  EIFR &= ~0x02;
}
