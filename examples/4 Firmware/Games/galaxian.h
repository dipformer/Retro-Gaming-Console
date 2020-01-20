
#ifndef galaxian_h
#define galaxian_h


const uint8_t galaxian_Anime_PGM[] PROGMEM = {
  0xe0, 0x40, 0x00, 0x00, 0x00, 0x10, 0x02, 0x00,
  0x38, 0x10, 0x00, 0x00, 0x10, 0x02, 0x00, 0x08,
  0x38, 0x10, 0x00, 0x00, 0x02, 0x00, 0x08, 0x00,
  0x07, 0x02, 0x00, 0x02, 0x00, 0x08, 0x00, 0x00,
  0x07, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x40,
  0x1c, 0x08, 0x00, 0x08, 0x00, 0x00, 0x40, 0x00,
  0x1c, 0x08, 0x00, 0x00, 0x00, 0x40, 0x00, 0x10,
  0xe0, 0x40, 0x00, 0x00, 0x40, 0x00, 0x10, 0x02
};

const uint16_t galaxianSound_Fire_PGM [] PROGMEM = { NOTE_C|OCT_5|DUR_32, NOTE_E|OCT_5|DUR_32, NOTE_G|OCT_5|DUR_32,  MELODY_END};
const uint16_t galaxianSound_Bang_PGM [] PROGMEM = { NOTE_E|OCT_2|DUR_16, MELODY_END};
const uint16_t galaxianSound_Fall_PGM [] PROGMEM = {NOTE_C|OCT_4|DUR_32, NOTE_B|OCT_3|DUR_32, NOTE_A|OCT_3|DUR_32, NOTE_F|OCT_3|
                DUR_16, NOTE_E|OCT_3|DUR_16, NOTE_D|OCT_3|DUR_8, NOTE_C|OCT_3|DUR_8, NOTE_D|OCT_3|DUR_4, MELODY_END};

const uint16_t galaxianShip_PGM [] PROGMEM = {0xe0, 0x40};

uint8_t galaxianRun () {
  uint16_t score = 0;
  uint8_t shipPosX = 3;
  uint8_t maxShipPosX = 5;
  int16_t timer = 0;
  int16_t stepMillis = 500;

  uint8_t space[GB_MATRIX_LINES];
  uint16_t balls[GB_MATRIX_LINES];

  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) balls[i] = 0;
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) space[i] = 0;

  while (true) {

    if (space[1] != 0) {
      GameBoard.playMelody (galaxianSound_Fall_PGM);
      return 0;
    }

    for (uint8_t i=0; i<GB_MATRIX_LINES; i++) {
      uint8_t b = 0x80 >> (balls[i] & 0x07);
      uint8_t y = balls[i] >> 8;
      if (space[y] & b) {
        space[y] = space[y] & (~b);
        balls[i] = 0;
        score++;
        stepMillis-=1;
        GameBoard.playMelody (galaxianSound_Bang_PGM);
      }
    }   
    for (uint8_t i=0; i<GB_MATRIX_LINES; i++) {
      if (balls[i]!=0) {
        balls[i] += 0x100;
        if ((balls[i] & 0xf00) == 0) balls[i] = 0;
      }
    }

    uint8_t key = GameBoard.getPressedKey ();
    if (key==GB_KEYCODE_RETURN) {
      return key;
    }
    if (key==GB_KEYCODE_LEFT) {
      if (shipPosX>0) shipPosX--;
    }
    if (key==GB_KEYCODE_RIGHT) {
      if (shipPosX<maxShipPosX) shipPosX++;
    }
    if (key==GB_KEYCODE_FIRE) {
      for (uint8_t i=0; i<GB_MATRIX_LINES; i++) {
        if (balls[i]==0) {
          balls[i] = 0x20 | (shipPosX+1);
          GameBoard.playMelody (galaxianSound_Fire_PGM);
          break;
        }
      }
      
    }    
    // CALCULATE
    if (timer >= stepMillis) {
      for (uint8_t i=0; i<GB_MATRIX_LINES-1; i++) {
        space[i]=space[i+1];
      }
      space[GB_MATRIX_LINES-1] = random (256) & 0x7e;
      timer = 0;
   }

    uint8_t fail = 0;
    GameBoard.clearMatrix ();
    for (uint8_t i=0; i<GB_MATRIX_LINES; i++) {
      GameBoard.matrix[i] = space[i];
    }    

    for (uint8_t i=0; i<2; i++) {
      uint8_t b = pgm_read_byte (&galaxianShip_PGM[i]);
      b >>= shipPosX;
      if ((GameBoard.matrix[i] & b) != 0) fail = 1;
      GameBoard.matrix[i] |= b;
    }
    for (uint8_t i=0; i<GB_MATRIX_LINES; i++) {
      if (balls[i]!=0) {
        uint8_t b = 0x80 >> (balls[i] & 0x07);
        uint8_t y = balls[i] >> 8;
        GameBoard.matrix[y] |= b;
      }
    }
    GameBoard.printNumberToDigit (score);     
    GameBoard.show ();
   
    delay (10);
    timer+=10;
  }
}

#endif
