/*
This is is Tim Quinn's temperature chamber control program,
'temp_chamber_main.ino'
*/

//include the Xively libraries
#include <CountingStream.h>
#include <Xively.h>
#include <XivelyClient.h>
#include <XivelyDatastream.h>
#include <XivelyFeed.h>

//setup for Xively client
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char xivelyKey[] = "YOUR_API_KEY";
char sensorId[] = "sensor_reading";
char bufferId[] = "info_message";

//set up the Xively datastreams
XivelyDatastream datastreams[] = {
XivelyDatastream(tempSet, strlen(tempSet), DATASTREAM_INT),
XivelyDatastream(tempMain, strlen(tempMain), DATASTREAM_FLOAT),
XivelyDatastream(tempIn, strlen(tempIn), DATASTREAM_FLOAT),
XivelyDatastream(tempOut, strlen(tempOut), DATASTREAM_FLOAT),
XivelyDatastream(fanRPM, strlen(fanRPM), DATASTREAM_FLOAT)
}

XivelyFeed feed(15552, datastreams, 5 /* number of datastreams */);

EthernetClient client;
XivelyClient xivelyclient(client);


// include the LCD library:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(31, 32, 36, 35, 34, 33);

//create degree symbol
byte degSign[8] = {0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00};

//setup for temp pins
int tempPinMain= A2;
int tempPinIn = A3;
int tempPinOut = A4;

int tempReadMain;
int tempReadIn;
int tempReadOut;

float tempVoltMain;
float tempVoltIn;
float tempVoldOut;

float tempMain;
float tempIn;
float tempOut;

int tempBuff;
int tempSet;

//fan monitoring setup
int fanPinRPM = A5;

void setup() {
  //set output pins
  pinMode(relaycontrol, OUTPUT); //digital pin - HIGH for on, LOW for off
  pinMode(PWMfan, OUTPUT); //PWM pin required
  pinMode(ColdLED, OUTPUT); //controls blue LED
  pinMode(GoodLED, OUTPUT); //controls green LED
  pinMode(HotLED, OUTPUT); //controls red LED
  analogReference(EXTERNAL); //apply 3.3V to Ref pin

  //initial temperature set point & buffer
  tempSet = 25;
  tempBuff = 1;
 
  // set up the LCD and print
  lcd.begin(16, 2);
  lcd.print("Temp: ");
  lcd.print(currentTemp); //insert variable for temp & string for degC
  lcd.setCursor(0,1);
  lcd.print("Set Point: ");
  lcd.print(setPoint); //insert variable for temp & string for degC
}

void loop() {
  //this code manipulates the temp voltages
  tempReadMain = analogRead(tempPinMain);
  tempReadIn = analogRead(tempPinIn);
  tempReadOut = analogRead(tempPinOut);
  tempVoltMain = (tempReadMain*aref_voltage)/1024.0;
  tempVoltIn = (tempReadIn*aref_voltage)/1024.0;
  tempVoltOut = (tempReadOut*aref_voltage)/1024.0;
  tempMain = (tempVoltMain - 0.5)*100;
  tempIn = (tempVoltIn - 0.5)*100;
  tempOut = (tempVoltOut - 0.5)*100;
  
  //this code prints the temp voltages to the LCD
  lcd.setCursor(6,0);
  lcd.print(tempMain);
  lcd.println(degSign);
  lcd.println("C");
  lcd.setCursor(11,1);
  lcd.print(setPoint);
  lcd.println(degSign);
  lcd.println("C");  
  
  //this code calculates fan RPM
  // ???????
  
  //this code sends RPM and temp data to web via Xively
  datastreams[0].setInt(tempSet);
  datastreams[1].setFloat(tempMain);
  datastreams[2].setFloat(tempIn);
  datastreams[3].setFloat(tempOut);
  datastreams[4].setFloat(fanRPM);
  
  //these are the button interrupts?
  // ????????
  
 
  //this begins the fan and heater control
  if (tempMain <= tempSet - tempBuff) {
    digitalWrite(relayControl, HIGH);
    analogWrite(PWMfan, 0);
  }
  else if (tempMain >= tempSet + tempBuff) {      
      digitalWrite(relayControl, LOW);
      tempMain = round(tempMain*10);
      tempSet = round(tempSet*10);
      int fan = tempMain - tempSet;
      fan = fan + 128;
      analogWrite(PWMfan, fan)
  }
  else if (tempSet - tempBuff < tempMain < tempSet + tempBuff){
    digitalWrite(relayControl, LOW);
    analogWrite(PWMfan, 0);
  }

}
