//MASTER CODE

#include <Wire.h>

int pushButton = A0;
int x = 0;

void setup()
{
  Wire.begin();
  pinMode(pushButton, INPUT);
}

void loop()
{
Wire.beginTransmission(1);
  x= digitalRead(pushButton);
  Wire.write(x);
  Wire.endTransmission();
  delay(500);
}

//SLAVE CODE 

#include <Wire.h>
int LED = 8;
int x = 0;

void setup()
{
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  
}

void loop()
{
delay(100);
}
 
void receiveEvent(int howMany){
    
x= Wire.read();
  
if(x==1){
    digitalWrite(LED, HIGH);
    Serial.print(1);}
else{
    digitalWrite(LED,LOW);
    Serial.print(0);}     
  
}
