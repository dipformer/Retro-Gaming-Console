/*
  Tetris game for AVR Game board

*/

#include <AvrGameBoard.h>


#define TETRIS_FIGURE_COUNT 7
const uint16_t tetrisFigurs_PGM [TETRIS_FIGURE_COUNT][4] PROGMEM = {
  {0x0f00, 0x4444, 0x0f00, 0x4444},
  {0x0660, 0x0660, 0x0660, 0x0660},
  {0x0360, 0x4620, 0x0360, 0x4620},
  {0x0630, 0x2640, 0x0630, 0x2640},
  {0x0170, 0x4460, 0x0740, 0x6220},
  {0x0470, 0x6440, 0x0710, 0x2260},
  {0x0270, 0x4640, 0x0720, 0x2620}  
};

const uint16_t tetrisSound_Fall_PGM [] PROGMEM = {NOTE_F|OCT_5|DUR_32, MELODY_END};
const uint16_t tetrisSound_FullLine_PGM [] PROGMEM = {NOTE_C|OCT_5|DUR_32, NOTE_D|OCT_5|DUR_32, NOTE_E|OCT_5|DUR_32, NOTE_F|OCT_5|DUR_32, NOTE_G|OCT_5|DUR_32, NOTE_A|OCT_5|DUR_32, NOTE_B|OCT_5|DUR_32, MELODY_END};
const uint16_t tetrisSound_End_PGM [] PROGMEM = {NOTE_C|OCT_4|DUR_32, NOTE_B|OCT_3|DUR_32, NOTE_A|OCT_3|DUR_32, NOTE_F|OCT_3|DUR_32, 
NOTE_E|OCT_3|DUR_32, NOTE_D|OCT_3|DUR_32, NOTE_C|OCT_3|DUR_32, NOTE_D|OCT_3|DUR_16,MELODY_END};


void setup() {
  GameBoard.begin (); 
}

uint8_t tetrisGetFigure (uint8_t n, uint8_t ang, int8_t x, uint8_t * buf) {
  uint16_t f = pgm_read_word (&tetrisFigurs_PGM[n][ang]);
  buf[0]=(f<<2) & 0x3c;
  buf[1]=(f>>2) & 0x3c;
  buf[2]=(f>>6) & 0x3c;
  buf[3]=(f>>10) & 0x3c;
  uint8_t error = 0;
  for (uint8_t i=0; i<4; i++) {
    if (x<0) {
      for (uint8_t j=0; j<(-x); j++) {
        if (buf[i] & 0x80) error=1;
        buf[i] <<= 1;
      }
    }
    else if (x>0) {
      for (uint8_t j=0; j<x; j++) {
        if (buf[i] & 0x01) error=1;
        buf[i] >>= 1;
      }
    }   
  }
  return error;
}

uint8_t tetrisCheckFigureInGlass (uint8_t * glass, uint8_t * figure, int8_t y) {
  for (uint8_t i=0; i<4; i++) {
    int8_t line = y+i;
    if (line<0) {
      if (figure[i]!=0) return 1;
    }
    else if (line<GB_MATRIX_LINES) {
      if ((figure[i] & glass[line]) != 0) return 1;
    }
  }
  return 0;  
}


