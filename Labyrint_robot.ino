#include <Servo.h>                      // Include servo library

long frequencyInHz = 38000;

int pinLeftEmitter = 8;
int pinLeftReciever = 4;

int pinRightEmitter = 6;
int pinRightReciever = 5;

int pinLedLeft = 2; 
int pinLedRight = 7;

Servo servoLeft;                        // Declare left and right servos
Servo servoRight;
int trigPin = 3;
int echoPin = 10;

int distanceLimit = 15; // cm

void setup()
{
  pinMode(pinLeftEmitter, OUTPUT); 
  pinMode(pinLeftReciever, INPUT); 
  pinMode(pinRightEmitter, OUTPUT); 
  pinMode(pinRightReciever, INPUT); 
  
  pinMode(pinLedLeft, OUTPUT); //2-led left
  pinMode(pinLedRight, OUTPUT);  //7-led Right
  
  pinMode(trigPin, OUTPUT); //sound emitter - ultrasonic sensor
  pinMode(echoPin, INPUT);  //ultrasonic distance reader 
  
  Serial.begin(9600);
  
  servoLeft.attach(13);
  servoRight.attach(12);
}

void loop() 
{

  unsigned long distance = calculateDistance();
   int irLeft = irDetect(pinLeftEmitter, pinLeftReciever, frequencyInHz);
   int irRight = irDetect(pinRightEmitter, pinRightReciever, frequencyInHz);
   
  if(distance < distanceLimit)
  {
    
    
    if(0 == irLeft)
    {
       turnRight(10);
    }
    else if (0 == irRight)
    {
      turnLeft(10);
    }
    else
    {
      turnRight(10);
    }
  }
  else 
  {
    forward(10);
  }
 
       
  digitalWrite(pinLedLeft, !irLeft);
  digitalWrite(pinLedRight, !irRight);
  
  Serial.print("Left: ");
  Serial.print(irLeft);
  Serial.print("  ");
  Serial.print("Right: ");
  Serial.print(irRight);
  Serial.print("  ");
  Serial.print("Distance: ");
  Serial.println(distance);
}


int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);         // IRLED 38 kHz for at least 1 ms
  delay(1);                             // Wait 1 ms
  int ir = digitalRead(irReceiverPin);  // IR receiver -> ir variable
  delay(1);                             // Down time before recheck
  return ir;                            // Return 1 no detect, 0 detect
}
void forward(int time)
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
  delay(time);
}
void turnLeft(int time)
{
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1300);
  delay(time);
}
void turnRight(int time)
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1700);
  delay(time);
}
void backward(int time)
{
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700);
  delay(time);
}
unsigned long calculateDistance()
{
  digitalWrite(trigPin, HIGH); //Trigger ultrasonic detection 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
   
  unsigned long dist = pulseIn(echoPin, HIGH); //Read ultrasonic reflection
  dist = dist/58; //Calculate distance in cm
  
  if((dist > 400) || (dist <= 0))
    {
      dist = 100;
    }
 return dist;
}
  
  


