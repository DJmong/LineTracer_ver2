#ifndef _PID_CONTROL_H_
#define _PID_CONTROL_H_

//함수 선언
void ControlInit(int* L_speed, int* R_speed);
int Control(int direction, int* L_speed, int* R_speed);

#endif
