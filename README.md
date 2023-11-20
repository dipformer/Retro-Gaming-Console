﻿﻿# Библиотека для платы DIPFORMER Игровая ретро консоль
Плата **DIPFORMER Игровая ретро консоль** это Arduino совместимое устройство на микроконтроллере ATmega168 в форм-факторе игровой 
консоли. Плата разработана для обучения программированию игровых сценариев. 
Программирование платы осуществляется при помощи Arduino IDE и подключением библиотеки `AvrGameBoard.h`. Библиотека 
поддерживает все основные функции по работе с железом, при использовании библиотеки написание программного 
кода полностью сосредотачивается на логике игрового сценария, визуализации сцены.

Для формирования игрового сценария плата **DIPFORMER Игровая ретро консоль** предоставляет следующие модули, размещенные на плате:
- основной дисплей из светодиодной матрицы размером 8x16 точек;
- трехсимвольный семисегментный индикатор;
- 6 кнопок управления, включая четыре кнопки направления, кнопка FIRE, кнопка RETURN;
- зуммер для воспроизведения звуков.

Управление всеми перечисленными модулями реализовано в библиотеке для Arduino IDE. Поддержка перечисленных 
модулей разделена в библиотеке на три отдельные группы, которые являются совершенно независимыми:
- отображение;
- клавиатура;
- звук.

## Инициализация
Подключение библиотеки для **DIPFORMER Игровая ретро консоль** осуществляется в два этапа. Первый - это собственно команда 
препроцессора для подключения библиотеки `#include <AvrGameBoard.h>`. Второй - это в процедуре `setup()` 
инициализация класса библиотеки `GameBoard.begin ()`.

```
#include <AvrGameBoard.h>

void setup() {
  GameBoard.begin (); 
}

void loop() {
}
```


## Oтображение
Светодиодная матрица и семисегментный индикатор объединены в единую систему формирования изображения. 
Для формирования кадра изображения используются два массива байтов. Массив для светодиодной матрицы 
состоит из 16 байт `GameBoard.matrix[0..15]`. Массив для семисегментного индикатора состоит из 3-х байт 
`GameBoard.digit[0..2]`. Формирование кадра изображения сводится к заполнению этих массивов необходимыми 
данными. Сама по себе запись данных в массивы не изменяет отображаемых данных на дисплеях. Что бы заполненные 
данные кадра из обоих массивов отобразились, необходимо выполнить команду `GameBoard.show()`. Вы можете в 
произвольном порядке работать с указанными массивами, заполнять их необходимыми данными, стирать и перезаписывать, 
и когда данные готовы для отображения, просто вызовите `GameBoard.show()`.

- `GameBoard.show()` - переносит данные из массивов `GameBoard.matrix[0..15]` и `GameBoard.digit[0..2]` в дисплеи 
для их отображения.

Все ниже перечисленные дополнительные функции для работы с дисплеями, по своей сути изменяют данные 
только в перечисленных массивах, а не отображают данные на дисплеях на прямую. Для отображения данных 
так же необходимо вызвать `GameBoard.show()`.

## Oтображение на светодиодной матрице
Массив `GameBoard.matrix[0..15]` содержит 16 байт данных, каждый байт соответствует одной строке на дисплее. 
Всего на дисплее 16 строк. Cамая нижняя строка соответствует нулевой ячейке массива `GameBoard.matrix[0]`. 
Каждая ячейка массива содержит один байт данных, или 8 бит. Каждый бит будет определять, будет ли светиться 
соответствующий светодиод в строке из 8-и светодиодов. Например для включения правого светодиода в строке, 
в соответствующую ячейку массива необходимо записать значение 0x01, для левого 0x80, для свечения всей строки 
0xff в шестнадцатиричной системе счисления.

Ниже приведен простой пример включения и выключения правого нижнего светодиона с периодичностью в 1 секунду:
```
void loop() {
  GameBoard.matrix[0] = 1;
  GameBoard.show ();
  delay (500);
  GameBoard.matrix[0] = 0;
  GameBoard.show ();
  delay (500);
}
```
Дополнительные функции светодиодной матрицы:

- `GameBoard.clearMatrix ()` - очищает данные в массиве `GameBoard.matrix[0..15]` выставляя все значения в 0.
- `GameBoard.setMatrixIntensity (uint8_t v)` - устанавливает яркость отображения для всей светодиодной матрицы. 
Параметр функции `v` может изменяться от 0 до 15, где 0 соответствует полному затемнению отображения, а 15 максимальной яркости. Установка 
яркости происходит в момент вызова функции `GameBoard.show()`
- `GameBoard.printNumberToMatrix (uint8_t n, uint8_t x, uint8_t y)` - выводит значение цифры `n` oт 0 до 9 в 
позицию `(x, y)` массива `GameBoard.matrix[0..15]`.
- `GameBoard.startEffect (uint8_t effect)` - запускает эффект `effect` при выводе следующего кадра. 
Доступны эффекты `GB_MATRIX_EFFECT_LINES`, `GB_MATRIX_EFFECT_LEFT`, `GB_MATRIX_EFFECT_RIGHT`. Функция устанавливает, 
что при следующем вызове `GameBoard.show()` необходимо не просто отобразить данные, а отобразить их с соответствующим 
эффектом.

