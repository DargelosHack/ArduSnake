//--------------------------------------------------------------
//-- Different oscillations for unimod
//-- When the test button is pressed, the oscillation is changed
//-- The phase different between the two modules is changed
//--------------------------------------------------------------
//-- (c) Juan Gonzalez-Gomez (Obijuan), Jul-2013
//-- GPL license
//--------------------------------------------------------------
#include <Servo.h>
#include <Oscillator.h>
#include "skymega.h"

const int T = 2000;
const int A = 40;
const int O = 0;
const double Ph[] = {0.0, 180.0, 90.0};

//-- Number of elements in the sequence
int seq_size = sizeof(Ph)/sizeof(double);
int seq = 0;  //-- Sequence counter

//-- Declare two oscillators
Oscillator osc[2];

void setup()
{
  //-- Attach the oscillator to the servo
  //-- For arduino, you can use the pin number instead of SERVO2 (for example 8)
  osc[0].attach(SERVO2);
  osc[1].attach(SERVO4);
  
  // The led is an output
  pinMode(LED, OUTPUT);      
  
  //-- The button is an input
  pinMode(BUTTON, INPUT);
  //-- Activate the button pull-up resistor
  digitalWrite(BUTTON, HIGH); 

  //-- Serial port is used for debuging
  Serial.begin(9600); 

  //-- Set the initial oscillation
  osc[0].SetO(O);
  osc[0].SetA(A);
  osc[0].SetT(T);
  osc[0].SetPh(0);

  osc[1].SetO(O);
  osc[1].SetA(A);
  osc[1].SetT(T);
  osc[1].SetPh(DEG2RAD(Ph[seq]));
  
}

//-- This function returns true when the test button
//-- is pressed
bool button_clicked()
{
  
  static int button=1;
  static int oldbutton=1;
  
  //-- Save the previous button state
  oldbutton=button;
  
  // read the button
  button = digitalRead(BUTTON);

  //-- Detect a change in the button
  if (button!=oldbutton) {
    delay(20); //-- for debouncing
    if (button==PRESSED)
      return true;
  } 
  
  return false;
}

//-- This function returns true when the time "time"
//-- has passed since the last call
bool timeout(long time)
{
  static long previousMillis = 0; 
  static long currentMillis;
  
  //-- Read current time
  currentMillis = millis();
 
  //-- Check if the timeout has passed
  if(currentMillis - previousMillis > time) {
    previousMillis = currentMillis;   

    return true;
  }
  
  return false;
}

//-- Led state
int ledState = OFF;

//-- Led blinking period, in ms
long period = 1000;

//-- The servo will oscillate with the
//-- default parameters
void loop()
{
  //-- Refresh the oscillators
  for (int i=0; i<2; i++)
    osc[i].refresh();
  
  if (button_clicked()) {
    Serial.println("clicked!");
    
    //-- Point to the next sequence
    seq = (seq + 1) % seq_size;
    Serial.println(seq, DEC);
    
    //-- Update the oscillator with the new sequence
    osc[1].SetPh(DEG2RAD(Ph[seq]));
  }
}


