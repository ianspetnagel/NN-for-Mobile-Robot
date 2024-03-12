/*
 keyestudio Mini Tank Robot v2.0
 lesson 11
 ultrasonic_avoid_tank
 http://www.keyestudio.com
*/
#include<avr/wdt.h>


int lightOne = A1;
int lightTwo = A2;




//Code for the LED Screen
unsigned char smile[] = {0x00, 0x00, 0x00, 0x2e, 0x2a, 0x4e, 0x40, 0x40, 0x40, 0x40, 0x4e, 0x2a, 0x2e, 0x00, 0x00, 0x00};

unsigned char sleep[] = {0x00, 0x00, 0x00, 0x44, 0x44, 0x24, 0x20, 0x20, 0x20, 0x20, 0x24, 0x44, 0x44, 0x00, 0x00, 0x00};

unsigned char wdt1[] = {0x00, 0x3c, 0x20, 0x10, 0x20, 0x3c, 0x00, 0x3c, 0x24, 0x18, 0x00, 0x04, 0x3c, 0x04, 0x00, 0x00};

unsigned char wdt2[] = {0x00, 0x1e, 0x20, 0x10, 0x20, 0x1e, 0x00, 0x3e, 0x22, 0x1c, 0x00, 0x02, 0x3e, 0x02, 0x00, 0x2e};

unsigned char int1[] = {0x00, 0x00, 0x22, 0x3e, 0x22, 0x00, 0x3e, 0x04, 0x08, 0x3e, 0x00, 0x02, 0x3e, 0x02, 0x00, 0x00};

#define SCL_Pin  A5  //Set clock pin to A5
#define SDA_Pin  A4  //Set data pin to A4



int random2;
int a;
int a1;
int a2;
#define ML_Ctrl 13  //define the direction control pin of left motor
#define ML_PWM 11   //define PWM control pin of left motor
#define MR_Ctrl 12  //define the direction control pin of right motor
#define MR_PWM 3   //define PWM control pin of right motor

#define Trig 5  //ultrasonic trig Pin
#define Echo 4  //ultrasonic echo Pin
int distance;
#define servoPin 9  //servo Pin
int pulsewidth;

/************the function to run motor**************/
void Car_front()
{
  digitalWrite(MR_Ctrl,LOW);
  analogWrite(MR_PWM,200);
  digitalWrite(ML_Ctrl,LOW);
  analogWrite(ML_PWM,200);
}
void Car_back()
{
  digitalWrite(MR_Ctrl,HIGH);
  analogWrite(MR_PWM,200);
  digitalWrite(ML_Ctrl,HIGH);
  analogWrite(ML_PWM,200);
}
void Car_left()
{
  digitalWrite(MR_Ctrl,LOW);
  analogWrite(MR_PWM,255);
  digitalWrite(ML_Ctrl,HIGH);
  analogWrite(ML_PWM,255);
}
void Car_right()
{
  digitalWrite(MR_Ctrl,HIGH);
  analogWrite(MR_PWM,255);
  digitalWrite(ML_Ctrl,LOW);
  analogWrite(ML_PWM,255);
}
void Car_Stop()
{
  digitalWrite(MR_Ctrl,LOW);
  analogWrite(MR_PWM,0);
  digitalWrite(ML_Ctrl,LOW);
  analogWrite(ML_PWM,0);
}

//The function to control servo
void procedure(int myangle) {
  for (int i = 0; i <= 50; i = i + (1)) {
    pulsewidth = myangle * 11 + 500;
    digitalWrite(servoPin,HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servoPin,LOW);
    delay((20 - pulsewidth / 1000));
  }
}
//The function to control ultrasonic sensor
float checkdistance() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  float distance = pulseIn(Echo, HIGH) / 100.00;  //58.20, that is, 2*29.1=58.2
  delay(10);
  return distance;
}
uint8_t timer = 1;
  //****************************************************************
