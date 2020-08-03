#ifndef _TracerSensor_H_
#define _TracerSensor_H_

//시스템
#include <avr/io.h>
#include <avr/interrupt.h>

//함수 선언
void TracerInit();
void TracerRead(int *arr);


#endif
