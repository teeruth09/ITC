#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

////////////////////////////////////bottom//////////////////////////////////////////////////
#define ButtonA 4 //down
#define ButtonB 5 //up
#define speakerPin 3 // sound
int stateA;
int stateB;
int LastA = LOW;
int LastB = LOW;
int debouncetime = 50;
int Lasttime;
int Lasttime2;
int DebounceA() {
  int A = digitalRead(ButtonA);
  if (A != LastA)
    Lasttime = millis();
  if (millis() - Lasttime > debouncetime)
  { stateA = A;
    return 1;
  }
  LastA = A;
  return 0;
}
int DebounceB() {
  int B = digitalRead(ButtonB);
  if (B != LastB)
    Lasttime2 = millis();
  if (millis() - Lasttime2 > debouncetime)
  {
    stateB = B;
    return 1;
  }
  LastB = B;
  return 0;
}
////////////////////////////////////bottom//////////////////////////////////////////////////
int16_t x1 = 0, y1 = 11, x2 = 0, y2 = 21;
uint8_t ball_dir_x = 1, ball_dir_y = 1;
uint8_t ball_x = 64, ball_y = 16;
int16_t score = 0;


void start(void) {
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(42, 10);
  display.println(F("PONG"));
  display.display();  
  delay(2000);    
}

void gameover(void) {
  display.clearDisplay();

  display.setCursor(12, 3);
  display.setTextSize(2);
  display.print(F("GAME OVER"));
  display.setCursor(12, 24);
  display.setTextSize(1);
  display.print(F("score = "));
  display.setTextSize(1);
  display.print(score);
  display.display();    
  delay(100);
  exit(0); // Exit loop
}

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  pinMode(ButtonA, INPUT);
  pinMode(ButtonB, INPUT);
  pinMode(speakerPin, OUTPUT);

  start();
  display.clearDisplay();
}

void loop() {
  DebounceA();
  DebounceB();
  if (stateA == HIGH && DebounceA() == 1)
  {
    display.clearDisplay();
    if (y2 < 31) {
      y1 += 1;
      y2 += 1;
    }
  }
  if (stateB == HIGH && DebounceB() == 1)
  {
    display.clearDisplay();
    if (y1 > 0) {
      y1 -= 1;
      y2 -= 1;
    }
  }
  uint8_t new_x = ball_x + ball_dir_x;
  uint8_t new_y = ball_y + ball_dir_y;

  if (new_x == 127) // ชนขอบขวา
  {
    ball_dir_x = -ball_dir_x;
    new_x += ball_dir_x + ball_dir_x;
    tone(speakerPin, 500, 10);
  }
  if (new_y == 0 || new_y == 32) // ชนขอบบน,ขอบล่าง
  {
    ball_dir_y = -ball_dir_y;
    new_y += ball_dir_y + ball_dir_y;
    tone(speakerPin, 500, 10);
  }
  if (new_x == 0 && new_y >= y1 - 1 && new_y <= y2 + 1) // ชนแป้น
  {
    ball_dir_x = -ball_dir_x;
    new_x += ball_dir_x + ball_dir_x;
    score+=1;
    tone(speakerPin, 500, 10);
  }
  if (new_x == 0 && !(new_y >= y1 && new_y <= y2)) // ไม่ชน
  {
    gameover();
  }
  display.drawCircle(ball_x, ball_y, 1, SSD1306_BLACK);
  display.drawCircle(new_x, new_y, 1, SSD1306_WHITE);
  display.drawLine(0, y1, 0, y2, SSD1306_WHITE);
  ball_x = new_x;
  ball_y = new_y;
  display.display();
}
