#include "Control.h"
#include "Signal.h"

//매크로
#define S 1
#define F 2

int base_speed = 140;
int max_speed = 230;
int min_speed = 100;

void ControlInit(int *L_speed, int* R_speed){
  *L_speed = base_speed;
  *R_speed = base_speed;
}


int Control(int sensor, int* L_speed, int* R_speed){
  static int pre_sensor = 0;
  int gain[5] = {F,S,0,-S,-F};

  //pid
  float Kp = 19.5;
  float Ki = 0.165;
  float Kd = 0.2;

  // err_sum, err
  static int err_sum = 0;
  static int pre_err = 0;

  
  int PIDControl = 0;
  int err = 0;
  
  //예외처리
  if(sensor == INTERSECTION) sensor=pre_sensor;
  else if(sensor == NONE) sensor = pre_sensor;

  
    
  if(!(sensor&GO)){
    max_speed = 200;
    pre_sensor = sensor;
    //2진비트 디코더 + 가중치 입력  
    for(int i=0;i<5;i++){
      if((sensor&0x01)){
        err += gain[4-i] * (sensor&0x01);
      }
      sensor >>= 1;
    }

    PIDControl = (int)(Kp * err + Ki * err_sum + Kd * (err - pre_err));  
    *L_speed -= PIDControl;
    *R_speed += PIDControl;
    
    //최대, 최소값 조절
    if(*L_speed > max_speed) *L_speed = max_speed;
    if(*R_speed > max_speed) *R_speed = max_speed;
    if(*L_speed < min_speed) *L_speed = min_speed;
    if(*R_speed < min_speed) *R_speed = min_speed;
  
    
  }
  else{
    max_speed = 180;
    pre_sensor = sensor;
    err = 0;
    *L_speed = base_speed;
    *R_speed = base_speed;
  }

    
 
  err_sum = (err_sum * 0.8) + err;
  pre_err = err;
  return 1;
}
