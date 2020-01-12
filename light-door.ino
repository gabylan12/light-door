
#include <AsyncDelay.h>
/*
 * STATUS
 *                 (evaluate each 10 seconds)
 *                 lightSensor < LIGHT_LEVEL
 *                   ----------------->  
 *   NATURAL_LIGHT                        DARKNESS (light off)
      (light off)   <------------------             
                  lightSensor >= LIGHT_LEVEL    |   | 
 *                                              |   | 
 *                                     timeout^ |   |  pirStatus=HIGH        
 *                                              |   |   
 *                                      ACTIVITY (for 45 seconds, if pirStatus=HIGH reset timeout)
 *                                        (light on)   
 */


/** CONSTANTS */
const int RELAY = 2;
const int PIR = 3;
const int PHOTOCELL = A0;
const int LIGHT_LEVEL = 30;
const long INTERVAL = 45000;


/** state machine */
const int NATURAL_LIGHT = 0;
const int DARKNESS = 1; 
const int ACTIVITY = 2;
int state = DARKNESS;
unsigned long currentMillis;
AsyncDelay readPhotocell;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(PHOTOCELL, INPUT);
  pinMode(PIR, INPUT);
  digitalWrite(RELAY, LOW); 
  digitalWrite(LED_BUILTIN, LOW);   
  
  //PHOTOCELL
  readPhotocell.start(10000, AsyncDelay::MILLIS);
  Serial.begin(9600);
  Serial.println("START");
}

// the loop function runs over and over again forever
void loop() {


  //process photocell
  if (readPhotocell.isExpired() && state != ACTIVITY ) {
    int photocellReading  = map(analogRead(PHOTOCELL), 0, 1023, 0, 100);
    Serial.println(photocellReading);
    if(photocellReading > LIGHT_LEVEL){
      Serial.println("NATURAL_LIGHT");
      state = NATURAL_LIGHT;
    }
    else{
      Serial.println("DARKNESS");
      state = DARKNESS;
    }
    readPhotocell.repeat();
  }

  //process pir
  if(state !=NATURAL_LIGHT){
    int pirStatus = digitalRead(PIR);
    switch(state){
      case DARKNESS:{
        if (pirStatus == HIGH) {
          currentMillis = millis();
          Serial.println("ACTIVITY");
          state = ACTIVITY;
          digitalWrite(LED_BUILTIN, HIGH); // TURN ON LIGHT
          digitalWrite(RELAY, HIGH);  
          delay(3000);
        }
      }
      break;
      
      case ACTIVITY:{
        if(pirStatus == HIGH){
          currentMillis = millis();
          Serial.println("reset");
        }
        else if(millis() > currentMillis + INTERVAL){
          Serial.println("DARKNESS");
          state = DARKNESS;
          digitalWrite(LED_BUILTIN, LOW); // TURN OFF LIGHT
          digitalWrite(RELAY, LOW);  
        }
      }
      break;
    }
  }
    
   
  

  delay(500);
}
