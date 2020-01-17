
#include "AvrGameBoard.h"

// AVR Game Board pins

#define GB_PIN_BUZZER 4 
#define GB_BIT_BUZZER 4   
#define GB_PORT_BUZZER PORTD

#define GB_PIN_LEDS_DATA 5
#define GB_PIN_LEDS_CLK  6
#define GB_PIN_LEDS_LOAD 7

#define GB_PIN_BUTTON_RETURN A0
#define GB_PIN_BUTTON_LEFT   A1
#define GB_PIN_BUTTON_RIGHT  A2
#define GB_PIN_BUTTON_UP     A3
#define GB_PIN_BUTTON_DOWN   A4
#define GB_PIN_BUTTON_FIRE   A5


// MAX7219 defines

#define MAX7219_REG_NoOp        0x00
#define MAX7219_REG_Digit0      0x01
#define MAX7219_REG_DecodeMode  0x09
#define MAX7219_REG_Intensity   0x0a
#define MAX7219_REG_ScanLimit   0x0b
#define MAX7219_REG_Shutdown    0x0c
#define MAX7219_REG_DisplayTest 0x0f

// key reader  defines

#define GB_KEY_BUFFER_SIZE 8
#define GB_KEY_FILTER_STEPS 100

// system defines

#define GB_ISR_FREQUENSY 20000
#define GB_ISR_MICROSECONDS 1000000/GB_ISR_FREQUENSY
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))
#define INVR(x,y) (x^=(1<<y))

CGameBoard GameBoard;

const uint16_t notesFreq_PGM[] PROGMEM = { 0, 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902 };  // 5th octave
const uint8_t notesDurations_PGM[] PROGMEM = { 0, 1, 2, 3, 5, 7, 11, 15, 23, 31, 47, 63, 111, 127, 239, 255};  

uint16_t * buzzerBuf = 0;
uint8_t buzzerTempo = 0;
uint32_t buzzerTimeCnt=0;
uint32_t buzzer_cnt;
uint32_t buzzer_comp;

const uint8_t keysToPins_Codes_PGM[] PROGMEM = { GB_KEYCODE_RETURN, GB_KEYCODE_LEFT, GB_KEYCODE_RIGHT, GB_KEYCODE_UP, GB_KEYCODE_DOWN, GB_KEYCODE_FIRE };  
const uint8_t keysToPins_Pins_PGM[] PROGMEM = { GB_PIN_BUTTON_RETURN, GB_PIN_BUTTON_LEFT, GB_PIN_BUTTON_RIGHT, GB_PIN_BUTTON_UP, GB_PIN_BUTTON_DOWN, GB_PIN_BUTTON_FIRE };  

const uint8_t digit_Chars_PGM[] PROGMEM = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x01, 0x4f, 0x37, 0x0e, 0x67, 0x00};
#define DIGIT_CHAR_NO 0x0f

const uint8_t matrix_Numbers_PGM[] PROGMEM = {
  0x1f, 0x11, 0x1f, //0
  0x12, 0x1f, 0x10, //1
  0x1d, 0x15, 0x17, //2
  0x15, 0x15, 0x1f, //3
  0x07, 0x04, 0x1f, //4
  0x17, 0x15, 0x1d, //5
  0x1f, 0x15, 0x1d, //6
  0x01, 0x01, 0x1f, //7
  0x1f, 0x15, 0x1f, //8
  0x17, 0x15, 0x1f //9
};


uint8_t keysCurKey = 0;
uint8_t keysState = 0;  // current keys state
uint8_t keysBuffer[GB_KEY_BUFFER_SIZE]; 
uint8_t keysBufferPos = 0;
uint8_t keysBufferLook = 0;

int8_t keysFilters[6] = {0, 0, 0, 0, 0, 0};



void CGameBoard::begin () {
  pinMode(GB_PIN_BUZZER, OUTPUT);

  pinMode(GB_PIN_LEDS_DATA, OUTPUT);
  pinMode(GB_PIN_LEDS_CLK, OUTPUT);
  pinMode(GB_PIN_LEDS_LOAD, OUTPUT);

  pinMode(GB_PIN_BUTTON_RETURN, INPUT);
  pinMode(GB_PIN_BUTTON_LEFT, INPUT);
  pinMode(GB_PIN_BUTTON_RIGHT, INPUT);
  pinMode(GB_PIN_BUTTON_UP, INPUT);
  pinMode(GB_PIN_BUTTON_DOWN, INPUT);
  pinMode(GB_PIN_BUTTON_FIRE, INPUT);

  digitalWrite(GB_PIN_LEDS_LOAD, HIGH); 
  digitalWrite(GB_PIN_LEDS_CLK, HIGH);
  digitalWrite(GB_PIN_LEDS_DATA, HIGH); 

  
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) matrix[i]=0; 
  matrixIntensity = 0xf;
  for (uint8_t i=0; i<GB_DIGIT_COUNT; i++) digit[i]=0; 
  digitIntensity = 0xf;
  matrixEffect = 0;

  //for (uint8_t i=0; i<GB_KEY_FILTER_STEPS; i++) keysFilter[i]=0; 

  // enable interrupt TIMER1_COMPA_vect
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11);
  TIMSK1 |= (1 << OCIE1A);
  OCR1A = 2000000/GB_ISR_FREQUENSY;  
  TCNT1 = 0;

  
  loadMax7219Registers (MAX7219_REG_DecodeMode, 0x00, 0x00);  
  loadMax7219Registers (MAX7219_REG_ScanLimit, 0x07, GB_DIGIT_COUNT-1);      
  loadMax7219Registers (MAX7219_REG_Shutdown, 0x01, 0x01);    
  loadMax7219Registers (MAX7219_REG_DisplayTest, 0x00, 0x00); 
  ledsUpdate ();

}

