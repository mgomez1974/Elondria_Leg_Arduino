#include <Adafruit_ILI9340.h>

// Test motor joint mode

#include "DynamixelMotor.h"

// direction pin, if you use tristate buffer
#define DIR_PIN 2

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

const int analogInPin1 = A0;
const int analogInPin3 = A1;
const int analogInPin5 = A2;
int sensorValue1 = 0;
int sensorValue3 = 0;
int sensorValue5 = 0;
int counter_refresh=0;

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
  tft.begin();
  tft.fillScreen(ILI9340_BLACK);
  tft.setTextColor(ILI9340_GREEN);
  tft.setTextSize(3);
  tft.setCursor(0, 0);  
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
  tft.setTextColor(ILI9340_GREEN);
  tft.setTextSize(1);
  tft.setCursor(0, 0);  tft.print("X1,Y1  ");
  counter_refresh=0;
}

void loop() 
{
  double Y_Art1;
  double Y_Art3;
  double Y_Art5;
  static double Y_Art1_Old,sensorValue1_Old;
  static double Y_Art3_Old,sensorValue3_Old;
  static double Y_Art5_Old,sensorValue5_Old;

  Y_Art1=(M_Art1*sensorValue1)+B_Art1;
  Y_Art3=(M_Art3*sensorValue3)+B_Art3;
  Y_Art5=(M_Art5*sensorValue5)+B_Art5;
  sensorValue1 = analogRead(analogInPin1);     if (sensorValue1>X1_Art1&&sensorValue1<X2_Art1)         motor1.goalPosition(Y_Art1);
  sensorValue3 = analogRead(analogInPin3);     if (sensorValue3>X1_Art3&&sensorValue3<X2_Art3)         motor3.goalPosition(Y_Art3);
  sensorValue5 = analogRead(analogInPin5);     if (sensorValue5>X1_Art5&&sensorValue5<X2_Art5)         motor5.goalPosition(Y_Art5);
  //tft.fillRect(24, 0, 40, 8, ILI9340_RED);


  if (counter_refresh >30)
  {
    counter_refresh=0;
  tft.setTextSize(1);
  tft.setTextColor(ILI9340_BLACK);
                                            tft.setCursor(50, 0); tft.print(sensorValue1_Old);                                           tft.setCursor(100, 0); tft.print(Y_Art1_Old); 
                                            tft.setCursor(50, 100); tft.print(sensorValue3_Old);                                          tft.setCursor(100, 100); tft.print(Y_Art3_Old); 
                                            tft.setCursor(50, 200); tft.print(sensorValue5_Old);                                          tft.setCursor(100, 200); tft.print(Y_Art5_Old); 
  //tft.setTextSize(1);
  tft.setTextColor(ILI9340_GREEN);
                                            tft.setCursor(50, 0);  tft.print(sensorValue1);                                              tft.setCursor(100, 0); tft.print(Y_Art1); 
                                            tft.setCursor(50, 100); tft.print(sensorValue3);                                              tft.setCursor(100, 100); tft.print(Y_Art3); 
                                            tft.setCursor(50, 200); tft.print(sensorValue5);                                              tft.setCursor(100, 200); tft.print(Y_Art5); 
  Y_Art1_Old=Y_Art1;
  Y_Art3_Old=Y_Art3;
  Y_Art5_Old=Y_Art5;
  sensorValue1_Old=sensorValue1;
  sensorValue3_Old=sensorValue3;
  sensorValue5_Old=sensorValue5;                                            
  }
  counter_refresh++;


}
