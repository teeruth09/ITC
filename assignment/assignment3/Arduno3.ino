#define ButtonA 10
#define ButtonB 11
int stateA;
int stateB;
int LastA = LOW;
int LastB = LOW;
int debouncetime = 50;
int Lasttime;
int Lasttime2;
int number;
int bitPattern[9] = {B1111001, B0100100, B0110000, B0011001, B0010010, B0000010, B1111000, B0000000, B0010000};

int compare[3] = {B1000000, B1000010, B1000111};   //equal = 0 gather = c less = L 
const byte numPins = 7;
const int segmentPins[7] = {8,2,3,4,5,6,7};
//start
int randomNo;
int coolDown;
int increasingNumber =0;
bool isbit;
//end
void setup() {
  Serial.begin(9600);
  for(int i=0; i<numPins; i++)
    pinMode(segmentPins[i],OUTPUT);
  pinMode(ButtonA,INPUT);
  pinMode(ButtonB,INPUT);
  //start
  randomSeed(analogRead(A0));
  randomNo = random(1,9);    //start 1
//Start Random
  Serial.print("Random =");
  Serial.println(randomNo);
}
//start Debounce
void DebounceA() {
    int A = digitalRead(ButtonA);
    if(A!=LastA)
    Lasttime = millis();
    if(millis()-Lasttime > debouncetime)
    stateA=A;
    LastA=A;
}

void DebounceB() {
    int B = digitalRead(ButtonB);
    if(B!=LastB)
    Lasttime2 = millis();
    if(millis()-Lasttime2 > debouncetime)
    stateB=B;
    LastB=B;
}
//start  
void StepNumber() 
{ 
  if(millis() - coolDown> 50) 
  { 
   if(increasingNumber>8) 
  { 
    increasingNumber = 0;//1;
  }
  for(int segment =0;segment<7;segment++) 
  { 
    isbit = bitRead(bitPattern[increasingNumber],segment);
    digitalWrite(segmentPins[segment],isbit);
  }
  increasingNumber++; 
  }
  coolDown = millis();
}
//end


//start check number
void Checknum()
{
 //equal
 if(increasingNumber ==  randomNo)
 {
    for(int segment =0;segment<7;segment++) 
    { 
    isbit = bitRead(compare[0],segment);
    digitalWrite(segmentPins[segment],isbit);
    }
    randomSeed(analogRead(A0));
    randomNo = random(1,9);
 } 
  //Morethan
  if(increasingNumber >  randomNo)
 {
    for(int segment =0;segment<7;segment++) 
    { 
    isbit = bitRead(compare[1],segment);
    digitalWrite(segmentPins[segment],isbit);
    }
    randomSeed(analogRead(A0));
    randomNo = random(1,9);
 } 
 //Less
 if(increasingNumber <  randomNo)
 {
    for(int segment =0;segment<7;segment++) 
    { 
    isbit = bitRead(compare[2],segment);
    digitalWrite(segmentPins[segment],isbit);
    }
    randomSeed(analogRead(A0));
    randomNo = random(1,9);
 } 
 
}

//start loop
void loop() {
  DebounceA();
  DebounceB();
  
  if(stateA == HIGH) 
  StepNumber();
  //Call function Checknum
  if(stateB == HIGH)
  Checknum();
  Serial.print("Random =");
  Serial.println(randomNo);
  
}