void setup(){
  //Interrupt 
  //attachInterrupt(digitalPinToInterrupt(lightOne),ifun, FALLING);
  //attachInterrupt(analogPinToInterrupt(A2),ifun, FALLING);

  Serial.begin(9600);

  
  //CODE FOR LED SCREEN
  //Set pin to output
  pinMode(SCL_Pin,OUTPUT);
  pinMode(SDA_Pin,OUTPUT);
  //clear the display
  //matrix_display(clear);
  pinMode(7, INPUT_PULLUP);
   
  pinMode(servoPin, OUTPUT);
  procedure(90); //set servo to 90°
  
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(ML_Ctrl, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  pinMode(MR_Ctrl, OUTPUT);
  pinMode(MR_PWM, OUTPUT);

  //wdt_enable(timer); 
}
void loop(){
  //For LED 
  matrix_display(smile);  // display smile face

  //code for interrupt
  int data = digitalRead(7);
  Serial.print(data);
  Serial.print("--");
  
   int sensorValue0 = analogRead(lightTwo);  // read the value from the sensor
   int sensorValue1 = analogRead(lightOne);

   Serial.print(sensorValue0);
   Serial.print("--");
   Serial.println(sensorValue1);  

   int sensorValue = ((sensorValue0 + sensorValue1) / 2); 
  while (sensorValue < 500){
  
    Car_Stop();
    matrix_display(sleep);  // display sleep face
    delay(5000);
    break;

  }
    //Code for WDT
    
  if (sensorValue < 500)
  {
    
    matrix_display(wdt1);
    delay(3000);
    matrix_display(wdt2);
    delay(3000);
    ifun();
  } 
  else
  {     
    random2 = random(1, 100);
  a = checkdistance();  //assign the front distance detected by ultrasonic sensor to variable a
  
  if (a < 20) //when the front distance detected is less than 20 
  {
      Car_Stop();  //robot stops
      delay(500); //delay in 500ms
      procedure(150);  //Ultrasonic platform turns left
      for (int j = 1; j <= 10; j = j + (1)) { //for statement, the data will be more accurate if ultrasonic sensor detect a few times.
        a1 = checkdistance();  //assign the left distance detected by ultrasonic sensor to variable a1
      }
      delay(300);
      procedure(75); //Ultrasonic platform turns right
      for (int k = 1; k <= 10; k = k + (1)) {
        a2 = checkdistance(); //assign the right distance detected by ultrasonic sensor to variable a2
      }
      
      if (a1 < 50 || a2 < 50)  //robot will turn to the longer distance side when left or right distance is less than 50cm. 
      {
        if (a1 > a2) //left distance is greater than right side      
        {
          procedure(90);  //Ultrasonic platform turns back to right ahead         
          Car_left();  //robot turns left
          delay(500);  //turn left for 500ms
          Car_front(); //go front
        } 
        else 
        {
          procedure(90);
          Car_right(); //robot turns right
          delay(500);
          Car_front();  //go front
        }
      } 
      else  //If both side is greater than or equal to 50cm, turn left or right randomly
      {
        if ((long) (random2) % (long) (2) == 0)  //When the random number is even
        {
          procedure(90);
          Car_left(); //tank robot turns left
          delay(500);
          Car_front(); //go front
        } 
        else 
        {
          procedure(90);
          Car_right(); //robot turns right
          delay(500);
          Car_front(); //go front
       }
     }
  } 
  else  //If the front distance is greater than or equal to 20cm, robot car will go front
  {
      Car_front(); //go front
  }
  }
  delay(500);



  
}


//Functions for LED Screen


//Func for interrupt


void ifun()
{
  Car_Stop();
  matrix_display(int1);
  delay(1500);
}

void matrix_display(unsigned char matrix_value[])
{
  IIC_start();  // use the function of the data transmission start condition
  IIC_send(0xc0);  //select address
  
  for(int i = 0;i < 16;i++) //pattern data has 16 bits
  {
     IIC_send(matrix_value[i]); //convey the pattern data
  }

  IIC_end();   //end the transmission of pattern data  
  IIC_start();
  IIC_send(0x8A);  //display control, set pulse width to 4/16 s
  IIC_end();
}

//the condition to start conveying data
void IIC_start()
{
  digitalWrite(SCL_Pin,HIGH);
  delayMicroseconds(3);
  digitalWrite(SDA_Pin,HIGH);
  delayMicroseconds(3);
  digitalWrite(SDA_Pin,LOW);
  delayMicroseconds(3);
}
//Convey data
void IIC_send(unsigned char send_data)
{
  for(char i = 0;i < 8;i++)  //Each byte has 8 bits 8bit for every character
  {
      digitalWrite(SCL_Pin,LOW);  // pull down clock pin SCL_Pin to change the signal of SDA
      delayMicroseconds(3);
      if(send_data & 0x01)  //set high and low level of SDA_Pin according to 1 or 0 of every bit
      {
        digitalWrite(SDA_Pin,HIGH);
      }
      else
      {
        digitalWrite(SDA_Pin,LOW);
      }
      delayMicroseconds(3);
      digitalWrite(SCL_Pin,HIGH); //pull up the clock pin SCL_Pin to stop transmission
      delayMicroseconds(3);
      send_data = send_data >> 1;  // detect bit by bit, shift the data to the right by one
  }
}

//The sign of ending data transmission
void IIC_end()
{
  digitalWrite(SCL_Pin,LOW);
  delayMicroseconds(3);
  digitalWrite(SDA_Pin,LOW);
  delayMicroseconds(3);
  digitalWrite(SCL_Pin,HIGH);
  delayMicroseconds(3);
  digitalWrite(SDA_Pin,HIGH);
  delayMicroseconds(3);
}
//******************************************************


 //****************************************************************

