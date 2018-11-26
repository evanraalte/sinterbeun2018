#include <Arduino.h>
#include <CapacitiveSensor.h>
#include <notes.h>
#include <math.h>


// #define VERBOSE

//19 and 13 for LED's (maybe a battery indicator on pin A6 or A7)

const size_t NUM_SENSORS = 12;
CapacitiveSensor cs_2[NUM_SENSORS] = {
  CapacitiveSensor(2,3),
  CapacitiveSensor(2,4),
  CapacitiveSensor(2,5),
  CapacitiveSensor(2,6),
  CapacitiveSensor(2,7),
  CapacitiveSensor(2,8),
  CapacitiveSensor(2,9),
  CapacitiveSensor(2,10),
  CapacitiveSensor(18,14),
  CapacitiveSensor(18,15),
  CapacitiveSensor(18,16),
  CapacitiveSensor(18,17),
};



#define PRESS_THRESHOLD 20

#define SHORTEST_NOTE 2000


#define BATTERYLED 13
#define SPEAKER 11
#define DELAY  (1*1000000)/8000

// volatile float i = 0;
volatile bool overflow = false;
volatile uint8_t overflowCnt = 0;

volatile uint32_t adcCount = 0;
volatile bool measureAdc = true;

// volatile uint8_t val = 0;
// volatile uint16_t i = 0;

volatile bool play[NUM_SENSORS] = {false};
volatile uint16_t playCnt[NUM_SENSORS] = {0}; 

const uint8_t* adresses[] = {c1_wav,c1s_wav,d1_wav,d1s_wav,e1_wav,f1_wav,f1s_wav,g1_wav,g1s_wav,a1_wav,a1s_wav,b1_wav}; //check if this works..
const unsigned int lenghts[] = {c1_wav_len,c1s_wav_len,d1_wav_len,d1s_wav_len,e1_wav_len,f1_wav_len,f1s_wav_len,g1_wav_len,g1s_wav_len,a1_wav_len,a1s_wav_len,b1_wav_len};

SIGNAL(TIMER2_OVF_vect){
  overflowCnt++;
  if(overflowCnt >= 8){
    overflow = true;
    overflowCnt = 0;
    // digitalWrite(LED_BUILTIN, HIGH);s
  }
  adcCount++;
  if(adcCount >= 62500*10){
    adcCount = 0;
    measureAdc = true;
  }
}


void setup() {
  #ifdef VERBOSE
  Serial.begin(9600);
  #endif


  for(uint8_t i = 0; i < NUM_SENSORS; i++ ){
    cs_2[i].set_CS_AutocaL_Millis(0xFFFFFFFF); 
  }

  pinMode(BATTERYLED, OUTPUT);
  pinMode(SPEAKER, OUTPUT);

  // Timer2 -- 62.5 kHz PWM and 7.81 kHz sample update
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  TIMSK2 |= _BV(TOIE2);
  interrupts();



}

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
        if(playCnt[j] < lenghts[j] ){
          playCnt[j]++; //sumTone +=
           sumTone += pgm_read_byte_near(adresses[j] + playCnt[j]);//pgm_read_byte_near(adresses[j] + playCnt[j]);
        } else {
          play[j] = false;
          playCnt[j] = 0;
        }
      } else {
        playCnt[j] = 0;
      }
    }   

    //map sumtone to a smaller  variable
    OCR2A = (uint8_t) map(sumTone, 0, 255*NUM_SENSORS, 0, 255); //how many keys can you press at once? maybe this is too pessimistic

    overflow = false;
  }

  if(measureAdc){
    //0-255 <-> 0V-5V 0.02V resolution
    //Low battery sign at 3.2V (160)
    uint8_t voltage = analogRead(A6);

    #ifdef VERBOSE
    float v = voltage*0.02;
    print("Voltage: ");
    println(v);
    #endif
    
    if(voltage <= 160){
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
    measureAdc = false;
  }
}