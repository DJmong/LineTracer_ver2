#include "Control.h"

#define Kp 1
#define Ki 0
#define Kd 1

#define SPEED_CAL(x) x/2; //속도 : PWM 비율 연산 값

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
