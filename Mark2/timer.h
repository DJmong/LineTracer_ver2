#ifndef _TIMER_H_
#define _TIMER_H_

//시스템
#include <avr/io.h>
#include <avr/interrupt.h>

//함수 선언
void timer_init();
int time_get();
#endif
