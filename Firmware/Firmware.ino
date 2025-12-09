
#include "driver/gpio.h"
#include "soc/gpio_struct.h"
#include "soc/gpio_reg.h"
// Libraries
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>

// OLED screen resolution
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

// OLED SPI pin definitions
#define OLED_MOSI  15
#define OLED_CLK   5  
#define OLED_DC    10  
#define OLED_RESET 3
#define OLED_CS    13 

// Initialize OLED display object
Adafruit_SSD1306 Oled(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

#define DC_HIGH  (GPIO.out_w1ts = (1UL << OLED_DC))
#define DC_LOW   (GPIO.out_w1tc = (1UL << OLED_DC))
#define CS_HIGH  (GPIO.out_w1ts = (1UL << OLED_CS))
#define CS_LOW   (GPIO.out_w1tc = (1UL << OLED_CS))
// #define DC_HIGH  digitalWrite(OLED_DC,HIGH);
// #define DC_LOW  digitalWrite(OLED_DC,LOW);
// #define CS_HIGH  digitalWrite(OLED_CS,HIGH);
// #define CS_LOW  digitalWrite(OLED_CS,LOW);
uint8_t buffer[SCREEN_WIDTH * (SCREEN_HEIGHT / 8)];

void sendCommand(uint8_t cmd) {
  SPI.transfer(cmd);
}

void sendData(uint8_t data) {
  SPI.transfer(data);
}

// Set a pixel ON or OFF and update OLED immediately
void drawPixelFast(int x, int y, bool color) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

  int page = y / 8;
  int bit = y % 8;
  int index = page * SCREEN_WIDTH + x;

  if (color)
    buffer[index] |= (1 << bit);
  else
    buffer[index] &= ~(1 << bit);

  // Update just this byte on OLED
  
  //CS_LOW;
  uint8_t cmds[3] = { 0xB0 + page, 0x00 + (x & 0x0F), 0x10 + ((x >> 4) & 0x0F) };
  DC_LOW;
  SPI.transfer(cmds, 3); // send all commands at once
  DC_HIGH;
  SPI.transfer(buffer[index]);            // Send updated byte
  //CS_HIGH;
}


void setup() {
  delay(1000);


  Serial.begin(9600);
  SPI.begin(OLED_CLK, -1, OLED_MOSI, OLED_CS);

  Serial.println("a!");
  pinMode(OLED_RESET, OUTPUT);
  digitalWrite(OLED_RESET, LOW);
  delay(50);
  digitalWrite(OLED_RESET, HIGH);
  delay(50);

  // Initialize the OLED display
  if(!Oled.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true) {Serial.println("nope");}
  }
  // for (int i = 0; i < 10; i++) {
  //   Serial.println("yes!");
  //   delay(1000);
  // }
  // Set display text size and color, then clear the screen
  Oled.ssd1306_command(SSD1306_SETCONTRAST);
  Oled.ssd1306_command(0xFF);  // Max contrast
  Oled.setRotation(0);
  Oled.invertDisplay(false);

  Oled.clearDisplay();
  Oled.setCursor(0, 0);
  Oled.setTextSize(2);
  Oled.setTextColor(WHITE);
  Oled.display();
  delay(1000);
  Oled.println("Hi");
  delay(1000);

      // Display the content
  CS_LOW;

  while(true) {
    for(int i = 0; i < SCREEN_WIDTH; i++) {
      for (int x = 0; x < SCREEN_HEIGHT; x++) {
        // Oled.drawPixel(i, x, SSD1306_INVERSE);
        // Oled.display();
        drawPixelFast(i, x, true); 
        //delay(10);
      }
    }
    //delay(1000);
    for(int i = 0; i < 128; i++) {
      for (int x = 0; x < 64; x++) {
        // Oled.drawPixel(i, x, SSD1306_INVERSE);
        // Oled.display();
        drawPixelFast(i, x, false); 
        //delay(10);
      }
    }
    //delay(1000);
  }
}

float t = 0;  // Time variable for animation

void loop() {




  // Oled.clearDisplay();

  // // Draw animated diagonal sine bands
  // for (int y = 0; y < 64; y++) {
  //   // Sine-controlled diagonal banding
  //   float wave = sin((y * 0.15) + t) * 30; // wave range [-30, +30]
  //   int centerX = (int)(wave + 64);       // center the wave horizontally

  //   // Draw a soft band
  //   for (int w = -4; w <= 4; w++) { // thickness 9
  //     int x = centerX + w + y / 2; // diagonal skew
  //     if (x >= 0 && x < 128) {
  //       Oled.drawPixel(x, y, SSD1306_WHITE);
  //     }
  //   }
  // }

  // Oled.display();
  // t += 0.1;     // Animation speed
  // delay(100);    // Frame delay
}
