# AVR-Game-board
Library for Arduino IDE for programming AVR Game board.
The library fully controls the use of the AVR Game board
1. Draw in led matrix
2. Draw in digital led
3. Read key state
4. Play sound

<h2>Draw in led matrix</h2>
Use the <b>GameBoard.matrix[0..15]</b> array to write new screen. The <b>GameBoard.matrix</b> array has 16 byte it coincides with 16 rows of LEDs. The bottom row matches with <b>GameBoard.matrix[0]</b>. Each row has 8 LEDs, it matches 8 bits of one byte of the array. To turn on the right LED, write the value 0x01 to the array item. Write the value 0x80 to turn on the left LED.</b>
