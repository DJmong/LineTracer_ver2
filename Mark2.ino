#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include "Motor.h"
#include "TracerSensor.h"
#include "Signal.h"
#include "Control.h"
#include "timer.h"
#include "Encoder.h"

void test(int n, int L_speed, int R_speed, int next_delay);

int L_speed, R_speed;

int main(void){
  //변수 선언
  int TracerSignal;
  float dist;
  //초기 설정
  Serial.begin(9600);
  timer_init();
  MotorInit();
  TracerInit();  
  EncoderInit();

  sei();
  MotorIn(140,140);
  while(1){
    dist = get_dist();
    Serial.println(dist);
  }

  return 0;
}


/*
void test(int Tracer, int L_speed, int R_speed, int next_delay){
    if(Serial.read() == 's'){
      MotorIn(0,0);
      while(!(Serial.read() == 'g'));
      ControlInit(&L_speed, &R_speed);
      MotorIn(L_speed, R_speed);
    }

    
    Serial.print("Tracer : ");
    Serial.print(Tracer, 2);
    Serial.print("                ");
    Serial.print("L_speed : ");
    Serial.print(L_speed);
    Serial.print("  ");
    Serial.print("R_speed : ");
    Serial.print(R_speed);
    Serial.print("  ");
    Serial.print("delay : ");
    Serial.println(next_delay);
}
*/
