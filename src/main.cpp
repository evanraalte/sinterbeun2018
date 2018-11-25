#include <Arduino.h>
#include <CapacitiveSensor.h>
#include <notes.h>
#include <math.h>


// CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2); 
// CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);

const size_t NUM_SENSORS = 12;
CapacitiveSensor cs_2[NUM_SENSORS] = {
  CapacitiveSensor(4,2),
  CapacitiveSensor(4,3),
  CapacitiveSensor(4,2),
  CapacitiveSensor(4,3),
  CapacitiveSensor(4,2),
  CapacitiveSensor(4,3),
  CapacitiveSensor(4,2),
  CapacitiveSensor(4,3),
  CapacitiveSensor(4,2),
  CapacitiveSensor(4,3),
  CapacitiveSensor(4,2),
  CapacitiveSensor(4,3),
};

// #define SHORTEST_NOTE 2000

#define SPEAKER 11
#define DELAY  (1*1000000)/8000

// volatile float i = 0;
volatile bool overflow = false;
volatile uint8_t overflowCnt = 0;
volatile uint8_t val = 0;
volatile uint16_t i = 0;

SIGNAL(TIMER2_OVF_vect){
  overflowCnt++;
  if(overflowCnt >= 8){
    overflow = true;
    overflowCnt = 0;
    // digitalWrite(LED_BUILTIN, HIGH);s
  }

  if (overflow){

    OCR2A = pgm_read_byte_near(a1_wav + i);
    if(i == a1_wav_len - 1){
      i = 0;
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    } 
    else i++;


    overflow = false;
    // digitalWrite(LED_BUILTIN, LOW); 
  }


}

void setup() {
  Serial.begin(9600);
  for(uint8_t i = 0; i < NUM_SENSORS; i++ ){
    cs_2[i].set_CS_AutocaL_Millis(0xFFFFFFFF); 
  }

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
  long keys[12] = {  0 }; 
  for(uint8_t i = 0; i < 12; i++ ){
    keys[i] = cs_2[i].capacitiveSensor(1);
    // if(keys[i] > 100) digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.print(keys[i]);
    Serial.print('\t');
  }
  Serial.println();
  // space check
  
  

  // for(unsigned int i = 0; i < a1_wav_len; i ++  ){
  //   OCR2A = 127; // pgm_read_byte_near(a1_wav + i);
  //   delayMicroseconds(DELAY);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(a1s_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(b1_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(c1_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(c1s_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(d1_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(d1s_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(e1_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(f1_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(f1s_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(g1_wav + i);
  // }
  // for(unsigned int i = 0; i < SHORTEST_NOTE; i ++  ){
  //   OCR2A = pgm_read_byte_near(g1s_wav + i);
  // }

}