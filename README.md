# AVR-Game-board
Library for Arduino IDE for programming AVR Game board.
The library fully controls the use of the AVR Game board
1. Draw in led matrix
2. Draw in digital led
3. Read key state
4. Play sound

<h2>Draw in led matrix</h2>
Use the <b>GameBoard.matrix[0..15]</b> array to write new screen. 
The <b>GameBoard.matrix</b> array has 16 byte it coincides 16 rows of LEDs. The bottom row matches with GameBoard.matrix[0].
