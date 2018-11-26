#include <Arduino.h>
#include <CapacitiveSensor.h>
#include <notes.h>
#include <math.h>


// CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2); 
// CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);

const size_t NUM_SENSORS = 12;
CapacitiveSensor cs_2[NUM_SENSORS] = {
  CapacitiveSensor(4,5),
  CapacitiveSensor(4,6),
  CapacitiveSensor(4,7),
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

#define PRESS_THRESHOLD 20

// #define SHORTEST_NOTE 2000

#define SPEAKER 11
#define DELAY  (1*1000000)/8000

// volatile float i = 0;
volatile bool overflow = false;
volatile uint8_t overflowCnt = 0;
volatile uint8_t val = 0;
volatile uint16_t i = 0;

volatile bool play[NUM_SENSORS] = {false};
volatile uint16_t playCnt[NUM_SENSORS] = {0}; 

const uint8_t* adresses[] = {c1_wav,c1s_wav,d1_wav,d1s_wav,e1_wav,f1_wav,f1s_wav,g1_wav,g1s_wav,a1_wav,a1s_wav,b1_wav}; //check if this works..

SIGNAL(TIMER2_OVF_vect){
  overflowCnt++;
  if(overflowCnt >= 8){
    overflow = true;
    overflowCnt = 0;
    // digitalWrite(LED_BUILTIN, HIGH);s
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
  long keys[NUM_SENSORS] = {  0 }; 
  static bool pressed[NUM_SENSORS] = { false};
  static bool pressed_old[NUM_SENSORS] = { false};
  
  // play[0] = true;
  for(uint8_t i = 0; i < NUM_SENSORS; i++ ){
    keys[i] = cs_2[i].capacitiveSensor(3);
    pressed_old[i] = pressed[i];
    pressed[i] = (keys[i] > PRESS_THRESHOLD)?true:false;
    noInterrupts();
    if (pressed[i] && !pressed_old[i]){
      play[i] = true;
      playCnt[i] = 0;
    }
    interrupts();
    // Serial.print(play[i]);
    // Serial.print('\t');
  }
  // Serial.println();
  if (overflow){
    uint16_t sumTone = 0;

    for(uint16_t j = 0; j < NUM_SENSORS; j++){
      if(play[j]){
        if(playCnt[j] < 2400){
          playCnt[j]++; //sumTone +=
           sumTone += pgm_read_byte_near(adresses[j] + playCnt[j]);//pgm_read_byte_near(adresses[j] + playCnt[j]);
        } else {
          play[j] = false;
          playCnt[j] = 0;
        }
      } else {
        playCnt[j] = 0;
      }
      // Serial.print(play[j]);
      // Serial.print(':');
      // Serial.print(playCnt[j]);
      // Serial.print('\t');
    }
    // Serial.println();
    

    //map sumtone to a smaller  variable
    OCR2A = (uint8_t) map(sumTone, 0, 255*NUM_SENSORS, 0, 255); //how many keys can you press at once? maybe this is too optimistic

    // OCR2A = pgm_read_byte_near(a1_wav + i);
    // if(i == a1_wav_len - 1){
    //   i = 0;
    //   digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // } 
    // else i++;


    overflow = false;
    // digitalWrite(LED_BUILTIN, LOW); 
  }


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