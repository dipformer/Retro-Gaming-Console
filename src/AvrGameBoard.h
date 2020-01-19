#ifndef AVRGAMEBOARD_H
#define AVRGAMEBOARD_H

#include "Arduino.h"

// AVR Game Board defines

#define GB_MATRIX_COUNT 2
#define GB_DIGIT_COUNT 3
#define GB_MATRIX_LINES GB_MATRIX_COUNT * 8
#define GB_MATRIX_COLS 8
#define GB_MAX_INTENSITY 15

#define GB_KEYCODE_NOT_PRESSED 0x00
#define GB_KEYCODE_RETURN 0x20
#define GB_KEYCODE_LEFT 0x01
#define GB_KEYCODE_RIGHT 0x02
#define GB_KEYCODE_UP 0x04
#define GB_KEYCODE_DOWN 0x08
#define GB_KEYCODE_FIRE 0x10


#define GB_MATRIX_EFFECT_LINES 1
#define GB_MATRIX_EFFECT_LEFT 2
#define GB_MATRIX_EFFECT_RIGHT 3




//SOUND defines:

#define MELODY_DEF_TEMPO 25 // ms for DUR_d64


#define NOTE_PAUSE 0x0000
#define NOTE_C 0x0001  // до
#define NOTE_Cm 0x0002
#define NOTE_Db 0x0002
#define NOTE_D 0x0003  // ре
#define NOTE_Dm 0x0004
#define NOTE_Eb 0x0004
#define NOTE_E 0x0005  // ми
#define NOTE_F 0x0006  // фа
#define NOTE_Fm 0x0007
#define NOTE_Gb 0x0007
#define NOTE_G 0x0008  // соль
#define NOTE_Gm 0x0009
#define NOTE_Ab 0x0009
#define NOTE_A 0x000a   // ля
#define NOTE_Am 0x000b
#define NOTE_Bb 0x000b
#define NOTE_B 0x000c  // си

#define OCT_8 0x0000
#define OCT_7 0x0010
#define OCT_6 0x0020
#define OCT_5 0x0030  // second octave
#define OCT_4 0x0040  // first octave
#define OCT_3 0x0050
#define OCT_2 0x0060
#define OCT_1 0x0070
#define OCT_0 0x0080

#define DUR_64 0x0000
#define DUR_32 0x0100
#define DUR_32P 0x0200
#define DUR_16 0x0300
#define DUR_16P 0x0400
#define DUR_8 0x0500
#define DUR_8P 0x0600
#define DUR_4 0x0700
#define DUR_4P 0x0800
#define DUR_2 0x0900
#define DUR_2P 0x0a00
#define DUR_1 0x0b00
#define DUR_1P 0x0c00
#define DUR_F2 0x0d00
#define DUR_F2P 0x0e00
#define DUR_F4 0x0f00


#define MELODY_END 0xffff
#define MELODY_REPEAT 0xfffe


class CGameBoard {
  public:
  CGameBoard () {};
  void begin ();

  void playMelody (const uint16_t * buf);
  void playMelody (const uint16_t * buf, uint8_t tempo);
  void stopMelody ();

  void setDigitIntensity (uint8_t v) {digitIntensity = v;};
  void clearDigit ();
  void setDigitChar (uint8_t n, uint8_t ch); // set char to position n
  void printNumberToDigit (uint16_t num);  // print number to digits
  
  void setMatrixIntensity (uint8_t v) {matrixIntensity = v;};
  void clearMatrix ();
  uint8_t printNumberToMatrix (uint8_t n, uint8_t x, uint8_t y);
  void startEffect (uint8_t effect);
  
  void show ();

  uint8_t getPressedKey ();
  void clearKeyBuffer ();
  uint8_t getKeysState ();

  private:
  void loadMax7219Registers (uint8_t reg, uint8_t mValue, uint8_t dValue); 
  void showEffect ();
  
  public:
  uint8_t matrix[GB_MATRIX_LINES];
  uint8_t digit[GB_DIGIT_COUNT];

  private:
  uint8_t matrixIntensity;
  uint8_t digitIntensity;
  uint8_t copyMatrix[GB_MATRIX_LINES];
  uint8_t matrixEffect;
  

};


extern CGameBoard GameBoard;


#endif // AVRGAMEBOARD_H
