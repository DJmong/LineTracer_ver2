/*
 * 미완성
 * 
 * 
 * 기능
 *  Encoder 모듈과 Timer모듈을 이용해 시간, 거리값을 불러와
 * 현재 속도를 계산할 수 있다.
 *  모터 스피드와 PWM 비율을 계산해 MotorIn 함수를 호출할 수 있다.
 * 
 * 구현 못한거
 *  PID 피드백
 */

//Module Include 
#include "Control.h"
#include "Encoder.h"
#include "Motor.h"
#include "timer.h"

#define Kp 1
#define Ki 0
#define Kd 1

#define SPEED_CAL(x) x/2 //속도 : PWM 비율 연산 값

int spd_L, spd_R;

void ControlInit(){
  EncoderInit();
  MotorInit();
  TimerInit();
}

void SetSpeed (int L, int R){
  // 속도 연산식 만들 것
  spd_L = SPEED_CAL(L);
  spd_R = SPEED_CAL(R);

  MotorIn(spd_L, spd_R);
}
void Turn(int ){
  
}
int PID_L(int real_spd, int set_spd){
  int gain, err;
  static int sum = 0, pre = 0;
  err = set_spd - real_spd;
  
  gain = (int) (Kp * err + Ki * sum + Kd * (err - pre));

  pre = err;
  sum += err;
  sum *= 0.8;
  return gain;
}

int PID_R(int real_spd, int set_spd){
  int gain, err;
  static int sum = 0, pre = 0;
  err = set_spd - real_spd;
  
  gain = (int) (Kp * err + Ki * sum + Kd * (err - pre));

  pre = err;
  sum += err;
  sum *= 0.8;

  return gain;
}

float GetSpeed(int t, float dist){
  float spd = dist/t * 360 / 1000; // m/h 단위
  return spd;
}
