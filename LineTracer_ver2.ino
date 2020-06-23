#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "Motor.h"
#include "TracerSensor.h"
#include "Signal.h"
#include "Control.h"
#include "timer.h"
#include "Encoder.h"

int L_speed, R_speed;

int main(void){
  //변수 선언
  int TracerSignal;
  float dist = 0;
  //초기 설정
  Serial.begin(9600);
  timer_init();
  MotorInit();
//  TracerInit();  
  EncoderInit();

  sei();
//  MotorIn(140,140);
  while(1){
    dist = EncoderGetDist('R');
    
    Serial.println(dist);
  }

  return 0;
}
