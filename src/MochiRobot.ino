/*
  Desi Mochi Robot - Source Code
  Author: Jayabeer Sutar
  Date: August 31, 2025
  
  A simple, interactive robot that reacts to touch with sounds and expressions on an OLED display.
  Built with a Xiao ESP32-C3.
  Inspired by the Mochi robot from Huy Khong (https://themochi.huykhong.com/).
*/

// Include necessary libraries
#include <Wire.h>
#include <U8g2lib.h>

// Define the pins for our components
#define TOUCH_PIN   D1 // The TTP223 touch sensor Signal pin
#define BUZZER_PIN  D2 // The Buzzer positive pin

// I2C pins for OLED on Xiao ESP32-C3 are D4 (SDA) and D5 (SCL)

// Initialize the OLED display using the U8g2 library.
// This constructor is for a 128x32 display. If you have 128x64, change it to:
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* scl=*/ D5, /* sda=*/ D4, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* scl=*/ D5, /* sda=*/ D4, /* reset=*/ U8X8_PIN_NONE);

// Variables for managing state
bool isTouched = false;
unsigned long lastBlinkTime = 0;
bool eyeState = true; // true = open, false = closed

void setup() {
  // Start Serial for debugging (optional)
  Serial.begin(115200);

  // Set pin modes
  pinMode(TOUCH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize the display
  u8g2.begin();
  
  // Play a startup sound
  playStartupSound();

  // Display a welcome message
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(30, 20, "Mochi Booting...");
  u8g2.sendBuffer();
  delay(2000);
}

void loop() {
  // Read the current state of the touch sensor
  isTouched = digitalRead(TOUCH_PIN) == HIGH;

  if (isTouched) {
    // If Mochi is touched...
    drawHappyFace();
    playInteractionSound();
    delay(200); // A small delay to prevent rapid re-triggering
  } else {
    // If Mochi is not being touched, just blink
    drawBlinkingFace();
  }
}

// --- Face Drawing Functions ---

void drawBlinkingFace() {
  // This function makes the eyes blink every 2 seconds
  if (millis() - lastBlinkTime > 2000) {
    eyeState = !eyeState; // Toggle the eye state
    lastBlinkTime = millis();
  }

  u8g2.clearBuffer();
  
  if (eyeState) {
    // Draw open eyes (circles)
    u8g2.drawCircle(45, 16, 8); // Left eye
    u8g2.drawCircle(83, 16, 8); // Right eye
  } else {
    // Draw closed eyes (lines)
    u8g2.drawHLine(38, 16, 14); // Left eye
    u8g2.drawHLine(76, 16, 14); // Right eye
  }
  
  u8g2.sendBuffer();
}

void drawHappyFace() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  
  // Draw happy "up-turned" eyes using simple geometric shapes
  u8g2.drawEllipse(45, 16, 10, 5, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawEllipse(45, 16, 10, 5, U8G2_DRAW_UPPER_LEFT);
  
  u8g2.drawEllipse(83, 16, 10, 5, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawEllipse(83, 16, 10, 5, U8G2_DRAW_UPPER_LEFT);
  
  u8g2.sendBuffer();
}


// --- Sound Functions ---

void playStartupSound() {
  tone(BUZZER_PIN, 1000, 100); // tone(pin, frequency, duration)
  delay(120);
  tone(BUZZER_PIN, 1200, 100);
  delay(120);
  tone(BUZZER_PIN, 1400, 100);
}

void playInteractionSound() {
  tone(BUZZER_PIN, 1500, 50);
  delay(60);
  tone(BUZZER_PIN, 1800, 50);
}
