/*
  Random for AVR Game board

*/

#include <AvrGameBoard.h>

void setup() {
  GameBoard.begin (); 
}

void loop () {

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
    if ((key == GB_KEYCODE_UP) && (mode<6)) mode++;
    if ((key == GB_KEYCODE_DOWN) && (mode>0)) mode--;
    delay (30);
  }

}