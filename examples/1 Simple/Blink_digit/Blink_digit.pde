/*
  Blink for AVR Game board

  Turns on a segment on for one second, then off for one second, repeatedly.

*/

#include <AvrGameBoard.h>

void setup() {
  GameBoard.begin (); 
}

void loop() {
  GameBoard.digit[0] = 1;
  GameBoard.show ();
  delay (1000);
  GameBoard.digit[0] = 0;
  GameBoard.show ();
  delay (1000);
}