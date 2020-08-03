#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#include "Control.h"
#include "TracerSensor.h"
/*
 * 라인트레이서 프로젝트 (20.03.01~20.07.10) 
 * MCU : Arduino Nano
 * 사용 부품 : IR센서 묶음(TracerSensor), 엔코더 모터(Encoder, Motor)
 * 현재 구현단계 : TracerSensor을 이용해 바닥의 선을 인식하는 중
 * 이전 구현단계 : Encoder을 이용해 모터의 속도(m/h), 거리(mm)를 측정
 * 
 * 현재 문제점
 * 바닥의 선을 인식하지 못함.
 */

int main(void){
  //변수 선언
  int L_realspd, R_realspd;
  int L_spd = 80, R_spd = 80;
  int temp = 100;
  int dist = 0;
  int t = 0;
  int sig[8] = {0, };
  int val[8] = {590, 730, 300, 230, 140, 300, 120, 300};
  int dif = 0;
  //초기 설정
  ControlInit();
  Serial.begin(9600);
  TracerInit();
//  MotorIn(L_spd, R_spd);
  while(1){
    TracerRead(sig);
    for(int i = 0; i < 8; i++){      
      Serial.print(sig[i]);
      Serial.print(" ");
    }
      Serial.println("");
//    while(t <= 0) t = TimerGet();
    L_realspd = GetSpeed(t, EncoderGetDist('L'));
    R_realspd = GetSpeed(t, EncoderGetDist('R'));
    
  }
  return 1;
}
