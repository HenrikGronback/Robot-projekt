#include <Servo.h>
Servo servoLeft;
Servo servoRight;

byte wLeftOld;
byte wRightOld;
byte counter;

int left = 800;
int back = 1000;
int right = 400;
int front = 20;
void setup()
{
  pinMode(7, INPUT);
  pinMode(5, INPUT);
  tone(4, 3000, 1000);
  delay(1000);
  
  servoLeft.attach(11);
  servoRight.attach(10);
  
  wLeftOld = 0;
  wRightOld = 1;
  counter = 0;

}
void loop() 
{
  
  byte wLeft = digitalRead(5);
  byte wRight = digitalRead(7);
  
  if(wLeft != wRight)
  {
    if ((wLeft != wLeftOld) && (wRight != wRightOld))
    {
    counter++;
    wLeftOld = wLeft;
    wRightOld = wRight;
    if(counter == 4)
    {
      wLeft = 0;
      wRight = 0;
      counter = 0;
    } 
  }
                                    
  else 
  {
    counter = 0;
  }
 }

  
  if((wLeft == 0) && (wRight == 0))
  {
    backward(back);
    turnLeft(left);
  }
  else if(wLeft == 0)
  {
  backward(back);
  turnRight(right);
  }
  else if(wRight == 0)
  {
  backward(back);
  turnLeft(left);
  } 
  else
  {
  forward(front);
  } 
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
