#ifndef demos_h
#define demos_h

#include "Arduino.h"
//#include "AvrGameBoard.h"




uint8_t demoFire () {
  uint8_t mode = 1;
  uint8_t power = 7;
  uint8_t flame[GB_MATRIX_COLS];
  uint8_t spark[GB_MATRIX_COLS];
  uint8_t intensity = 10;
  
  GameBoard.clearKeyBuffer ();
  GameBoard.clearDigit ();
  GameBoard.stopMelody ();

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
        if (flame[i]==power-1) {
          if (spark[i]==0) {
            spark[i]=power+mode;
            flame[i]--;
          }
        }
      }
      if (r==2) {
        if (flame[i]<power) flame[i]++;
      }
    }
    
    for (uint8_t line=0; line < GB_MATRIX_LINES; line++) {
      uint8_t val = 0;
      uint8_t bt = 1;
      for (uint8_t i=0; i<GB_MATRIX_COLS; i++) {
        if (flame[i]+mode>line) val |= bt;
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

    uint8_t key = GameBoard.getPressedKey ();
    if (key & (GB_KEYCODE_RETURN | GB_KEYCODE_FIRE | GB_KEYCODE_LEFT | GB_KEYCODE_RIGHT)) return key;
    if ((key == GB_KEYCODE_UP) && (mode<5)) mode++;
    if ((key == GB_KEYCODE_DOWN) && (mode>0)) mode--;

    delay (30);
  }
}


uint8_t demoRandom () {
  uint8_t mode = 3;
  GameBoard.setMatrixIntensity(10);
  while (true) {
    for (uint8_t i=0; i<GB_MATRIX_LINES; i++) {
      GameBoard.matrix[i] = random (256);
      for (uint8_t m = mode; m<5; m++) GameBoard.matrix[i] &= random (256);
      if (mode==6) GameBoard.matrix[i] |= random (256);
    }
    GameBoard.show ();
    
    uint8_t key = GameBoard.getPressedKey ();
    if (key & (GB_KEYCODE_RETURN | GB_KEYCODE_FIRE | GB_KEYCODE_LEFT | GB_KEYCODE_RIGHT)) return key;
    if ((key == GB_KEYCODE_UP) && (mode<6)) mode++;
    if ((key == GB_KEYCODE_DOWN) && (mode>0)) mode--;
    delay (30);
  }
  
}

uint8_t demoMatrix () {
  uint8_t mode = 3;
  uint8_t ledCount = GB_MATRIX_COLS*4;
  GameBoard.setMatrixIntensity(10);
  uint8_t leds[ledCount];
  
  for (uint8_t i=0; i<ledCount; i++) leds[i]=0xff;
  
  while (true) {
    for (uint8_t i=0; i<ledCount; i++) {
      if (leds[i]==0xff) {
        if (random(30) < 1) {
          int8_t c = random(GB_MATRIX_COLS);
          uint8_t e = 0;
          for (uint8_t j=0; j<ledCount; j++) {
            int8_t lx = leds[j]>>4;
            int8_t ly = leds[j] & 0x0f;
            if ((lx>=c-(6-mode)) && (lx<=c+(6-mode))) {
              if (ly > GB_MATRIX_LINES-14+mode*2) e++;
            }
          }
          if (e==0) leds[i]=(c<<4) | (GB_MATRIX_LINES-1);
        }
      }
      
    }
    GameBoard.clearMatrix ();
    for (uint8_t i=0; i<ledCount; i++) {
      if (leds[i]!=0xff) {
        GameBoard.matrix[leds[i]&0x0f] = 0x80 >> (leds[i]>>4);
        if ((leds[i] & 0x0f) == 0) leds[i] = 0xff;
        else leds[i]--;
      }     
    }
    
    GameBoard.show ();
       
    uint8_t key = GameBoard.getPressedKey ();
    if (key & (GB_KEYCODE_RETURN | GB_KEYCODE_FIRE | GB_KEYCODE_LEFT | GB_KEYCODE_RIGHT)) return key;
    if ((key == GB_KEYCODE_UP) && (mode<5)) mode++;
    if ((key == GB_KEYCODE_DOWN) && (mode>1)) mode--;
    delay (30);
  }
}



#endif
