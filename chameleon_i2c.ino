#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>

// ---------------- NeoPixel ----------------
#define NEOPIXEL_PIN 3
#define NUM_PIXELS   10

Adafruit_NeoPixel pixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ---------------- TCS34725 ----------------
Adafruit_TCS34725 tcs =
  Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,
                    TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);

  if (!tcs.begin()) {
    Serial.println("No TCS34725 found! Check wiring.");
    while (1);
  }

  pixel.begin();
  pixel.show(); // OFF
}

void loop() {
  float red, green, blue;

  tcs.setInterrupt(false);
  delay(60);
  tcs.getRGB(&red, &green, &blue);
  tcs.setInterrupt(true);

  uint8_t r = constrain((int)red,   0, 255);
  uint8_t g = constrain((int)green, 0, 255);
  uint8_t b = constrain((int)blue,  0, 255);

  uint32_t color = 0; // default OFF

  // -------- COLOR LOGIC --------
  if (r > 100 && g < 100 && b < 100) {
    color = pixel.Color(255, 0, 0);   // RED
  }
  else if (g > 100 && r < 100 && b < 100) {
    color = pixel.Color(0, 255, 0);   // GREEN
  }
  else if (b > 100 && r < 100 && g < 100) {
    color = pixel.Color(0, 0, 255);   // BLUE
  }
  else {
    color = 0; // OFF
  }

  // -------- APPLY TO ALL 10 LEDs --------
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixel.setPixelColor(i, color);
  }

  pixel.show();

  // Debug
  Serial.print("R: "); Serial.print(r);
  Serial.print("  G: "); Serial.print(g);
  Serial.print("  B: "); Serial.println(b);

  delay(200);
}
