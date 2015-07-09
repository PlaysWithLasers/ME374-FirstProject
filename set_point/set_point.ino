//display the temperature on a LCD

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(30,29,28,27,26,25);

//create the degree symbol
byte degSign[8] = {
0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00
};


//define the pin constants
int ledPin = 22;
int button = 23;
int button2 = 31;
int relay = 24;
int temp = 0;

//create the set point variable and initilize it
int setPoint = 25.;

//Button state variables

int buttonOneState;             // the current reading from the input pin
int lastButtonOneState = LOW;   // the previous reading from the input pin
int buttonTwoState;             // the current reading from the input pin
int lastButtonTwoState = LOW;   // the previous reading from the input pin

//Debounce variables
long lastDebounceTimeOne = 0;  // the last time the output pin was toggled
long lastDebounceTimeTwo = 0;  // the last time the output pin was toggled
int debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup(){
  
 //set analog reference to external
 analogReference(EXTERNAL);
 
  //create the custom character
    // create a new character
  lcd.createChar(1, degSign);
  
  
  //set the led pin to output
  pinMode(ledPin, OUTPUT);
  
  //set the button to input
  pinMode(button, INPUT);
  
  //set the second button to input
  pinMode(button2, INPUT);
  
  //set the relay to output
  pinMode(relay, OUTPUT);
  
  //set the temperature sensor to input
  pinMode(temp,INPUT);
  
  //Initilize serial communication
  Serial.begin(57600);
  
   // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  
}

void loop(){
 
//read the state of button 1
  int readingOne = digitalRead(button);
    
  // If the switch changed, due to noise or pressing:
  if (readingOne != lastButtonOneState) {
    // reset the debouncing timer
    lastDebounceTimeOne = millis();
  } 
  
  if ((millis() - lastDebounceTimeOne) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonOneState = readingOne;

  }
  
 

   //read the state of button 1
  int readingTwo = digitalRead(button2);
    
  // If the switch changed, due to noise or pressing:
  if (readingTwo != lastButtonTwoState) {
    // reset the debouncing timer
    lastDebounceTimeTwo = millis();
  } 
  
  if ((millis() - lastDebounceTimeTwo) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonTwoState = readingTwo;

}
  if(!buttonOneState && lastButtonOneState){

    setPoint++;
    buttonOneState = !buttonOneState;
  }
  else if(!buttonTwoState && lastButtonTwoState){

    setPoint--;
    buttonTwoState = !buttonTwoState;
    
  }
  
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonOneState = readingOne;
  
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonTwoState = readingTwo;
  
  
 
  //read the temperature sensor
  int tempRead = analogRead(temp);
  
  //convert to a voltage
  double tempVolt = tempRead*5./1024.;
  
  //convert to degrees Celsius
  double temperature = (tempVolt-.5)*100.;
  
  //Print the temperature over serial
  Serial.println(temperature);
  
  
  //Set the lcd cursor to the start of the 1st line
  lcd.setCursor(0,0);

  //print the headline:
  lcd.print("Temp:");
  
  //Display the temperature
  lcd.print(temperature);
  
  //Print out our custom degree symbol
  lcd.write(1);
  
  //Print the unites
  lcd.print("C");
  
  //move to the second line
  lcd.setCursor(0,1);
  
  //print the heading
  lcd.print("Set Point:");
  
  //print the value of the set point
  lcd.print(setPoint);
  
  //print the degree symbol
  lcd.write(1);
  
  //print the units
  lcd.print("C ");
  

}
