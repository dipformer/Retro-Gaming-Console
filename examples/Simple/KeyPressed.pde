
/*
  Keys test for AVR Game board

  Set pixels from a pressed key.

*/

#include <AvrGameBoard.h>

const uint16_t melody[] PROGMEM = { 
  NOTE_C|OCT_5|DUR_32, MELODY_END
};

int key;

void setup() {
  GameBoard.begin (); 
}

void loop() {
  key = GameBoard.getPressedKey ();
  if (key != 0) {
    GameBoard.matrix[0] = key;
    GameBoard.show ();
    GameBoard.playMelody (melody);
  }
}