void CGameBoard::playMelody (const uint16_t * buf) {
  playMelody (buf, MELODY_DEF_TEMPO);
}

void CGameBoard::playMelody (const uint16_t * buf, uint8_t tempo) {
  buzzerTempo = tempo;
  buzzerBuf = buf;
  buzzerTimeCnt = 0;
}

void CGameBoard::stopMelody () {
  buzzerBuf = 0;
  buzzerTimeCnt = 0;
}


void CGameBoard::setDigitChar (uint8_t n, uint8_t ch) {
  if (n<GB_DIGIT_COUNT) {
    digit[n] = pgm_read_byte (&digit_Chars_PGM[ch]);
  }
}

void CGameBoard::printNumberToDigit (uint16_t num) {
  uint8_t i = 0;
  while (i<GB_DIGIT_COUNT) {
    if ((num>0)||(i==0)) {
      setDigitChar (i, num % 10);
      num /= 10;
    }
    else setDigitChar (i, DIGIT_CHAR_NO);
    i++;
  }
}

void CGameBoard::clearDigit () {
  for (uint8_t i=0; i<GB_DIGIT_COUNT; i++) {
    digit[i]=0;
  }
}



void CGameBoard::clearMatrix () {
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) matrix[i] = 0;  
}



uint8_t CGameBoard::printNumberToMatrix (uint8_t n, uint8_t x, uint8_t y) {
  uint8_t * p = &matrix_Numbers_PGM[n*3];
  for (uint8_t i=0; i<3; i++) {
    uint8_t mx = x+i;
    if (mx<GB_MATRIX_COLS) {
      uint8_t b = pgm_read_byte (p);
      uint8_t bt = 0x10;
      for (uint8_t j=0; j<5; j++) {
        uint8_t my = y+j;
        if (my<GB_MATRIX_LINES) {
          if (b & bt) {
            matrix[my] |= (0x80>>mx);
          }       
        }
        bt>>=1;
      }
    }
    p++;
  }
  return x+4;
}


void CGameBoard::startEffect (uint8_t effect) {
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) copyMatrix[i] = matrix[i]; 
  matrixEffect = effect;
}

void CGameBoard::showEffect () {
  uint8_t effect = matrixEffect;
  uint8_t newMatrix[GB_MATRIX_LINES];
  matrixEffect = 0;
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) newMatrix[i] = matrix[i]; 
  if (effect==GB_MATRIX_EFFECT_LINES) {
    for (uint8_t i=0; i<GB_MATRIX_LINES; i++) matrix[i] = copyMatrix[i]; 
    for (uint8_t i=0; i<(GB_MATRIX_LINES>>1); i++) {
      matrix[i]=0xff;
      matrix[GB_MATRIX_LINES-i-1]=0xff;
      ledsUpdate ();
      delay (30);
    }   
    for (int8_t i=(GB_MATRIX_LINES>>1)-1; i>=0; i--) {
      matrix[i]=newMatrix[i];
      matrix[GB_MATRIX_LINES-i-1]=newMatrix[GB_MATRIX_LINES-i-1];
      ledsUpdate ();
      delay (30);
    }
  }
  else if (effect==GB_MATRIX_EFFECT_LEFT) {
    for (uint8_t c=0; c<GB_MATRIX_COLS; c++) {
      for (uint8_t i=0; i<GB_MATRIX_LINES; i++) matrix[i] = (copyMatrix[i]>>c) | (newMatrix[i]<<(7-c)); 
      ledsUpdate ();
      delay (30);      
    }
  }
  else if (effect==GB_MATRIX_EFFECT_RIGHT) {
    for (uint8_t c=0; c<GB_MATRIX_COLS; c++) {
      for (uint8_t i=0; i<GB_MATRIX_LINES; i++) matrix[i] = (copyMatrix[i]<<c) | (newMatrix[i]>>(7-c)); 
      ledsUpdate ();
      delay (30);      
    }
  }
  else ledsUpdate ();
}

