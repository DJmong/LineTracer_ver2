#include "TracerSensor.h"

//핀 매크로
#define PIN_IN PINF
#define DDR_IN DDRF

//사용할 센서 개수
#define s_count 8

//인터럽트에 들어갈 전역 변수
int result_pre[s_count] = {0,};
int result_now[s_count] = {0,};
int adc_now = 0;

//초기 설정
void TracerInit(){
  //ADC 포트 설정
  DDR_IN |= ~0xFF;
  //ADMUX 초기설정
  ADMUX |= (1 << REFS0);
  ADMUX |= 0 << MUX2 | 0 << MUX1 | 0 << MUX0; 
  // ADC 사용
  ADCSRA |= 1 << ADEN; 
  ADCSRA |= 1 << ADSC;
  ADCSRA |= 1<< ADIE;
  //ADC 프리스케일
  ADCSRA |= (1<<ADPS2) |(0<<ADPS1) | (1<<ADPS0); 

}

//ADC 결과값 리턴
int TracerRead(){  
  int way = 0;

  for(int i = 0;i < s_count; i++){
    way <<= 1;
    way |= result_pre[i];
  }
  
  return way;

}

//ADC 인터럽트
ISR(ADC_vect){
  result_now[adc_now] = ADCW;

  //들어오는 값이 500이상일 때 0
  if(result_pre[adc_now] > 800){
    result_pre[adc_now] = 0;
  }
  else{
    result_pre[adc_now] = 1;
  }
  
  //ADC 7채널까지 상승하다가 7일때 0이 됨
  if((ADMUX&0x07) == 0x07){
    ADMUX ^= 0x07;
    for(int i = 0; i < s_count; i++){
      result_pre[i] = result_now[i];
      result_now[i] = 0;
    }
  }
  else{
    ADMUX += 0x01;
    adc_now++;
  }
  ADCSRA |= 1 << ADSC;
}
