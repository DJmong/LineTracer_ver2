/*
 * 타이머 인터럽트 1번 (16bit timer)을 이용해 1초를 만드는 함수
 * 구글링하셈 기억 안남
 */

//시스템
#include <avr/io.h>
#include <avr/interrupt.h>


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

// 시간 값을 읽음
int TimerGet(){
  return time_s;
}

// 시간 값 초기화
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
