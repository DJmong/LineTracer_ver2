#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "Encoder.h"
#include "Motor.h"
#include "timer.h"



void ControlInit();
float GetSpeed(int t, float dist);
void SetSpeed (int L, int R);
int PID_L(int real, int set);
int PID_R(int real, int set);
#endif
