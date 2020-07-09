#include "timer.h"

//인터럽트 전역 변수
volatile int time_s = 0;
volatile int tmp = 0;
int a = -2000;
void TimerInit(){
//  TCCR1A |= (1<<WGM11)|(0<<WGM10);
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  TIMSK1 |= 1 << TOIE1;
  TCNT1 = a;
  OCR1A = 0x500;
}


int TimerGet(){
  return time_s;
}

void TimerClear(){
  time_s = 0;
}
ISR(TIMER1_OVF_vect){
  tmp++;
  TCNT1 = a;
  if(tmp > 1000){
    tmp = 0;
    time_s++;
  }

  
}
