#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <array>

#include "./truth_tables.hpp"
#include "./gate_symbols.h"

#define BUTTON_INPUT_A 5
#define BUTTON_INPUT_B 7

#define BUTTON_TOGGLE_GATE 6

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SCREEN_RESET -1

#define SCREEN_I2C_ADDR 0x3C

// Which pin on the Arduino is connected to the NeoPixels?
#define NEOPIXEL_PIN        47 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NEOPIXEL_NUMPIXELS 4 // Popular NeoPixel ring size

const std::array<eGateType, 6> allGateTypes = {
  eGateType::AND,
  eGateType::OR,
  eGateType::XOR,
  eGateType::NAND,
  eGateType::NOR,
  eGateType::XNOR
};

bool toggleInputPressedLastTick = false;
uint8_t currentLogicGateIndex = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);
Adafruit_NeoPixel pixels(NEOPIXEL_NUMPIXELS, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);

const unsigned char* gateTypeToBitmapAddr(eGateType gateType) {
  switch (gateType) {
    case eGateType::AND: return AND_SYMBOL;
    case eGateType::OR: return OR_SYMBOL;
    case eGateType::XOR: return XOR_SYMBOL;
    case eGateType::NAND: return NAND_SYMBOL;
    case eGateType::NOR: return NOR_SYMBOL;
    case eGateType::XNOR: return XNOR_SYMBOL;
    default: return AND_SYMBOL;
  }
}

void turnOnAllNeopixel() {
  pixels.clear();

  for (int i = 0; i < NEOPIXEL_NUMPIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));

  pixels.show();
}

void turnOffAllNeopixel() {
  pixels.clear();
  pixels.show();
}

void setupDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR)) {
    Serial.println("Failed to init display!");
    for (;;);
  }

  display.clearDisplay();
  display.display();

  pixels.begin();
}

void updateDisplay() {
  display.clearDisplay();
  display.drawBitmap(0, 0, gateTypeToBitmapAddr(static_cast<eGateType>(currentLogicGateIndex)), 128, 64, 1);
  
  String gateString = GateTypeToString(static_cast<eGateType>(currentLogicGateIndex));
  // int16_t x1, y1;
  // uint16_t w, h;
  // display.getTextBounds(gateString.c_str(), 0, 0, &x1, &y1, &w, &h);

  // display.setCursor((SCREEN_WIDTH - w) / 2, SCREEN_HEIGHT - 16);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.write(gateString.c_str());

  display.display();
}

void setup() {
  Serial.begin(115200);
  setupDisplay();
  updateDisplay();

  pinMode(BUTTON_INPUT_A, INPUT_PULLUP);
  pinMode(BUTTON_INPUT_B, INPUT_PULLUP);
  pinMode(BUTTON_TOGGLE_GATE, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  uint8_t 
    buttonA = !digitalRead(BUTTON_INPUT_A),
    buttonB = !digitalRead(BUTTON_INPUT_B),
    buttonToggle = !digitalRead(BUTTON_TOGGLE_GATE);

  // updating of the currentLogicGateIndex variable
  if (buttonToggle && !toggleInputPressedLastTick) {
    currentLogicGateIndex++;
    if (currentLogicGateIndex >= static_cast<uint8_t>(allGateTypes.size())) {
      currentLogicGateIndex = 0;
    }

    updateDisplay();
    delay(500);
  }

  if (buttonToggle) {
    toggleInputPressedLastTick = true;
  } else {
    toggleInputPressedLastTick = false;
  }
  
  // application of logic
  Serial.println(String(buttonA) + " " + String(buttonB) + " " + String(buttonToggle));
  if (truthLUT[(currentLogicGateIndex << 2) | (buttonA << 1) | buttonB])
    turnOnAllNeopixel();
  else turnOffAllNeopixel();
}
