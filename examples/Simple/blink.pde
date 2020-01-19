/*
  Blink for AVR Game board

  Turns a pixel on for one second, then off for one second, repeatedly.

*/

#include <AvrGameBoard.h>

void setup() {
  GameBoard.begin (); 
}

void loop() {
  GameBoard.matrix[0] = 1;
  GameBoard.show ();
  delay (1000);
  GameBoard.matrix[0] = 0;
  GameBoard.show ();
  delay (1000);
}