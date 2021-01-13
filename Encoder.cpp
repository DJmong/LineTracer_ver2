
/*
 * Encoder 모듈을 사용할 수 있도록 초기화 시켜주는 함수
 * PD 2,3,4,5를 사용하며,
 * 핀 매핑
 * 엔코더 A : PD 2,4
 * 엔코더 B : PD 3,5
 * 각 엔코더는 A상 B상으로 나뉘어 있으며, 각 엔코더의 A상을 인터럽트 핀에 꽂아야 함.
 */

//system
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Motor.h"
//단위 정의
#define rad 40 // 바퀴반지름 (단위 mm)
#define PI 3.14159265359 // 원주율
#define cpr 960 // 모터 한 바퀴당 클럭 주기
#define C rad * 2 * PI // 원주

#define DIST_CAL C / cpr // 현재 바퀴가 움직인 거리 계산식(단위 mm)

//엔코더 핀 매크로 정의
#define ENCODER_A_READ PORTD&0x14 // PD2,4 (D2, D4) Read 
#define ENCODER_B_READ PORTD&0x28 // PD3,5 (D3, D5) Read
#define ENCODER_INIT DDRD &= ~(0x14 | 0x28) // PD 2,3,4,5 (D2, D3, D4, D5) set input
#define ENCODER_READ PIND //

volatile float dist_L = 0; // L바퀴 거리값
volatile float dist_R = 0; // R바퀴 거리값


void EncoderInit() {
  //현재 펄스 값을 읽어 상승에지, 하강에지를 선택함
  int now_a, now_b;

  ENCODER_INIT; // PD 2,4, PD 3,5

  now_a = ENCODER_A_READ;
  now_b = ENCODER_B_READ;

  // 현재 값에 따른 인터럽트 조건 설정
  // 1일 경우 하강 엣지, 0일 경우 상승 엣지로 작동
  // 이와 관련된 내용은 Encoder Motor CPR과 관련된 그림을 참고할 것.
  if (!(now_a & 0x04)) { // PD4의 값이 0일 경우
    EICRA |= 1 << ISC00; // 상승 에지로 설정
  }

  if (!(now_a & 0x08)) { // PD5의 값이 0일 경우
    EICRA |= 1 << ISC10; // 상승 에지로 설정
  }

  //기본 인터럽트 설정
  //엔코더 A 인터럽트 설정
  EICRA |= 1 << ISC01 | 1 << ISC11; // INT0,1 ISC set
  EIMSK |= 1 << INT0 | 1 << INT1; // use INT0,1

  sei(); // 인터럽트 시작
}

/*
 * 모터가 움직인 거리를 계산해 float형으로 변환하는 함수
 * L모터와 R모터의 값을 따로 설정하기 때문에, 호출시 방향을 선택해주어야 한다.
 * L모터는 'L'
 * R모터는 'R'
 * 로 구분한다.
 * 그 이외의 값은 쓰레기 값이 출력된다.
 */
float EncoderGetDist(char direc) { 
  if (direc == 'L' || direc == 'l') {
    return dist_L * DIST_CAL;
  }
  else if (direc == 'R' || direc == 'r') {
    return dist_R * DIST_CAL;
  }
  return -26856565;
}
/*
 * 모터가 움직인 거리를 초기화하는 함수
 * L모터는 'L'
 * R모터는 'R'
 * 로 구분
 */
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
  int ISCbit = 1 << ISC00; // 상승 에지인지 확인하기 위한 변수
  if (EICRA & ISCbit) { //상승 에지일 경우
    dist_R += (ENCODER_READ & 0x10) ? -0.5 : 0.5; //CPR 표 참고할 것
  }
  else { //하강 에지 확인일 경우
    dist_R += (ENCODER_READ & 0x10) ? 0.5 : -0.5; //CPR 표 참고할 것
  }
  EICRA ^= 1 << ISC00; // 하강에지일 경우 상승으로, 상승일 경우 하강으로
  EIFR &= ~0x01; // 인터럽트 플래그 초기화
}


  //***************B모터************
  // B모터 a상
  // 위 코드와 같으므로 추가적인 주석 없음
ISR(INT1_vect) {

  int ISCbit = 1 << ISC10;

  if (EICRA & ISCbit) { //상승 에지일 경우
    dist_L += (ENCODER_READ & 0x20) ? 0.5 : -0.5;
  }
  else { //하강 에지 확인일 경우
    dist_L += (ENCODER_READ & 0x20) ? -0.5 : 0.5;
  }
  EICRA ^= 1 << ISC10;
  EIFR &= ~0x02;
}
