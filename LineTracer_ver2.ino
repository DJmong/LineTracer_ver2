#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#include "Control.h"
#include "TracerSensor.h"


int main(void){
  //변수 선언
  int L_realspd, R_realspd;
  int L_spd = 200, R_spd = 200;
  int temp = 100;
  int dist = 0;
  int t = 0;
  //초기 설정
  ControlInit();
  Serial.begin(9600);
  TracerInit();

  while(1){
    
    for(int i = -255; i< 256; i++){
      MotorIn(i, i);
      while(t <= 0) t = TimerGet();
      L_realspd = GetSpeed(t, EncoderGetDist('L'));
      R_realspd = GetSpeed(t, EncoderGetDist('R'));
      Serial.print("Spd   ");
      Serial.print(i);
      Serial.print("  L   ");
      Serial.print(L_realspd);
    
      Serial.print("  R   ");
      Serial.println(R_realspd);
//      L_spd += PID_L(L_realspd, L_spd);
//      R_spd += PID_R(R_realspd, R_spd);
//      SetSpeed(L_spd, R_spd);
      TimerClear();
      EncoderClear('L');
      EncoderClear('R');
      t = 0;
    }
    return 1;
  }
  return 0;
}