Константы для работы со светодиодной матрицей:

- `GB_MATRIX_LINES` - количество строк матрицы, равно 16.
- `GB_MATRIX_COLS` - количество колонок матрицы, равно 8.
- `GB_MAX_INTENSITY` - максимальное значение яркости свечения светодиодов, равно 15.


## Oтображение на семисегментных индикаторах

Массив `GameBoard.digit[0..2]` содержит три байта данных для отображения информации на трех семисегментных индикаторах. Элемент массива с индексом 0 `GameBoard.digit[0]` соответствует правому индикатору. Каждый бит данных массива соответствует одному светодиодному элементу на индикаторе. Вы можете выводить изображения не только цифр, но и любых элементов, в так же включать сегменты по отдельности.
Например ниже приведен код, который производит включение и выключение нижних сегментов на всех трех индикаторах:

```
void loop() {
  GameBoard.digit[0] = 1;
  GameBoard.digit[1] = 1;
  GameBoard.digit[2] = 1;
  GameBoard.show ();
  delay (500);
  GameBoard.digit[0] = 0;
  GameBoard.digit[1] = 0;
  GameBoard.digit[2] = 0;
  GameBoard.show ();
  delay (500);
}

```

Для вывода цифровых изображений на семисегментные индикаторы используются дополнительные функции библиотеки.

- `GameBoard.setDigitChar (uint8_t n, uint8_t ch)` - используется для вывода на семисегментный индикатор в позиции `n` данных, соответствующих отображению цифры `ch` в интервале от 0 до 9, а так же дополнительных символов в интервале от 10 до 14. Значение 15 соответствует выключению всех сегментов индикатора.
- `GameBoard.printNumberToDigit (uint16_t num)` - выводит на семисегментные индикаторы десятичное значение `num` в интервале от 0 до 999. Функция заполняет массив `GameBoard.digit[0..2]`, для отображения которого необходимо использовать `GameBoard.show()`

Дополнительные функции семисегментных индикаторов:

- `GameBoard.clearDigit ()` - очищает данные в массиве `GameBoard.digit[0..2]` выставляя все значения в 0.
- `GameBoard.setDigitIntensity (uint8_t v)` - устанавливает яркость отображения для всеx семисегментных индикаторов.
Параметр функции `v` может изменяться от 0 до 15, где 0 соответствует полному затемнению отображения, а 15 максимальной яркости. Установка 
яркости происходит в момент вызова функции `GameBoard.show()`

Константы для работы со светодиодной матрицей:

- `GB_DIGIT_COUNT` - количество семисегментных индикаторов, равно 3.

## Клавиатура

Библиотека постоянно опрашивает состояние клавиш и производит фильтрацию от дребезга контактов. В вашем 
сценарии вам достаточно выполнить запрос к библиотеке что бы определить состояние клавиш. Вы можете 
использовать один из двух режимов предоставления информации о нажатии клавиш: текущее состояние клавиш или 
события нажатия клавиш из буфера клавиатуры.

При запросе вы получаете специальный код кнопки или нескольких кнопок нажатых одновременно:

- `GB_KEYCODE_LEFT` - кнопка влево;
- `GB_KEYCODE_RIGHT` - кнопка вправо;
- `GB_KEYCODE_UP` - кнопка вверх;
- `GB_KEYCODE_DOWN` - кнопка вниз;
- `GB_KEYCODE_FIRE` - кнопка FIRE (огонь, выстрел);
- `GB_KEYCODE_RETURN` - кнопка RETURN (возврат);

### События нажатия клавиш из буфера клавиатуры

Библиотека постоянно отслеживает состояние клавиш и помещает события нажатия клавиш в специальный буфер клавиатуры. 
Все нажатые клавиши сохраняются в буфере последовательно. Вы можете считывать данные о нажатых 
клавишах из буфера вызывая функцию `GameBoard.getPressedKey ()`. Вызов функции извлекает событие нажатия из буфера 
и возвращает результат в виде кода клавиши. Возвращаемым результатом будет либо только один код какой либо 
клавиши или 0 если нажатий не было зафиксировано с предыдущего раза вызова данной функции. Данную функцию удобно 
использовать, так как нет необходимости самостоятельно следить за состоянием каждой клавиши что бы поймать ее нажатие.
Вы можете вызывать данную функцию в цикле и анализировать коды нажатых клавиш до тех пор, пока она не вернет вам 0.

- `GameBoard.getPressedKey ()` - возвращает код нажатой клавиши из буфера клавиатуры.

- `GameBoard.clearKeyBuffer ()` - очищает буфер клавиатуры и удаляет информацию о всех нажатых клавишах если такая была. 
Функция может быть использована при запуске какого либо нового процесса игры для того что бы события о ранее нажатых клавишах и не обработанные клавиши 
не мешали новому процессу.

