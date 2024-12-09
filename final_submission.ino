#include <avr/io.h>
#include <util/delay.h>

//define the led pin connection
#define green_led 9
#define yellow_led 8   
#define red_led 7   

//define the button connection and the air sensor connection
#define right_button A0  
#define left_button A4    
const int sensorPin = A5;   

//button status 
bool rightButtonPressed = false;
bool leftButtonPressed = false;

//PM concentration status that sets up the Shinyei sensor
unsigned long lowPulseDuration = 0;   
unsigned long measurementStartTime = 0;   
const unsigned long measurementInterval = 30000;  
float concentration = 0;

//display status that can change between different status when the buttons are pressed
//1) The display of the PM concentration
//2) the dispaly of the AQI index that converted from the PM concentration based on standard conversion
//3) the display of the description of the AQI index - such as "the air quality is good" - based on standard defination 
int displayState = 0;  

//setup function
void setup() {
  pinMode(right_button, INPUT_PULLUP);
  pinMode(left_button, INPUT_PULLUP);

  pinMode(green_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(red_led, OUTPUT);

  digitalWrite(green_led, HIGH);
  digitalWrite(yellow_led, LOW);
  digitalWrite(red_led, LOW);
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);

  measurementStartTime = millis();
}

void loop() {

  //1) the program mesuares the PM concentration using the Shinyei sensor
  //2) the program will then upddates the color of the led light - green represents good, yellow mean moderate, and red means severe
  //3) the program checks whether the button is pressed
  //4) the program then display the status according to the button status
  //5) display the information on the serial monitor
  measure_PM_concentration();

  update_led_light();

  check_pressed_buttons();

  dispaly_current_aqi_status(displayState);
}

//check if the button is pressed, if pressed, update immediately 
//the displayState is used to calculate the number that represents each descriptive status when the button pressed
//the dispalyState will loop between 0, 1, 2 that can represent different descriptive status: 0 represents displaying the AQI index, 1 represents the AQI status, 2 represents the display of the PM concentration
void check_pressed_buttons() {
  if (digitalRead(right_button) == LOW && !rightButtonPressed) {
    displayState = (displayState + 1) % 3;  
    rightButtonPressed = true;
    displayInfo(displayState);   
    delay(150);  
  }

  else if (digitalRead(right_button) == HIGH && rightButtonPressed) {
    rightButtonPressed = false;
  }

  else if (digitalRead(left_button) == LOW && !leftButtonPressed) {
    displayState = (displayState - 1 + 3) % 3;  
    leftButtonPressed = true;
    displayInfo(displayState);   
    delay(150);  
  }

  else (digitalRead(left_button) == HIGH && leftButtonPressed) {
    leftButtonPressed = false;
  }
}


//measure PM concentration using the Shinyei sensor connection
void measure_PM_concentration() {
  //connecting to the sensorPin defined above that connects the Shinyei sensor to Pin A5
  unsigned long pulseDuration = pulseIn(sensorPin, LOW);
  lowPulseDuration += pulseDuration;

  if (millis() - measurementStartTime >= measurementInterval) {
    float occupancyRatio = lowPulseDuration / (measurementInterval * 1000.0);  
    concentration = 1.1 * pow(occupancyRatio, 3) 
                  - 3.8 * pow(occupancyRatio, 2) 
                  + 520 * occupancyRatio 
                  + 0.62;

    lowPulseDuration = 0;
    measurementStartTime = millis();
  }
}

//converting the PM concentration to the AQI index
int calculateAQI(float pmConcentration) {
  //the mapping is based on the standard concentration-AQI conversion in the industry 
  //1) the Shinyei sensor produces the PM concentration
  //2) it passes the concentration value to this function
  //3) the function converts the concentration value to AQI
  if (pmConcentration <= 12) {
    return map(pmConcentration, 0, 12, 0, 50);  
  } else if (pmConcentration <= 35.4) {
    return map(pmConcentration, 12.1, 35.4, 51, 100);  
  } else if (pmConcentration <= 55.4) {
    return map(pmConcentration, 35.5, 55.4, 101, 150);   
  } else if (pmConcentration <= 150.4) {
    return map(pmConcentration, 55.5, 150.4, 151, 200);   
  } else if (pmConcentration <= 250.4) {
    return map(pmConcentration, 150.5, 250.4, 201, 300);   
  } else {
    return map(pmConcentration, 250.5, 500.4, 301, 500);  
  }
}

//update LEDs based on AQI status: green means good, yellow is moderate, and red is severe air quality
void update_led_light() {
  int aqi = calculateAQI(concentration);

  if (aqi <= 50) {
    //good air quality
    digitalWrite(green_led, HIGH);
    digitalWrite(yellow_led, LOW);
    digitalWrite(red_led, LOW);
  } else if (aqi <= 100) {
    //moderate air quality
    digitalWrite(green_led, LOW);
    digitalWrite(yellow_led, HIGH);
    digitalWrite(red_led, LOW);
  } else {
    //unhealthy to hazardous air quality
    digitalWrite(green_led, LOW);
    digitalWrite(yellow_led, LOW);
    digitalWrite(red_led, HIGH);
  }
}

//get descriptive line based on AQI status according to standard descriptive line in the industry
String descriptive_line_aqi(int aqi) {
  if (aqi <= 50) {
    return "Good Air Quality";
  } else if (aqi <= 100) {
    return "Moderate Air Quality";
  } else if (aqi <= 150) {
    return "Unhealthy for Sensitive Groups";
  } else if (aqi <= 200) {
    return "Unhealthy Air Quality";
  } else if (aqi <= 300) {
    return "Very Unhealthy Air Quality";
  } else {
    return "Hazardous Air Quality";
  }
}

//display information based on the current state in a serial on the serial monitor
void dispaly_current_aqi_status(int state) {
  int aqi = calculateAQI(concentration);
  //the following display is based on the button status
  if (state == 0) {
    //aqi index dispalying
    Serial.print("AQI Index: ");
    Serial.println(aqi);

  } else if (state == 1) {
    //descriptive line displaying
    String description = descriptive_line_aqi(aqi);
    Serial.print("Description: ");
    Serial.println(description);

  } else if (state == 2) {
    //PM concentration displaying
    Serial.print("PM Concentration is: ");
    Serial.println(concentration, 2);
  }
}
