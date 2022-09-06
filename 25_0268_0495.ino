#define button1 12 //left
#define button2 10 //middle
#define button3 8 //right
#define LED3 4 // red
#define LED2 3 //yellow
#define LED1 2 //green
int startR = 0, startG = 0, startY = 0, subY = 0;
int reading = 0;
int buttonState1;
int lastButtonState = LOW;
long LastDebouceTime = 0;
int ledState1 = LOW;
//start
int reading2 = 0;
int buttonState2;
int lastButtonState2 = LOW;
long LastDebouceTime2 = 0;
int ledState2 = LOW;
//end
void setup()
{
  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(LED3, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED1, OUTPUT);
}

void loop()
{
  int cur = millis();
  

//  Serial.println(digitalRead(button1));
//  Serial.println(digitalRead(button2));
//  Serial.println(digitalRead(button3));
//  Serial.println("_______________________________________");
//  delay(1000);
  switchRed(cur);
  switchGreen(cur);
  switchYellow(cur);
}

void switchRed(int cur)
{
//  int Redled = 0;
//  int lastB =0 ;
//start
    int ischange = 0;
  reading = digitalRead(button1);
  if (reading != lastButtonState)
  {
    LastDebouceTime = millis();
  }
  if((millis()-LastDebouceTime)>=50)
  {
    if(reading != buttonState1)
    {
      buttonState1 = reading;
      if(buttonState1 == HIGH)
      {
      startR = millis();  
      ledState1 != ledState1;         
      ischange = 1;
      }
    }
    lastButtonState = reading;
  }
//end                
  if (cur - startR > 3000 && startR != 0)
  {
   digitalWrite(LED3, LOW);
    startR = 0; 
 }
  
  else if (ischange && buttonState1 == HIGH)
  {
    digitalWrite(LED3, !digitalRead(LED3));
    startR = millis();//10,000
      
  }
  lastButtonState = reading;
}

void switchGreen(int cur)
{
  //start
 // int Greenled = 0;
  //int lastB = 0;
  int ischange = 0;
  reading2 = digitalRead(button3);
  if (reading2 != lastButtonState2)
  {
    LastDebouceTime2 = millis();
  }
  if((millis()-LastDebouceTime2)>=50)
  {
    if(reading2 != buttonState2)
    {
      buttonState2 = reading2;
      if(buttonState2 == HIGH)
      {
      startG = millis();  
      ledState2 != ledState2; 
      ischange = 1;
      }
    }//start
    lastButtonState2 = reading2;
    return ischange;
  }
  //end
  if (cur - startG > 3000 && startG != 0)
  {
    digitalWrite(LED1, LOW);
    startG = 0;
  }
  else if (digitalRead(button3) == LOW && digitalRead(LED3) != HIGH)
  {
    digitalWrite(LED1, !digitalRead(LED1));
    startG = millis();
    delay(200);
  }
}

void switchYellow(int cur)
{
  if (cur - startY <= 2000 && startY != 0)
  {
    if ((cur - subY) > 250) 
    {
      digitalWrite(LED2, !digitalRead(LED2));
      subY = millis();
    }
  }
  else if (cur - startY > 2000 && startY != 0)
  {
    digitalWrite(LED2, LOW);
    startY = 0;
  }
  else if (digitalRead(button2) == LOW && digitalRead(LED3) != HIGH && digitalRead(LED1) != HIGH)
  {
    digitalWrite(LED2, HIGH);
    startY = millis();
    subY = millis();
    delay(200);
  }
}
