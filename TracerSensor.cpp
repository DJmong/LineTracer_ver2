/*
 *  사용 MCU : Arduino Mega 2560
 *  PIN : Analog 0~7
 *  ADC Prescale : 16MHz / 32
 *  설명 : ADC 0 ~ 7을 이용해 TRCT 5000 센서 값을 읽어온다.
 *  
 */


#include "TracerSensor.h"
#include "Signal.h"
//핀 매크로
#define PIN_IN PINC
#define DDR_IN DDRC

//사용할 센서 개수
#define s_count 8

//인터럽트에 들어갈 전역 변수
volatile int now = 0;
volatile int val[8] = {0,};
//초기 설정
void TracerInit(){
  //ADC 포트 설정
  DDR_IN &= ~0x3F;
  
  //ADMUX
  ADMUX |= 0 << REFS1 | 1 << REFS0; // 기준 전압 5V
  ADMUX |= 0 << ADLAR; //오른쪽 정렬
  ADMUX |= 0 << MUX3 | 0 << MUX2 | 0 << MUX1 | 0 << MUX0; //채널 설정. ADC0 

  //ADCSRA
  ADCSRA |= 1 << ADPS2 | 0 << ADPS1 | 0 << ADPS0;
  ADCSRA |= 1 << ADEN;
  ADCSRA |= 1 << ADIE;
  ADCSRA |= 1 << ADSC;

  
  sei();
}

//ADC 결과값 리턴
int TracerRead(){  
  int way = 1;
  for(int i=0;i<8;i++){
    way <<= 1;
    way |= val[i] > 350 ? 1 : 0;
  }
  return way;
}

ISR(ADC_vect){
  now = ADMUX & (1 << MUX2 | 1 << MUX1 | 1 << MUX0);
  val[now] = ADCW;

  if(now == 7){ //최대 값일 경우 초기화
    ADMUX &= ~(1 << MUX2 | 1 << MUX1 | 1 << MUX0);
  }
  else{
    ADMUX += 1;
  }
  ADCSRA |= 1 << ADSC;
}
