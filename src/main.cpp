#include <Arduino.h>
#include <CapacitiveSensor.h>
#include <notes.h>
#include <math.h>


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2); 
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6); 


#define SPEAKER 11
#define DELAY  (1*1000000)/8000

// volatile float i = 0;
volatile bool overflow = false;
volatile uint8_t overflowCnt = 0;
volatile uint8_t val = 0;
volatile uint16_t i = 0;

SIGNAL(TIMER2_OVF_vect){
  // overflowCnt++;
  // if(overflowCnt >= 8){
  //   overflow = true;
  //   overflowCnt = 0;
  //   // digitalWrite(LED_BUILTIN, HIGH);s
  // }

  // if (overflow){

  //   OCR2A = pgm_read_byte_near(a1_wav + i);
  //   if(i == a1_wav_len - 1){
  //     i = 0;
  //     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //   } 
  //   else i++;

  //   // uint8_t val = 128 + 127 * sin(i);
  //   // OCR2A = val;
  //   // i += 6.28/8;
  //   overflow = false;
  //   // digitalWrite(LED_BUILTIN, LOW); 
  // }


}

void setup() {
  // cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF); 

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SPEAKER, OUTPUT);

  // Timer2 -- 62.5 kHz PWM
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  TIMSK2 |= _BV(TOIE2);
  interrupts();



}

// the loop function runs over and over again forever
void loop() {
  // space check

  for(unsigned int i = 0; i < a1_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(a1_wav + i);
  }
  for(unsigned int i = 0; i < a1s_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(a1s_wav + i);
  }
  for(unsigned int i = 0; i < b1_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(b1_wav + i);
  }
  for(unsigned int i = 0; i < c1_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(c1_wav + i);
  }
  for(unsigned int i = 0; i < c1s_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(c1s_wav + i);
  }
  for(unsigned int i = 0; i < d1_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(d1_wav + i);
  }
  for(unsigned int i = 0; i < d1s_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(d1s_wav + i);
  }
  for(unsigned int i = 0; i < e1_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(e1_wav + i);
  }
  for(unsigned int i = 0; i < f1_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(f1_wav + i);
  }
  for(unsigned int i = 0; i < f1s_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(f1s_wav + i);
  }
  for(unsigned int i = 0; i < g1_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(g1_wav + i);
  }
  for(unsigned int i = 0; i < g1s_wav_len; i ++  ){
    OCR2A = pgm_read_byte_near(g1s_wav + i);
  }

}