```
  while (int key = GameBoard.getPressedKey ()) {
    if (key == GB_KEYCODE_FIRE) {
      // было зафиксировано событие нажатия клавиши FIRE
    }
    else if (key == GB_KEYCODE_LEFT) {
      // было зафиксировано событие нажатия клавиши влево
    }
  }
  // обработали все события нажатия клавиш..
  
```

### Текущее состояние клавиш

- `GameBoard.getKeysState ()` - функция возвращает текущее состояние всех клавиш. Результатом функции может быть 0 если
в текущий момент не нажата ни одна клавиша, может быть код клавиши если нажата одна клавиша, могут быть коды клавиш объединенные по функции ИЛИ если
нажаты одновременно несколько клавиш. Для проверки нажатия конкретной клавиши рекомендуется использовать функцию логического И, например
```
  int key = GameBoard.getKeysState ();
  if (key & GB_KEYCODE_FIRE != 0) {
    // в текущий момент нажата клавиша FIRE
  }
  if (key & (GB_KEYCODE_FIRE | GB_KEYCODE_RIGHT) != 0) {
    // в текущий момент нажаты одновременно клавиши FIRE и влево
  }
  
```

## Звук

Библиотека поддерживает воспроизведение монофонической мелодии в фоновом режиме. Это означает, что воспроизведение мелодии
выполняется совершенно параллельно вашей программе и не останавливает ее выполнение. 

Мелодии задаются при помощи массива в памяти программ `const uint16_t myMelody [] PROGMEM = {}`. Мелодия записывается с использованием
специальной азбуки, где каждый звук определяется его нотой, октавой и длительностью. Для определения конкретного звука все три 
параметра соединяются по функции ИЛИ. Например нота до первой октавы и длительностью одна восьмая будет иметь вид
`NOTE_C|OCT_4|DUR_8`. В конце мелодии обязательно должен присутствовать специальный код окончания мелодии `MELODY_END` или ее повторения `MELODY_REPEAT`.

Для воспроизведения мелодии необходимо вызвать функцию `GameBoard.playMelody (myMelody)`.

Пример определения массива c мелодией и код, запускающий ее воспроизведение при нажатии любой клавиши. 

```
const uint16_t myMelody [] PROGMEM = { 
  NOTE_C|OCT_5|DUR_32, 
  NOTE_E|OCT_5|DUR_32, 
  NOTE_G|OCT_5|DUR_32,  
  MELODY_END};

void loop () {  
  if (GameBoard.getPressedKey () != 0) { 
    GameBoard.playMelody (myMelody);
  }
}  
```

Функции для воспроизведения мелодий:

- `GameBoard.playMelody (const uint16_t * buf)` - запуск мелодии на воспроизведение в фоновом режиме. Программа сразу же начнет выполняться дальше.
- `GameBoard.playMelody (const uint16_t * buf, uint8_t tempo)` - запуск мелодии на воспроизведение с указанием темпа. По умолчанию используется темп равный 20.
- `GameBoard.stopMelody ()` - останавливает воспроизведение мелодии если она воспроизводится в текущий момент.


| Код | Нота |
|-------------------------|------------------------------|
| `NOTE_C` | **до** |
| `NOTE_Cm` или `NOTE_Db` | до диез или ре бемоль |
| `NOTE_D` | **ре** |
| `NOTE_Dm` или `NOTE_Eb` | ре диез или ми бемоль |
| `NOTE_E` | **ми** |
| `NOTE_F` | **фа** |
| `NOTE_Fm` или `NOTE_Gb` | фа диез или соль бемоль |
| `NOTE_G` | **соль** |
| `NOTE_Gm` или `NOTE_Ab` | соль диез или ля бемоль |
| `NOTE_A` | **ля** |
| `NOTE_Am` или `NOTE_Bb` | ля диез или си бемоль |
| `NOTE_B` | **си** |
| `NOTE_PAUSE` | пауза, октава не указывается |

| Код | Октава |
|-------------------------|------------------------------|
| `OCT_8` | пятая |
| `OCT_7` | четвертая |
| `OCT_6` | третья |
| `OCT_5` | вторая |
| `OCT_4` | первая |
| `OCT_3` | малая |
| `OCT_2` | большая |
| `OCT_1` | контр |
| `OCT_0` | суб-контр | 

| Код | Длительность |
|-------------------------|------------------------------|
| `DUR_64` | шестьдесят четвертая |
| `DUR_32` | тридцать вторая |
| `DUR_32P` | тридцать вторая с точкой |
| `DUR_16` | шестнадцатая |
| `DUR_16P` | шестнадцатая с точкой |
| `DUR_8` | восьмая |
| `DUR_8P` | восьмая с точкой |
| `DUR_4` | четвертная |
| `DUR_4P` | четвертная с точкой |
| `DUR_2` | половинная |
| `DUR_2P` | половинная с точкой |
| `DUR_1` | целая |
| `DUR_1P` | целая с точкой |
| `DUR_F2` | две целых |
| `DUR_F2P` | две целых с точкой |
| `DUR_F4` | четыре целых |