void CGameBoard::ledsUpdate () {
  if (matrixEffect!=0) {
    showEffect ();
  }
  else {
    for (uint8_t dig = 0; dig<8; dig++) {
      digitalWrite(GB_PIN_LEDS_LOAD, LOW); 
      for (uint8_t i = 0; i<=GB_MATRIX_COUNT; i++) {
        uint8_t v;
        if (i==0) {
          if (dig<GB_DIGIT_COUNT) v=digit[dig]; 
          else v = 0;
        } 
        else {
          v = matrix[(i<<3) - dig - 1];
        }
        shiftOut(GB_PIN_LEDS_DATA, GB_PIN_LEDS_CLK, MSBFIRST, dig + MAX7219_REG_Digit0);
        shiftOut(GB_PIN_LEDS_DATA, GB_PIN_LEDS_CLK, MSBFIRST, v);  
       }
      digitalWrite(GB_PIN_LEDS_LOAD, HIGH);
    }
    loadMax7219Registers (MAX7219_REG_Intensity, matrixIntensity & 0xf, digitIntensity & 0xf);
  }
}


void CGameBoard::loadMax7219Registers (uint8_t reg, uint8_t mValue, uint8_t dValue) {
  digitalWrite(GB_PIN_LEDS_LOAD, LOW); 
  for (uint8_t i = 0; i<=GB_MATRIX_COUNT; i++) {
    shiftOut(GB_PIN_LEDS_DATA, GB_PIN_LEDS_CLK, MSBFIRST, reg);
    shiftOut(GB_PIN_LEDS_DATA, GB_PIN_LEDS_CLK, MSBFIRST, (i==0)?dValue:mValue);  
  }
  digitalWrite(GB_PIN_LEDS_LOAD, HIGH); 
}


uint8_t CGameBoard::getPressedKey () {
  if (keysBufferPos == 0) return 0;
  while (keysBufferLook!=0) delay (1);
  keysBufferLook++;   
  while (keysBufferLook!=1) delay (1);
  uint8_t key = keysBuffer[0];
  for (uint8_t i=1; i<GB_KEY_BUFFER_SIZE; i++) keysBuffer[i-1] = keysBuffer[i];
  keysBufferPos--;
  keysBufferLook--;
  return key;
}


void CGameBoard::clearKeyBuffer () { 
  keysBufferPos = 0;
}


ISR(TIMER1_COMPA_vect) {
  if (buzzerTimeCnt) {
    if (buzzer_comp!=0) {
      buzzer_cnt+=GB_ISR_MICROSECONDS;
      if (buzzer_cnt>=buzzer_comp) {
        INVR(GB_PORT_BUZZER, GB_BIT_BUZZER);
        buzzer_cnt-=buzzer_comp;
      }
    }
    buzzerTimeCnt--;
  }
  if ((buzzerTimeCnt==0) && (buzzerBuf!=0)) {
    uint16_t c = pgm_read_word (buzzerBuf++);
    if (c == MELODY_END) buzzerBuf=0;
    else {
      buzzer_cnt = 0;
      uint8_t note = c & 0xf;
      uint32_t dur = ((uint32_t)pgm_read_byte(notesDurations_PGM+((c & 0xf00) >> 8))+1)* buzzerTempo;
      if (note != 0) {
        uint32_t f = pgm_read_word (notesFreq_PGM+(note & 0xf));
        uint8_t oct = (c & 0xf0) >> 4;
        while (oct-- > 0) f=f>>1;
        buzzer_comp = 1000000 / 2 / f;   
      }
      else buzzer_comp = 0;
      buzzerTimeCnt = GB_ISR_FREQUENSY / 1000 * dur;
    }
  }
  if (buzzerTimeCnt==0) {
    CLR(GB_PORT_BUZZER, GB_BIT_BUZZER);
  }


  // keys
  uint8_t keyCode = pgm_read_byte (keysToPins_Codes_PGM+keysCurKey);
  uint8_t keyPin = pgm_read_byte (keysToPins_Pins_PGM+keysCurKey);
  int8_t * pf = keysFilters+keysCurKey;
  if (digitalRead (keyPin)==LOW) {
    if (*pf < 0) *pf = 0;
    (*pf)++;
    if (*pf > GB_KEY_FILTER_STEPS) {
      *pf = GB_KEY_FILTER_STEPS;
      if ((keysState & keyCode) == 0) { // press event
        if (keysBufferLook==0) {
          keysBufferLook++;
          if (keysBufferPos < GB_KEY_BUFFER_SIZE) {
            keysBuffer[keysBufferPos++] = keyCode;
          }
          keysBufferLook--;
          keysState = keyCode | keysState;
        }
      }
    }  
  }
  else {
    if (*pf > 0) *pf = 0;
    (*pf)--;
    if (*pf < -GB_KEY_FILTER_STEPS) {
      *pf = -GB_KEY_FILTER_STEPS;
      keysState = (~keyCode) & keysState; 
    }
  }
  
  keysCurKey++;
  if (keysCurKey>=6) keysCurKey = 0;
  
}


