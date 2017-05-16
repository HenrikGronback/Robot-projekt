#include <Servo.h>                      // Include servo library

Servo servoLeft;                        // Declare left and right servos
Servo servoRight;

int trigPinLeft = 2;
int echoPinLeft = 5;

int trigPinRight = 11;
int echoPinRight = 12;

int inputReadingLeft [10]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int inputReadingRight [10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int index = 0;
  
int meanLeft  = 0;
int meanRight = 0;

int i;

void setup()
{
  
  pinMode(trigPinLeft, OUTPUT); //sound emitter - ultrasonic sensor
  pinMode(echoPinLeft, INPUT);  //ultrasonic distance reader 
  
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
  
  Serial.begin(9600);
  
  servoLeft.attach(11);
  servoRight.attach(10);
}

void loop() 
{ 
  for (i = 0; i < 10; i++)
  {
    
    inputReadingLeft[i]  = calculateDistanceLeft();
    inputReadingRight[i] = calculateDistanceRight();
 
  }
  
  for (i = 0; i < 10; i++)
  {
    meanLeft  += inputReadingLeft[i];
    meanRight += inputReadingRight[i];
  }
  meanLeft  = meanLeft/10;
  meanRight = meanRight/10;
  
  if (meanLeft < meanRight && (meanRight - meanLeft > 3))
  {
     turnRight(10);
  }
    else if (meanLeft > meanRight && (meanLeft - meanRight > 3))
    {
       turnLeft(10);
    }
    else
    {
       forward(10);
    }
    
  Serial.print("Distanceleft: ");
  Serial.println(meanLeft);
  Serial.print("   ");
  Serial.print("DistanceRight: ");
  Serial.print(meanRight);
  delay(100);
}


void forward(int time)
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
  delay(time);
}
void turnLeft(int time)
{
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1300);
  delay(time);
}
void turnRight(int time)
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1500);
  delay(time);
}
void backward(int time)
{
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700);
  delay(time);
}
unsigned long calculateDistanceLeft()
{
  digitalWrite(trigPinLeft, HIGH); //Trigger ultrasonic detection 
  delayMicroseconds(10); 
  digitalWrite(trigPinLeft, LOW);
   
  unsigned long dist = pulseIn(echoPinLeft, HIGH); //Read ultrasonic reflection
  dist = dist/50; //Calculate distance in cm
  
  return dist;
  
}
unsigned long calculateDistanceRight()
{
  digitalWrite(trigPinRight, HIGH); //Trigger ultrasonic detection 
  delayMicroseconds(10); 
  digitalWrite(trigPinRight, LOW);
   
  unsigned long dist = pulseIn(echoPinRight, HIGH); //Read ultrasonic reflection
  dist = dist/50; //Calculate distance in cm
  
  return dist;
}
