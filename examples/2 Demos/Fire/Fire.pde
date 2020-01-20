/*
  Fire for AVR Game board

*/

#include <AvrGameBoard.h>


const uint16_t melody1[] PROGMEM = { NOTE_C|OCT_1|DUR_32, MELODY_END};
const uint16_t melody2[] PROGMEM = { NOTE_E|OCT_1|DUR_32, MELODY_END};
const uint16_t melody3[] PROGMEM = { NOTE_G|OCT_1|DUR_32, MELODY_END};

void setup() {
  GameBoard.begin (); 
}

void loop () {

  uint8_t flame[GB_MATRIX_COLS];
  uint8_t spark[GB_MATRIX_COLS];
  uint8_t intensity = 10;
  
  for (uint8_t i=0; i<GB_MATRIX_COLS; i++) {
    flame[i]=0;
    spark[i]=0;
  }

  
  uint8_t r;
  while (true) {

 
    for (uint8_t i=0; i<GB_MATRIX_COLS; i++) {
      if (spark[i]>0) spark[i]++;
      if (spark[i]>=GB_MATRIX_LINES) spark[i]=0;
      
      r = random (3);
      if (r==0) {
        if (flame[i]>2) flame[i]--;
        if (flame[i]==6) {
          if (spark[i]==0) {
            spark[i]=8;
            flame[i]--;
            r = random (3);
            if (r==0) GameBoard.playMelody (melody1);
            if (r==1) GameBoard.playMelody (melody2);
            if (r==2) GameBoard.playMelody (melody3);
          }
        }
      }
      if (r==2) {
        if (flame[i]<7) flame[i]++;
      }
    }
    
    for (uint8_t line=0; line < GB_MATRIX_LINES; line++) {
      uint8_t val = 0;
      uint8_t bt = 1;
      for (uint8_t i=0; i<GB_MATRIX_COLS; i++) {
        if (flame[i]>=line) val |= bt;
        if (spark[i]==line) val |= bt;      
        bt <<=1;
      }
      GameBoard.matrix[line] = val;
    }

    r = random (3);
    if (r==0) {
      intensity++;
      if (intensity>GB_MAX_INTENSITY) intensity=GB_MAX_INTENSITY;
    }
    if (r==2) {
      intensity--;
      if (intensity==0) intensity=1;
    }
    
    GameBoard.setMatrixIntensity(intensity);
    GameBoard.show ();

    delay (30);
  }

}