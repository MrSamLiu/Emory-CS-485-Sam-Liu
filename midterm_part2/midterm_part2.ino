#include <avr/io.h>
#include <util/delay.h>

//define buttons to pins on nano
#define left_button A0
#define right_button A4
#define up_button 13
#define down_button A5

//define the initial positions
int random_x = 0;   
int random_y = 0;   
int controller_x = 2;
int controller_y = 2;

//define speaker pin connections
const int speakerPin = 13;

//define the column and row matrix connecting to pins on nano
const int columnPins[] = {8, 9, 10, 11, 12, A3, A2, A1};   
const int rowPins[] = {0, 1, 2, 3, 4, 5, 6, 7};  

//initialize the boolean tracker for button pressed
bool left_button_is_pressed = false;
bool right_button_is_pressed = false;
bool up_button_is_pressed = false;
bool down_button_is_pressed = false;

void setup() {
  //setting up buttons as inputs
  pinMode(left_button, INPUT_PULLUP);
  pinMode(right_button, INPUT_PULLUP);
  pinMode(up_button, INPUT_PULLUP);
  pinMode(down_button, INPUT_PULLUP);

  //library function for random generator
  randomSeed(analogRead(0));   

  for (int i = 0; i < 8; i++) {
    pinMode(columnPins[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(rowPins[i], OUTPUT);
  }

  random_x = random(0, 8);  
  random_y = random(0, 8);  

  DDRD = 0xFF;
}

void loop() {
  button_input();

  for (int i = 0; i < 8; i++)
    digitalWrite(columnPins[i], HIGH);  

  //display random dots and controller dot 
  random_dot_display(random_x, random_y);
  controller_dot_display(controller_x, controller_y);

  //speaker makes sound if two dots collide with each other
  if (controller_x == random_x && controller_y == random_y) {
    _delay_ms(500);   
    random_x = random(0, 8);   
    random_y = random(0, 8);
    tone(speakerPin, 500); 
    delay(1000);
    noTone(speakerPin);
  }
}

void button_input(){
  //check left button and changing position index
  if (digitalRead(left_button) == LOW && !left_button_is_pressed) {
    controller_x--;
    if (controller_x < 0)
      controller_x = 7;   
    left_button_is_pressed = true;
  }
  if (digitalRead(left_button) == HIGH && left_button_is_pressed) {
    left_button_is_pressed = false;
  }

  //check right button and changing position index
  if (digitalRead(right_button) == LOW && !right_button_is_pressed) {
    controller_x++;
    if (controller_x > 7) 
      controller_x = 0;
    right_button_is_pressed = true;
  }
  if (digitalRead(right_button) == HIGH && right_button_is_pressed) {
    right_button_is_pressed = false;
  }

  //check up button and changing position index
  if (digitalRead(up_button) == LOW && !up_button_is_pressed) {
    controller_y--;
    if (controller_y < 0) 
      controller_y = 7; 
    up_button_is_pressed = true;
  }
  if (digitalRead(up_button) == HIGH && up_button_is_pressed) {
    up_button_is_pressed = false;
  }

  //check down button and changing the position index
   if (digitalRead(down_button) == LOW && !down_button_is_pressed) {
    controller_y++;
    if (controller_y > 7) 
      controller_y = 1;  
    down_button_is_pressed = true;
  }
  if (digitalRead(down_button) == HIGH && down_button_is_pressed) {
    down_button_is_pressed = false;
  }
}

//display controller dot 
void controller_dot_display(int x, int y) {
  PORTD = (1 << y);   
  for (int column = 0; column < 8; column++) {
    if (column == x) {
      digitalWrite(columnPins[column], LOW);   
    } 
    else {
      digitalWrite(columnPins[column], HIGH);  
    }
  }
  _delay_ms(2);   
  PORTD = 0x00;   
}

//display stationary random dot
void random_dot_display(int x, int y) {
  PORTD = (1 << y);   
  for (int column = 0; column < 8; column++) {
    if (column == x) {
      digitalWrite(columnPins[column], LOW);   
    } 
    else {
      digitalWrite(columnPins[column], HIGH);   
    }
  }
  _delay_ms(2);   
  PORTD = 0x00;   
}



