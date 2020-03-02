#ifndef snake_h
#define snake_h


const uint8_t snake_Anime_PGM[] PROGMEM = {
  0x00, 0x00, 0x00, 0x04, 0xe0, 0x20, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x04, 0x20, 0x38, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x04, 0x04, 0x1c, 0x00, 0x00,
  0x00, 0x04, 0x04, 0x04, 0x24, 0x04, 0x00, 0x00,
  0x00, 0x1c, 0x04, 0x04, 0x20, 0x00, 0x00, 0x00,
  0x00, 0x7c, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
  0x00, 0xe0, 0x80, 0x80, 0x20, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xe0, 0x00, 0x00, 0x00
};

#define SNAKE_NO_RABBIT 0xff

const uint16_t snakeSound_Goal_PGM [] PROGMEM = {NOTE_F|OCT_4|DUR_32, NOTE_F|OCT_5|DUR_32, MELODY_END};

const uint16_t snakeMelody_PGM [] PROGMEM = { 
  NOTE_C|OCT_4|DUR_32, NOTE_B|OCT_3|DUR_32, NOTE_A|OCT_3|DUR_32, NOTE_F|OCT_3|DUR_32, 
  NOTE_E|OCT_3|DUR_32, NOTE_D|OCT_3|DUR_32, NOTE_C|OCT_3|DUR_32, NOTE_D|OCT_3|DUR_16,
  NOTE_PAUSE|DUR_1,
  NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_4|DUR_8P, NOTE_Fm|OCT_4|DUR_16, NOTE_A|OCT_4|DUR_8, NOTE_G|OCT_4|DUR_8, NOTE_E|OCT_4|DUR_4, 
  NOTE_D|OCT_4|DUR_8, NOTE_D|OCT_4|DUR_8, NOTE_D|OCT_5|DUR_8P, NOTE_C|OCT_5|DUR_16, NOTE_C|OCT_5|DUR_8, NOTE_B|OCT_4|DUR_8, NOTE_PAUSE|DUR_4,
  NOTE_D|OCT_5|DUR_8, NOTE_C|OCT_5|DUR_8, NOTE_A|OCT_4|DUR_8, NOTE_Fm|OCT_4|DUR_8, NOTE_C|OCT_5|DUR_8, NOTE_B|OCT_4|DUR_8, NOTE_B|OCT_4|DUR_4,
  NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_4|DUR_8P, NOTE_A|OCT_4|DUR_16, NOTE_A|OCT_4|DUR_8, NOTE_G|OCT_4|DUR_4, NOTE_PAUSE|DUR_4,
  NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_4|DUR_8P, NOTE_Fm|OCT_4|DUR_16, NOTE_A|OCT_4|DUR_8, NOTE_G|OCT_4|DUR_8, NOTE_E|OCT_4|DUR_4, 
  NOTE_D|OCT_4|DUR_8, NOTE_D|OCT_4|DUR_8, NOTE_D|OCT_5|DUR_8P, NOTE_C|OCT_5|DUR_16, NOTE_C|OCT_5|DUR_8, NOTE_B|OCT_4|DUR_8, NOTE_PAUSE|DUR_4,
  NOTE_E|OCT_5|DUR_8, NOTE_C|OCT_5|DUR_8, NOTE_A|OCT_4|DUR_8, NOTE_Fm|OCT_4|DUR_8, NOTE_C|OCT_5|DUR_8, NOTE_B|OCT_4|DUR_8, NOTE_B|OCT_4|DUR_4,
  NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_4|DUR_8P, NOTE_A|OCT_4|DUR_16, NOTE_A|OCT_4|DUR_8, NOTE_G|OCT_4|DUR_4, NOTE_PAUSE|DUR_4,
  NOTE_E|OCT_5|DUR_8, NOTE_C|OCT_5|DUR_8, NOTE_A|OCT_4|DUR_8, NOTE_Fm|OCT_4|DUR_8, NOTE_C|OCT_5|DUR_8, NOTE_B|OCT_4|DUR_8, NOTE_B|OCT_4|DUR_4,
  NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_4|DUR_8P, NOTE_Dm|OCT_4|DUR_16, NOTE_A|OCT_4|DUR_8, NOTE_G|OCT_4|DUR_4, NOTE_PAUSE|DUR_4,
  NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_3|DUR_8, NOTE_B|OCT_4|DUR_8P, NOTE_Fm|OCT_4|DUR_16, NOTE_A|OCT_4|DUR_8, NOTE_G|OCT_4|DUR_8, NOTE_E|OCT_4|DUR_4, 
  NOTE_D|OCT_4|DUR_8, NOTE_D|OCT_4|DUR_8, NOTE_D|OCT_5|DUR_8P, NOTE_C|OCT_5|DUR_16, NOTE_C|OCT_5|DUR_8, NOTE_B|OCT_4|DUR_8, NOTE_PAUSE|DUR_4,
  NOTE_E|OCT_5|DUR_8, NOTE_C|OCT_5|DUR_8, NOTE_A|OCT_4|DUR_8, NOTE_Fm|OCT_4|DUR_8, NOTE_C|OCT_5|DUR_8, NOTE_B|OCT_4|DUR_8, NOTE_E|OCT_5|DUR_4,
  NOTE_B|OCT_4|DUR_8, NOTE_B|OCT_4|DUR_8, NOTE_Cm|OCT_4|DUR_8, NOTE_Dm|OCT_4|DUR_8, NOTE_E|OCT_5|DUR_4, NOTE_E|OCT_5|DUR_4,
  MELODY_END
};

