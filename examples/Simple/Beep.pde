
/*
  Beep for AVR Game board

*/

#include <AvrGameBoard.h>

const uint16_t melody[] PROGMEM = { 
  NOTE_C|OCT_4|DUR_8, NOTE_PAUSE|DUR_8, 
  MELODY_REPEAT
};

void setup() {
  GameBoard.begin (); 
  GameBoard.playMelody (melody);
}

void loop() {
  // the melody plays in the background
}
