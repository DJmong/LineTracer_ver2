#ifndef _ENCODER_H_
#define _ENCODER_H_

// system
#include <avr/io.h>

// 함수 정의
void EncoderInit();
float EncoderGetDist(char direc);
void EncoderClear(char dir);
#endif
