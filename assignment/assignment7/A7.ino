#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>

#define OLED_RESET -1
#define switch1 13
#define switch2 12
#define switch3 11
#define switch4 10
#define buzzer 6

#include <EEPROM.h>
Adafruit_SSD1306 OLED(OLED_RESET);
int hour, minute, second, Mode;
int wakeuptime[2], wakeupstatus;
long timer1;
int timerstatus;
int countdowntime[2], countdownstatus;
void setup()
{
  Serial.begin(9600);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Timer1.initialize(1000000); // microsecond
  Timer1.attachInterrupt(count);
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(10, 0);
  OLED.setTextSize(2);
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
  pinMode(switch4, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  hour = EEPROM.read(0); //0-1023
  minute = EEPROM.read(1);
  wakeuptime[0] = EEPROM.read(2);
  wakeuptime[1] = EEPROM.read(3);
  wakeupstatus = EEPROM.read(4);
}

void loop()
{
  if (digitalRead(switch4) == 0)
  {
    Mode = (Mode + 1) % 4;
    delay(100);
  }

  if (hour == wakeuptime[0] && minute == wakeuptime[1] && second == 0 && wakeupstatus)
  {
    Serial.print("Wake up : ");
    Serial.println(wakeupstatus);
    tone(buzzer, 700, 25);
    delay(20);
    tone(buzzer, 700, 25);
    delay(200);
    tone(buzzer, 700, 25);
    delay(200);
    tone(buzzer, 700, 25);
    delay(200);
  }
  if (countdowntime[0] == 0 && countdowntime[1] == 0 && countdownstatus == 1)
  {
    Serial.print("Count down : ");
    Serial.println(countdownstatus);
    countdownstatus = 0;
    tone(buzzer, 700, 25);
    delay(200);
    tone(buzzer, 700, 25);
    delay(200);
    tone(buzzer, 700, 25);
    delay(200);
    tone(buzzer, 700, 25);
    delay(200);
  }

  OLED.clearDisplay();
  if (Mode == 0)
  {
    printtime(hour, minute);
    setTime();
    OLED.setCursor(30, 17);
    OLED.print("time");
  }
  if (Mode == 1)
  {
    wakeup();
    printtime(wakeuptime[0], wakeuptime[1]);
    OLED.setCursor(10, 17);
    OLED.print("wakeup");
    if (wakeupstatus == 1)
      OLED.print("on");
    else
      OLED.print("off");
  }
  if (Mode == 2)
  {
    countdown();
    printtime(countdowntime[0], countdowntime[1]);
    OLED.setCursor(15, 17);
    OLED.print("countdown");
  }
  if (Mode == 3)
  {
    timer();
    printtime(timer1 / 60, timer1 % 60);
    OLED.setCursor(30, 17);
    OLED.print("timer");
  }
  OLED.display();
}

void printtime(int a, int b)
{
  OLED.setCursor(30, 0);
  if (a < 10)
  {
    OLED.print("0");
  }
  OLED.print(a);

  OLED.print(":");

  OLED.setCursor(65, 0);
  if (b < 10)
  {
    OLED.print("0");
  }
  OLED.print(b);
}

void count()
{
  second++;
  if (second == 60)
  {
    second = 0;
    minute++;
  }
  if (minute == 60)
  {
    minute = 0;
    hour++;
  }
  hour %= 24;
  if (countdownstatus == 1)
  {
    countdowntime[1]--;
    if (countdowntime[1] == -1)
    {
      countdowntime[1] = 59;
      countdowntime[0]--;
    }
  }
  if (timerstatus == 1)
  {
    timer1++;
  }
}

void setTime()
{
  if (digitalRead(switch1) == 0)
  {
    hour++;
    hour = hour % 24;
    delay(100);
  }
  if (digitalRead(switch2) == 0)
  {
    minute++;
    minute = minute % 60;
    delay(100);
  }
  if (digitalRead(switch3) == 0) //EEPROM
  {
    EEPROM.put(0, hour);
    EEPROM.put(1, minute);
    delay(100);
  }
}

void wakeup()
{
  if (digitalRead(switch1) == 0)
  {
    wakeuptime[0]++;
    wakeuptime[0] = wakeuptime[0] % 24;
    delay(100);
  }
  if (digitalRead(switch2) == 0)
  {
    wakeuptime[1]++;
    wakeuptime[1] = wakeuptime[1] % 60;
    delay(100);
  }
  if (digitalRead(switch3) == 0) //EEPROM
  {
    EEPROM.put(2, wakeuptime[0]);
    EEPROM.put(3, wakeuptime[1]);
    wakeupstatus = !wakeupstatus;
    EEPROM.put(4, wakeupstatus);
    delay(200);
  }
}
void countdown()
{
  if (digitalRead(switch1) == 0)
  {
    countdowntime[0]++;
    delay(100);
  }
  if (digitalRead(switch2) == 0)
  {
    countdowntime[1]++;
    countdowntime[1] = countdowntime[1] % 60;
    delay(100);
  }
  if (digitalRead(switch3) == 0) //EEPROM
  {
    countdownstatus = 1;
    delay(200);
  }
  if (countdownstatus == 1 && digitalRead(switch1) == 0)
  {
    countdowntime[0] = 0;
    countdowntime[1] = 0;
    countdownstatus = 0;
    delay(200);
  }
}
void timer()
{
  if (digitalRead(switch1) == 0)
  {
    timerstatus = !timerstatus;
    delay(200);
  }
  if (digitalRead(switch2) == 0)
  {
    timer1 = 0;
    delay(100);
  }
}
