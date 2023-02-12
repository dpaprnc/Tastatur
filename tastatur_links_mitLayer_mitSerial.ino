#include <Keyboard.h>

/*
7  18  19  20  21
15 14  16  10  1
2  3   4   5   6
-  -  -  8  9
*/

// Initialisiere Pins als Array und die dazugehörigen Tasten
const size_t buttonPin[] = { 7, 18, 19, 20, 21, 15, 14, 16, 10, 1, 2, 3, 4, 5, 6, 8, 9 };
const size_t buttonSym[] = { 'q', 'w', 'e', 'r', 't', 'a', 's', 'd', 'f', 'g', 'z', 'x', 'c', 'v', 'b', KEY_LEFT_SHIFT, 132 };
const size_t buttonSym1[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '%', '*', '/', KEY_LEFT_SHIFT, 132 };

size_t previousButtonState[17] = { HIGH };
size_t buttonState[17] = { LOW };

char receivedChar = -1;
bool layer_default = true;
bool layer_1 = false;

void setup() {
  // Setup:
  // Beginnt serialle Kommunikation mit 9600 Baud
  Serial1.begin(9600);
  for (size_t i = 0; buttonPin[i] != '\0'; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  Keyboard.begin();
}

void loop() {
  // Schreibe gesendeten byte
  recvOneChar();

  // überprüfe den status der Tasten
  for (size_t i = 0; buttonPin[i] != '\0'; i++) {
    buttonState[i] = digitalRead(buttonPin[i]);
  }
  
  // Knopf gedrückt
  for (size_t i = 0; buttonPin[i] != '\0'; i++) {
    if (buttonState[i] == LOW && previousButtonState[i] == HIGH) {
      if (layer_default) {
        // ist derzeit noch gedrückt:
        Keyboard.press(buttonSym[i]);
        delay(50);
      }
      if (layer_1) {
        Keyboard.press(buttonSym1[i]);
        delay(50);
      }
    }
  }
  for (size_t i = 0; buttonPin[i] != '\0'; i++) {
    if (buttonState[i] == HIGH && previousButtonState[i] == LOW) {
      // ist derzeit nicht mehr gedrückt
      if (layer_default) {
        // ist derzeit noch gedrückt:
        Keyboard.release(buttonSym[i]);
        delay(50);
      }
      if (layer_1) {
        Keyboard.release(buttonSym1[i]);
        delay(50);
      }
    }
  }
  // setze den derezitigen Status der Knöpfe auf den Vorherigen Zustand für den nächsten durchlauf
  for (size_t i = 0; buttonPin[i] != '\0'; i++) {
    previousButtonState[i] = buttonState[i];
  }
}

void recvOneChar() {
  if (Serial1.available() > 0) {
    receivedChar = Serial1.read();
    if (receivedChar == (char)0xc2) {
      layer_default = layer_default ^ true;
      layer_1 = layer_1 ^ true;
    } else {
      Keyboard.press(receivedChar);
      Keyboard.release(receivedChar);
    }
  }
}