uint8_t snakeGetRabbit (uint8_t *field) {
  uint16_t freeCnt = 0;
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) {
    uint8_t bt = 0x80;
    for (uint8_t j=0; j<GB_MATRIX_COLS; j++) {
      if ((field[i] & bt) == 0) freeCnt++;
      bt>>=1;
    }
  }
  uint8_t r = random (freeCnt);
  uint16_t cnt = 0;
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) {
    uint8_t bt = 0x80;
    for (uint8_t j=0; j<GB_MATRIX_COLS; j++) {
      if ((field[i] & bt) == 0) {
        if (cnt==r) return i | (j<<4);
        cnt++;
      }
      bt>>=1;
    }
  }
  return 0xff;
}

void snakeFillField (uint8_t * snake, uint8_t head, uint8_t length, uint8_t maxLength, uint8_t *field) {
  uint8_t h = head;
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) field[i]=0;
  for (uint16_t i=0; i<length; i++) {
    uint8_t b = snake[h];
    field[b & 0x0f] |= 0x80>>(b>>4); 
    h++;
    if (h == maxLength) h=0;
  }  
}


uint8_t snakeRun () {

  uint16_t snakeMaxLength = 128;
  uint8_t field[GB_MATRIX_LINES];
  uint8_t snake[snakeMaxLength];
  uint8_t snakeHead;
  uint8_t snakeLength;
  uint8_t snakeDirection;
  uint8_t rabbit = SNAKE_NO_RABBIT;
  int16_t timer = 0;
  uint8_t rabbitTimer = 0;
  uint8_t rabbitStep = 0;
  int16_t stepMillis = 250;
  
  GameBoard.clearKeyBuffer ();
  GameBoard.stopMelody ();
  GameBoard.setMatrixIntensity (10);
  GameBoard.setDigitIntensity (15);
  
  snake[0] = 0x46;
  snake[1] = 0x45;
  snake[2] = 0x44;
  snakeHead = 0;
  snakeLength = 3;
  snakeDirection= GB_KEYCODE_UP;

  snakeFillField (snake, snakeHead, snakeLength, snakeMaxLength, field);
   
  while (true) {   
    if (rabbit == SNAKE_NO_RABBIT) {
      rabbit = snakeGetRabbit (field);
    }

    uint8_t key = GameBoard.getPressedKey ();
    if (key==GB_KEYCODE_RETURN) {
      return key;
    }
    if (key & (GB_KEYCODE_UP | GB_KEYCODE_DOWN | GB_KEYCODE_LEFT | GB_KEYCODE_RIGHT)) {
      if (((key | snakeDirection) != (GB_KEYCODE_UP | GB_KEYCODE_DOWN)) && ((key | snakeDirection) != (GB_KEYCODE_LEFT | GB_KEYCODE_RIGHT))) 
        snakeDirection = key;
        timer=stepMillis;
    }

    // CALCULATE
    if (timer >= stepMillis) {

      uint8_t fail = 0;
      uint8_t head = snake[snakeHead];
      if (snakeDirection == GB_KEYCODE_UP) {
        if ((head & 0x0f) == (GB_MATRIX_LINES-1)) fail=1;
        else head +=1;
      }
      if (snakeDirection == GB_KEYCODE_DOWN) {
        if ((head & 0x0f) == 0) fail=1;
        else head -=1;          
      }
      if (snakeDirection == GB_KEYCODE_LEFT) {
        if ((head & 0xf0) == 0) fail=1;
        else head -=0x10;          
      }
      if (snakeDirection == GB_KEYCODE_RIGHT) {
        if ((head>>4) == (GB_MATRIX_COLS-1)) fail=1;
        else head +=0x10;                
      }      
      if (field[head & 0x0f] & (0x80>>(head>>4))) fail = 1;
      if (fail) {
        GameBoard.playMelody (snakeMelody_PGM);
        return 0;
      }
      if (snakeHead>0) snakeHead--;
      else snakeHead = snakeMaxLength-1;
      snake[snakeHead] = head;
      if (head == rabbit) {
        snakeLength +=1;
        GameBoard.playMelody (snakeSound_Goal_PGM);
        rabbit = snakeGetRabbit (field);
        rabbitTimer = 0;
        rabbitStep=0;
      }
      snakeFillField (snake, snakeHead, snakeLength, snakeMaxLength, field);
      timer = 0;
      rabbitStep++;
      if (rabbitStep>10) {
        rabbitTimer=0;
        rabbitStep=0;
      }
    }      


    // SHOW SCENE
    for (uint8_t i=0; i<GB_MATRIX_LINES; i++) GameBoard.matrix[i] = field[i];
    if (((timer % 60) < 30) || (rabbitTimer>=stepMillis)) {
      GameBoard.matrix[rabbit & 0x0f] |= 0x80>>(rabbit>>4);
    }
    GameBoard.printNumberToDigit (snakeLength);     
    GameBoard.show ();

    delay (10);
    timer+=10;
    if (rabbitTimer<stepMillis) rabbitTimer+=10;
  }

}


#endif