void loop () {

  uint16_t score = 0;
  uint8_t glass[GB_MATRIX_LINES];
  uint8_t figureNum;
  uint8_t figure[4];
  int8_t figurePosX;
  int8_t figurePosY;
  uint8_t figureAngle;
  int16_t timer;
  int16_t stepMillis;

  GameBoard.clearKeyBuffer ();
  GameBoard.stopMelody ();
  GameBoard.setMatrixIntensity (10);
  GameBoard.setDigitIntensity (15);
  
  for (uint8_t i=0; i<GB_MATRIX_LINES; i++) glass[i]=0;

    
  while (true) {

    // new figure
    figureNum = random (TETRIS_FIGURE_COUNT);
    figurePosX = 0;
    figurePosY = GB_MATRIX_LINES-3;
    figureAngle = 0;
    tetrisGetFigure (figureNum, figureAngle, figurePosX, figure);
    if (tetrisCheckFigureInGlass (glass, figure, figurePosY) != 0) {  // game over
      break;
    }

    timer = 0;
    stepMillis = 500;

    while (true) {

      uint8_t key = GameBoard.getPressedKey ();

      if (key & (GB_KEYCODE_FIRE | GB_KEYCODE_UP | GB_KEYCODE_LEFT | GB_KEYCODE_RIGHT)) {
        uint8_t new_figure[4];
        uint8_t new_figureAngle = figureAngle;
        uint8_t new_figurePosX = figurePosX;
      
        if (key==GB_KEYCODE_FIRE) {         
           if (new_figureAngle>=3) new_figureAngle=0;
           else new_figureAngle++;
        }
        if (key==GB_KEYCODE_UP) {           
           if (new_figureAngle==0) new_figureAngle=3;
           else new_figureAngle--;
        }
        if (key==GB_KEYCODE_LEFT) new_figurePosX--;
        if (key==GB_KEYCODE_RIGHT) new_figurePosX++;

        if (tetrisGetFigure (figureNum, new_figureAngle, new_figurePosX, new_figure) == 0) {  
          if (tetrisCheckFigureInGlass (glass, new_figure, figurePosY) == 0) {             
            for (uint8_t i=0; i<4; i++) figure[i]=new_figure[i];
            figureAngle = new_figureAngle;
            figurePosX = new_figurePosX;
          }
        }

      }
      if (key==GB_KEYCODE_DOWN) {
        stepMillis = 50;
        timer = 50;
      }
      if (key==GB_KEYCODE_RETURN) {
        break;
      }

      // CALCULATE
      if (timer >= stepMillis) {
        if (tetrisCheckFigureInGlass (glass, figure, figurePosY-1) == 0) {
          figurePosY--;          
        }
        else {
          for (uint8_t i=0; i<4; i++) {
            int8_t line = figurePosY+i;
            if ((line>=0) && (line<GB_MATRIX_LINES)) glass[line] |= figure[i];
          }
          for (int8_t i=GB_MATRIX_LINES-1; i>=0; i--) {
            if (glass[i] == 0xff) {
              // delete line
              GameBoard.playMelody (tetrisSound_FullLine_PGM);
              for (uint8_t j=0; j<8; j++) {
                delay (50);
                GameBoard.matrix[i] >>= 1;
                GameBoard.show ();
              }
              for (uint8_t j=i+1; j<GB_MATRIX_LINES; j++) {
                glass[j-1] = glass[j];
              }
              glass[GB_MATRIX_LINES-1] = 0;
              score++;
              GameBoard.printNumberToDigit (score);
            }
          }
          GameBoard.playMelody (tetrisSound_Fall_PGM);
          break;
        }
        timer = 0;

      }

      // SHOW SCENE
      for (uint8_t i=0; i<GB_MATRIX_LINES; i++) GameBoard.matrix[i]=glass[i];
      for (uint8_t i=0; i<4; i++) {
        int8_t line = figurePosY+i;
        if ((line>=0) && (line<GB_MATRIX_LINES)) GameBoard.matrix[line] |= figure[i];
      }
      GameBoard.printNumberToDigit (score);     
      GameBoard.show ();

      delay (10);
      timer+=10;
      
    }
    
  }

  // GAME OVER

  GameBoard.playMelody (tetrisSound_End_PGM);
  GameBoard.startEffect (GB_MATRIX_EFFECT_LINES);
  GameBoard.show ();
  delay (1000);
  GameBoard.clearKeyBuffer ();
  while (true) {
    GameBoard.clearDigit ();
    GameBoard.show ();
    delay (100);
    
    GameBoard.printNumberToDigit (score); 
    GameBoard.show ();
    delay (100);
       
    uint8_t key = GameBoard.getPressedKey ();
    if (key & (GB_KEYCODE_FIRE | GB_KEYCODE_RETURN)) break;
  }  
}