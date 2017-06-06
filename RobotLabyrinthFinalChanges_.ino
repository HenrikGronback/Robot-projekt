#include <Servo.h>                                             // Include servo library

Servo servoLeft;                                               // Declare left and right servos
Servo servoRight;

int trigPinLeft = 2;                                           // declare left trig and echo pin
int echoPinLeft = 5;

int trigPinRight = 11;                                        // declare right trig and echo pin
int echoPinRight = 12;

int trigPinFront = 7;                                          // declare front trig and echo pin
int echoPinFront = 9;      

int inputReadingLeft [10]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //declare left array
int inputReadingRight [10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //declare right array
int inputReadingFront [10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //declare front array

int angles [20] = {0};                                          // declare angles array
int numForwards = 1;                                           // declare the numbers of forwards that has been called
int curAngle = 0;                                              // declare and states the current angle

int maxloop = (numForwards > 19) ? 19: numForwards;             // declare the maximun loop for the array
int hasTurned = 0;                                             // declare how many times the robot has turned

  
int meanLeft  = 0;                                             //declare mean left and right
int meanRight = 0;
int meanFront = 0;

int distToWall = 5;                                            // declare and sets distance to wall to 5 cm
int distDelta = 5;                                             // declare the distance delta

int i;

void setup()
{
  
  pinMode(trigPinLeft, OUTPUT);                                //sound emitter - ultrasonic sensor left
  pinMode(echoPinLeft, INPUT);                                 //ultrasonic distance reader left
  
  pinMode(trigPinRight, OUTPUT);                               //sound emitter - ultrasonic sensor Right
  pinMode(echoPinRight, INPUT);                                //ultrasonic distance reader right
  
  Serial.begin(9600);                                          // Set data rate to 9600 bps

  servoLeft.attach(11);                                        //attach servo left and right
  servoRight.attach(10);
}

void loop() 
{ 
  
  for (i = 0; i < 10; i++)
  { 
    inputReadingLeft[i]  = calculateDistanceLeft();             // adds data to left array 
    inputReadingRight[i] = calculateDistanceRight();            // adds data to right array
    inputReadingFront[i] = calculateDistanceFront();            //adds data to front array
   }
  
  meanFront = 0;                                                //sets the mean data to 0
  meanLeft  = 0;
  meanRight = 0;
  
  for (i = 0; i < 10; i++)
  {
    meanLeft  += inputReadingLeft[i];                            //adds the sensor values to the array
    meanRight += inputReadingRight[i];
    meanFront += inputReadingFront[i];
  }
  meanLeft  = meanLeft/10;                                        //divides the values to get a mean value
  meanRight = meanRight/10;
  meanFront = meanFront/10;
  
 if(meanFront < distToWall)                                        //if the front sensor is detection something
   {
     for(i = 0; i < maxloop; i++)                                   
     {
       if(abs(curAngle - angles[i]) > 30 )
       {
         hasTurned = curAngle - angles [i];
         break;
       }
     }
     if (hasTurned < 0 )                                             //if the previous turn has been to the left it turns to the right
     {
       turnRight (800);                                               //does a 90 degree turn
     }
     else if (hasTurned > 0)                                          //if the previous turn has been to the right it turns to the left                                               
     {
       turnLeft(800);                                                 //does a 90 degree turn
     }
   }
  
  if ((meanLeft < meanRight) && (meanRight - meanLeft > distDelta))    //if the left sensor value is less than the right and diffrernce is left than 5 cm
  {
    turnRight(10);                                                     //turn right for 10 microseconds
    Serial.println("turing Right");                                    //prints out "turning right" to the serial monitor
  }
  
  else if ((meanLeft > meanRight) && (meanLeft - meanRight > distDelta)) //if the right sensor value is less than the left and diffrernce is left than 5 cm
    {
      turnLeft(10);                                                      //turn left for 10 microseconds
   
      Serial.println("turing Left");                                     //prints out "turning left" to the serial monitor
    }
    
  else
  {    
     forward(10);                                                        //else go forward for 10 microseconds
     
     Serial.println("going Forward");                                     //prints out "going forward" to the serial monitor                                       
  }
    
  Serial.print("Meanleft: ");                                              
  Serial.print(meanLeft);                                                 //prints out the meanLeft value
  Serial.print("   ");
  Serial.print("MeanRight: ");
  Serial.println(meanRight);                                               //prints out the meanLeft value
  delay(100);                                                              // adds a delay between the prints
}


void forward(int time)                                                      //function to go foward
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
  delay(time);
  
   if(numForwards < 20) 
  {
    angles[numForwards] = curAngle;
  }
  else 
  {
    for (i = 0; i <19; i++)                                               //for every time the forward function is called the angles array is incresed
      {
        angles[i] = angles [i+1];
      }
      angles [19] = curAngle;
  }
  numForwards ++;
}
void turnLeft(int time)                                                    //function to turn left
{
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1300);
  delay(time);
  curAngle = curAngle - 1;                                                 // substracts 1 to the current angle if turn left is called
}
void turnRight(int time)                                                   // adds 1 to the current angle if turn Right is called
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1500);
  delay(time);
  curAngle = curAngle + 1;
}

unsigned long calculateDistanceLeft()                                     //function to calculate left distance
{
  digitalWrite(trigPinLeft, HIGH);                                        //Trigger ultrasonic detection 
  delayMicroseconds(10); 
  digitalWrite(trigPinLeft, LOW);
   
  unsigned long dist = pulseIn(echoPinLeft, HIGH);                        //Read ultrasonic reflection
  dist = dist/50;                                                         //Calculate distance in cm
  
  return dist;
  
}

unsigned long calculateDistanceRight()                                    //function to calculate right distance
{
  digitalWrite(trigPinRight, HIGH);                                       //Trigger ultrasonic detection 
  delayMicroseconds(10); 
  digitalWrite(trigPinRight, LOW);
   
  unsigned long dist = pulseIn(echoPinRight, HIGH);                       //Read ultrasonic reflection
  dist = dist/50;                                                         //Calculate distance in cm
  
  return dist;
}

unsigned long calculateDistanceFront()                                    //function to calculate front distance
{
  digitalWrite(trigPinFront, HIGH);                                       //Trigger ultrasonic detection 
  delayMicroseconds(10);
  digitalWrite(trigPinFront, LOW);
  
  unsigned long dist = pulseIn(echoPinFront, HIGH);                       //Read ultrasonic reflection
  dist = dist/50;                                                         //Calculate distance in cm
  
  return dist;
}
