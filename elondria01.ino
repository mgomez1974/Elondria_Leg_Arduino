// Test motor joint mode

#include "DynamixelMotor.h"

// direction pin, if you use tristate buffer
#define DIR_PIN 2

const int analogInPin1 = A0;
const int analogInPin3 = A1;
const int analogInPin5 = A2;
int sensorValue1 = 0;
int sensorValue3 = 0;
int sensorValue5 = 0;

double X1_Art1=0;     double Y1_Art1=1024;
double X2_Art1=1024;  double Y2_Art1=0;
double M_Art1=(Y2_Art1-Y1_Art1)/(X2_Art1-X1_Art1);      double B_Art1=Y1_Art1-(M_Art1*X1_Art1);

double X1_Art3=880;   double Y1_Art3=652;
double X2_Art3=970;   double Y2_Art3=752;
double M_Art3=(Y2_Art3-Y1_Art3)/(X2_Art3-X1_Art3);      double B_Art3=Y1_Art3-(M_Art3*X1_Art3);

double X1_Art5=670;   double Y1_Art5=350;
double X2_Art5=956;   double Y2_Art5=200;
double M_Art5=(Y2_Art5-Y1_Art5)/(X2_Art5-X1_Art5);      double B_Art5=Y1_Art5-(M_Art5*X1_Art5);

// speed, between 0 and 1023
int16_t speed=128;
// communication baudrate
const long unsigned int baudrate = 1000000;

// Use this for hardware serial with tristate buffer
HardwareDynamixelInterface interface(Serial, DIR_PIN);

DynamixelMotor motor1(interface, 1);
DynamixelMotor motor3(interface, 3);
DynamixelMotor motor5(interface, 5);

void setup()
{ 
  interface.begin(baudrate);
  delay(100);
  
  // check if we can communicate with the motor
  // if not, we turn the led on and stop here
  uint8_t status1=motor1.init();  if(status1!=DYN_STATUS_OK)    { pinMode(LED_BUILTIN, OUTPUT);     digitalWrite(LED_BUILTIN, HIGH);    while(1);  }
  uint8_t status3=motor3.init();  if(status3!=DYN_STATUS_OK)    { pinMode(LED_BUILTIN, OUTPUT);     digitalWrite(LED_BUILTIN, HIGH);    while(1);  }
  uint8_t status5=motor5.init();  if(status5!=DYN_STATUS_OK)    { pinMode(LED_BUILTIN, OUTPUT);     digitalWrite(LED_BUILTIN, HIGH);    while(1);  }

  motor1.enableTorque();  
  motor3.enableTorque();  
  motor5.enableTorque();  

  motor1.speed(speed);      if (Y1_Art1<Y2_Art1)  motor1.jointMode(Y1_Art1, Y2_Art1);   else  motor1.jointMode(Y2_Art1, Y1_Art1);
  motor3.speed(speed);      if (Y1_Art3<Y2_Art3)  motor3.jointMode(Y1_Art3, Y2_Art3);   else  motor3.jointMode(Y2_Art3, Y1_Art3);
  motor5.speed(speed);      if (Y1_Art5<Y2_Art5)  motor5.jointMode(Y1_Art5, Y2_Art5);   else  motor5.jointMode(Y2_Art5, Y1_Art5);
  
}

void loop() 
{
  sensorValue1 = analogRead(analogInPin1);     if (sensorValue1>X1_Art1&&sensorValue1<X2_Art1)         motor1.goalPosition((M_Art1*sensorValue1)+B_Art1);
  sensorValue3 = analogRead(analogInPin3);     if (sensorValue3>X1_Art3&&sensorValue3<X2_Art3)         motor3.goalPosition((M_Art3*sensorValue3)+B_Art3);
  sensorValue5 = analogRead(analogInPin5);     if (sensorValue5>X1_Art5&&sensorValue5<X2_Art5)         motor5.goalPosition((M_Art5*sensorValue5)+B_Art5);
}
