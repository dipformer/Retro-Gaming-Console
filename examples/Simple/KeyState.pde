
/*
  Keys test for AVR Game board

  Set pixels from a pressed key.

*/

#include <AvrGameBoard.h>


int keys;

void setup() {
  GameBoard.begin (); 
}

void loop() {
  keys = GameBoard.getKeysState ();
  GameBoard.matrix[0] = keys;
  GameBoard.show ();
  delay (50);
}
