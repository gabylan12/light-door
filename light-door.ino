/*
 * STATUS
 *                      30 seconds without pirStatus = HIGH
 *   WITHOUT_ACTIVITY <------------------  
 *         |                             | 
 *         | pirStatus=HIGH &&           |          
 *         | lightSensor >= LIGHT_LEVEL  |   
 *          --------------------> ACTIVITY_DETECTED 
 *                                           
 */


/** CONSTANTS */
const int RELAY = 2;
const int SENSOR_LIGHT = A0;
const int LIGHT_LEVEL = 500;
const long INTERVAL = 60000;


/** state machine */

const int WITHOUT_ACTIVITY = 0;
const int ACTIVITY_DETECTED = 1; 
const int DOOR_CLOSED = 2;
const int DOOR_CLOSED_ACTIVITY = 3;
int state = WITHOUT_ACTIVITY;
unsigned long currentMillis;



// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(SENSOR_LIGHT, INPUT);
  digitalWrite(RELAY, LOW); 
  digitalWrite(LED_BUILTIN, LOW);   
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  int lightSensor = analogRead(SENSOR_LIGHT );
  switch(state){
    case WITHOUT_ACTIVITY:{
      //if (pirStatus == HIGH) {
      if (lightSensor >= LIGHT_LEVEL) {
        Serial.println("ACTIVITY_DETECTED");
        state = ACTIVITY_DETECTED;
        currentMillis = millis();
        digitalWrite(LED_BUILTIN, HIGH); // TURN ON LIGHT
        digitalWrite(RELAY, HIGH);  
        delay(3000);
      } 
    }
    break;
    
    case ACTIVITY_DETECTED:{
//      if(pirStatus == HIGH){
      if(false){
        currentMillis = millis();
      }
      else if(millis() > currentMillis + INTERVAL){
        Serial.println("WITHOUT_ACTIVITY");
        state = WITHOUT_ACTIVITY;
        digitalWrite(LED_BUILTIN, LOW); // TURN OFF LIGHT
        digitalWrite(RELAY, LOW);  
      }
    }
    break;
  }
    
   
  

  delay(500);
}
