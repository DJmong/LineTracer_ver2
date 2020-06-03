#include "TracerSensor.h"

//핀 매크로
#define PIN_IN PINC
#define DDR_IN DDRC

//사용할 센서 개수
#define s_count 8

//인터럽트에 들어갈 전역 변수
int result[s_count] = {0,};
int adc_now = 0;
int adc_end = 0;

//초기 설정
void TracerInit(){
  //ADC 포트 설정
  DDR_IN = ~0x3F;
  //ADMUX 초기설정
  ADMUX = (1 << REFS0);
  ADMUX |= 0<<MUX2 | 0<<MUX1||0<<MUX0; 
  // ADC 사용
  ADCSRA = 1 << ADEN; 
  ADCSRA |= 1 << ADSC;
  ADCSRA |= 1<< ADIE;
  //ADC 프리스케일
  ADCSRA |= (1<<ADPS2) |(0<<ADPS1) | (1<<ADPS0); 

  //인터럽트 개시
  sei();
}

//ADC 결과값 리턴
int TracerRead(){
 
  int way = 0;
  for(int i = 0;i < 8;i++){
    way <<= 1;
    way |= result[i];
  }
  adc_end=0;
  return way;

}

//ADC 인터럽트
ISR(ADC_vect){
  result[adc_now] = ADCW;

  //들어오는 값이 500이상일 때 0
  if(result[adc_now] > 800){
    result[adc_now] = 0;
  }
  else{
    result[adc_now] = 1;
  }
  
  //ADC 7채널까지 상승하다가 7일때 0이 됨
  if((ADMUX&0x07) == 0x07){
    ADMUX ^= 0x07;
    adc_now = 0;
    
    adc_end = 1;
  }
  else{
    ADMUX += 0x01;
    adc_now++;
  }
  ADCSRA |= 1 << ADSC;
}
