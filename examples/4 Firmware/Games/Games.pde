#include <AvrGameBoard.h>

#include "demos.h"
#include "tetris.h"
#include "snake.h"
#include "galaxian.h"



#define GAMES_COUNT 3
uint8_t (*games[GAMES_COUNT])() = {tetrisRun, snakeRun, galaxianRun};
uint8_t * anime[GAMES_COUNT] = {tetris_Anime_PGM, snake_Anime_PGM, galaxian_Anime_PGM};


#define DEMOS_COUNT 3
uint8_t (*demos[DEMOS_COUNT])() = {demoFire, demoRandom, demoMatrix};

const uint16_t startSound_PGM [] PROGMEM = { 
  NOTE_Fm|OCT_4|DUR_16, NOTE_Fm|OCT_4|DUR_16, NOTE_Fm|OCT_4|DUR_16, NOTE_D|OCT_4|DUR_4, NOTE_PAUSE|DUR_16, 
  NOTE_E|OCT_4|DUR_16, NOTE_E|OCT_4|DUR_16, NOTE_E|OCT_4|DUR_16, NOTE_C|OCT_4|DUR_4, 
  MELODY_END
};

int8_t idDemo = 0;
int8_t idGame = 0;

void setup() {
  GameBoard.begin (); 
  GameBoard.playMelody (startSound_PGM);
}


void loop() {

  playDemos ();
 
  while (selectGame ()) {
    playGame (); 
  }
  
  GameBoard.startEffect (GB_MATRIX_EFFECT_LINES);
}


void  playDemos () {
  while (true) {
    uint8_t key = demos[idDemo]();
    if (key==GB_KEYCODE_LEFT) {
      idDemo = subRound (idDemo, DEMOS_COUNT);
      GameBoard.startEffect (GB_MATRIX_EFFECT_LEFT);
    }
    if (key==GB_KEYCODE_RIGHT) {
      idDemo = addRound (idDemo, DEMOS_COUNT);
      GameBoard.startEffect (GB_MATRIX_EFFECT_RIGHT);
    }
    if (key==GB_KEYCODE_FIRE) return;
    
  }
}

int8_t selectGame () {
  uint8_t frame = 0;
  GameBoard.stopMelody ();
  GameBoard.setMatrixIntensity (10);
  GameBoard.startEffect (GB_MATRIX_EFFECT_LINES);
  while (true) {
    GameBoard.clearMatrix ();
    GameBoard.clearDigit ();
    GameBoard.printNumberToMatrix (idGame+1, 3, 10);

    // animate
    uint8_t * p = anime[idGame] + ((frame>>4)&0x07)*8;
    for (uint8_t i=0; i<8; i++) {
      GameBoard.matrix[i]=pgm_read_byte (p++);      
    }
    GameBoard.show ();

    
    uint8_t key = GameBoard.getPressedKey ();
    if (key==GB_KEYCODE_LEFT) {
      idGame = subRound (idGame, GAMES_COUNT);
      GameBoard.startEffect (GB_MATRIX_EFFECT_LEFT);
    }
    if (key==GB_KEYCODE_RIGHT) {
      idGame = addRound (idGame, GAMES_COUNT);
      GameBoard.startEffect (GB_MATRIX_EFFECT_RIGHT);
    }
    if (key == GB_KEYCODE_FIRE) return 1;
    if (key == GB_KEYCODE_RETURN) return 0;

    frame++;
    delay (30);
  }
}

void playGame () {
  while (true) {
    GameBoard.startEffect (GB_MATRIX_EFFECT_LINES);
    uint8_t key = games[idGame]();
    if (key != GB_KEYCODE_RETURN) {
      key = gameOver ();
    }
    if (key == GB_KEYCODE_RETURN) break;
  }   
}

uint8_t gameOver () {
  GameBoard.startEffect (GB_MATRIX_EFFECT_LINES);
  GameBoard.show ();
  delay (1000);
  GameBoard.clearKeyBuffer ();
  while (true) {
    for (uint8_t i=GB_MAX_INTENSITY*2; i>0; i--) {
      if (i<GB_MAX_INTENSITY) GameBoard.setDigitIntensity (i);
      else GameBoard.setDigitIntensity (GB_MAX_INTENSITY*2-i);
      GameBoard.show ();
      delay (15);
      uint8_t key = GameBoard.getPressedKey ();
      if (key & (GB_KEYCODE_FIRE | GB_KEYCODE_RETURN)) return key;
    }
  }  
}
  


int8_t addRound (int8_t i, int8_t count) {
  i++;
  if (i>=count) i=0;
  return i;
}

int8_t subRound (int8_t i, int8_t count) {
  i--;
  if (i<0) i=count-1;
  return i;
}
