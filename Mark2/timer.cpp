#include "timer.h"

//인터럽트 전역 변수
long time_ms = 0;
int a = -2000;
void timer_init(){
//  TCCR1A |= (1<<WGM11)|(0<<WGM10);
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  TIMSK1 |= 1 << TOIE1;
  TCNT1 = a;
  OCR1A = 0x500;
}


int time_get(){
  int temp = time_ms;
  time_ms = 0;
  return temp;
}
ISR(TIMER1_OVF_vect){

  TCNT1 = a;
  time_ms++;
  
}
