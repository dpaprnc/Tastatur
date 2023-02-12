#include <Keyboard.h>

/*
21  20  19  18  7   
1 10  16  14  15
6  5   4   3   2
9  8    -  -  -
*/

//Initialisiere Pins als Array und die dazugehörigen Tasten
const size_t buttonPin[] = { 21, 20, 19, 18, 7, 1, 10, 16, 14, 15, 6, 5, 4, 3, 2, 9, 8 };
const size_t buttonSym[] = { 'y', 'u', 'i', 'o', 'p', 'h', 'j', 'k', 'l', 10, 'n', 'm', ',', '.', 32, 0xc2, 8 };
const size_t buttonSym1[] = { '<', '>', '=', '-', '+', '(', ')', '[', ']', 10, ';', '\'', '/', '!', 32, 0xc2, 8 };

size_t previousButtonState[17] = { HIGH };
size_t buttonState[17] = { LOW };

bool layer_default = true;
bool layer_1 = false;

void setup() {
  // Setup:
  for (size_t i = 0; i<17; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  Keyboard.begin();
}

void loop() {

  //überprüfe den status der Tasten
  for (size_t i = 0; i<17; i++) {
    buttonState[i] = digitalRead(buttonPin[i]);
  }
  // Beginnt serialle Kommunikation mit 9600 Baud
  Serial1.begin(9600);
    // warte bis Serial1 aktiv ist
  while (!Serial1) {
    //warte
  }
  //Knopf gedrücktdg
  for (size_t i = 0; i<17; i++) {
    if (buttonState[i] == LOW && previousButtonState[i] == HIGH) {
      // ist derzeit noch gedrückt
      //wenn 239 gedrückt wurde
      if (i == 15) {
        //wechsel layer
        layer_default = layer_default ^ true;
        layer_1 = layer_1 ^ true;
        //und sende and linke Seite den Layerwechsel
        Serial1.write(buttonSym[i]);
        delay(50);
      }
      //sonst..
      else {
        // wenn default Layer aktiv
        if (layer_default) {
          //sende den Gedrückten Knopf aus default Layer
          Serial1.write(buttonSym[i]);
          delay(50);
        }
        //wenn Layer 1 aktiv
        if (layer_1) {
          //sende den Gedrückten Knopf aus Layer 1
          Serial1.write(buttonSym1[i]);
          delay(50);
        }
      }
    }
  }
  // Serial ende
  Serial1.end();
  //pinMode(buttonPin[5], INPUT_PULLUP);

  // setze den derezitigen Status der Knöpfe auf den Vorherigen Zustand für den nächsten durchlauf
  for (size_t i = 0; i<17; i++) {
    previousButtonState[i] = buttonState[i];
  }
}

