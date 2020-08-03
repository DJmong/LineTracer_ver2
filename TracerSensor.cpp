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

/*
 * TracerSensor 모듈 사용을 위한 초기화 함수
 * 핀 설정, ADC 레지스터 설정을 함
 * 일반 ADC모드,  
 */
//초기 설정
void TracerInit(){
  //ADC 포트 설정
  DDR_IN &= ~0x3F; // ADC 0~8 핀 입력으로 전환
  
  //ADMUX
  ADMUX |= 0 << REFS1 | 1 << REFS0; // 기준 전압 5V
  ADMUX |= 0 << ADLAR; //오른쪽 정렬
  ADMUX |= 0 << MUX3 | 0 << MUX2 | 0 << MUX1 | 0 << MUX0; //채널 설정 ADC0 

  //ADCSRA
  ADCSRA |= 1 << ADPS2 | 0 << ADPS1 | 0 << ADPS0; // prescaler 1/32
  ADCSRA |= 1 << ADEN; // ADc Enable
  ADCSRA |= 1 << ADIE; // ADc Interrupt Enable
  ADCSRA |= 1 << ADSC; // ADc Start Convertion

  
  sei();
}
/*
 * 함수 호출 인자로 배열의 주소를 받아와
 * 해당 배열의 주소로 값을 복사하는 함수
 * 리턴 없음, 주소 값을 이용해 값을 복사함
 */
//ADC 결과값 리턴
void TracerRead(int *arr){  
  for(int i=0; i<8; i++){
    arr[i] = val[i];
  }
}

ISR(ADC_vect){
  now = ADMUX & (1 << MUX2 | 1 << MUX1 | 1 << MUX0); // 현재 ADMUX값을 읽어 몇 번 센서를 읽었는지 저장
  val[now] = ADCW; // 읽은 센서 번호에 맞는 배열에 현재 아날로그 값 저장

  if(now == 7){ //최대 값일 경우 초기화
    ADMUX &= ~(1 << MUX2 | 1 << MUX1 | 1 << MUX0); //ADMUX를 초기화 함
  }
  else{
    ADMUX += 1; // 다음 센서를 읽도록 MUX 레지스터를 변경함
  }
  ADCSRA |= 1 << ADSC; // ADC 재시작
}
