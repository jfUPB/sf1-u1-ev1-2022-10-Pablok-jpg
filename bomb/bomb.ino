#include "SSD1306Wire.h"

#define BOMB_OUT 25 // rojo
#define LED_COUNT 26 // blanco
#define UP_BTN 13  //rojo
#define DOWN_BTN 32 //cafe
#define ARM_BTN 33 //naranja
int UPbuttonState = 0;
int lastUPbuttonState = 0;
int DOWNbuttonState = 0;
int lastDOWNbuttonState = 0;
bool bPress = false;

// Selecciona uno según tu display.
//SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_64_48);

void setup() {
  digitalWrite(BOMB_OUT, LOW);
  digitalWrite(LED_COUNT, LOW);
  pinMode(BOMB_OUT, OUTPUT);
  pinMode(LED_COUNT, OUTPUT);
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(ARM_BTN, INPUT_PULLUP);
  display.init();
  display.setContrast(255);
  display.clear();
  Serial.begin(115200);


}

void loop() {
  static uint8_t counter = 0;
  static uint32_t oldTime = 0;
  uint32_t newTime;
  static int x = 10;
  static int y = 20;
  UPbuttonState = digitalRead(UP_BTN);
  DOWNbuttonState = digitalRead(DOWN_BTN);

  enum class BombStates {Init, Configuracion, Activa, Desactivada, BOOM};
  static BombStates bombState =  BombStates::Configuracion;

  switch (bombState) {

    case BombStates::Init: {
        display.init();
        display.setContrast(255);
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_16);
        digitalWrite(BOMB_OUT, LOW);        
        display.clear();
        display.drawString(0, 5, String(counter));
        display.display();
        bombState = BombStates::Configuracion;
        counter = 20;
        break;
      }

    case BombStates::Configuracion:

      if (UPbuttonState != lastUPbuttonState) {
        if (UPbuttonState == HIGH) {
          bPress = true;
          counter++;
          if (counter > 60) {
            counter = 60;
            delay(50);
          }
        }
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_16);
        display.drawString(x, y, String(counter));
        display.display();
        lastUPbuttonState = UPbuttonState;
      }

      else if (DOWNbuttonState != lastDOWNbuttonState) {
        if (DOWNbuttonState == HIGH) {
          bPress = true;
          counter--;
          if (counter < 10) {
            counter = 10;
            delay(50);
          }
        }
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_16);
        display.drawString(x, y, String(counter));
        display.display();

      }
      lastDOWNbuttonState = DOWNbuttonState;

      digitalWrite(LED_COUNT, HIGH);
      
      else if (ARMbuttonState != lastARMbuttonState) {
          if (ARMbuttonState == HIGH) {
            bPress = true;
            bombState = BombStates::Activa;
          }
        }
        lastARMbuttonState = ARMbuttonState;

        digitalWrite(LED_COUNT, HIGH);

        break;
      }

    case BombStates::Activa: {
        const uint32_t TiempoLED_COUNT = 500;
        static uint32_t previousTMinus = 0;
        static uint8_t LED_countState = LOW;
        uint32_t currentTMinus = millis();

        if (currentTMinus - previousTMinus >= TiempoLED_COUNT) {
          previousTMinus = currentTMinus;
          if (LED_countState == LOW) {
            LED_countState = HIGH;
          }
          else {
            LED_countState = LOW;
            counter--;
            display.clear();
            display.setTextAlignment(TEXT_ALIGN_LEFT);
            display.setFont(ArialMT_Plain_16);
            display.drawString(x, y, String(counter));
            display.display();
          }
          digitalWrite(LED_COUNT, LED_countState);
        }
        if (counter == 0) {
          bombState = BombStates::BOOM;
        }
        break;
      }

    case BombStates::BOOM: {
        digitalWrite(LED_COUNT, LOW);
        digitalWrite(BOMB_OUT, HIGH);
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_16);
        display.drawString(x, y, String("BOOM!!"));
        display.display();
        delay (3000);
        bombState = BombStates::Init;
      }

  }

}
