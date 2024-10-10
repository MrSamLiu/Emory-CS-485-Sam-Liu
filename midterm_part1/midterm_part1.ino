#include <avr/io.h>
#include <util/delay.h>

//define the column pins
const int column_pins[] = {8, 9, 10, 11, 12, A3, A2, A1};;

//All display pattern for part 1 including the combined OK and names:

//given pattern for O in OK
const byte O_pattern[8]{
  0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E
};

//given pattern for K in OK
const byte K_pattern[8]{
  0x63, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x63, 0x61
};

//define matrix for S
const byte S_pattern[8]{
  0x3C, 0x66, 0x60, 0x3C, 0x06, 0x66, 0x3C, 0x00    
};

//define matrix for A
const byte A_pattern[8]{
  0x3C, 0x66, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0x00
};

//define matrix for M
const byte M_pattern[8]{
  0xC3, 0xE7, 0xFF, 0xDB, 0xC3, 0xC3, 0xC3, 0x00
};

//define matrix for L
const byte L_pattern[8]{
  0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0x00
};

//define matrix for I
const byte I_pattern[8]{
  0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00
};

//define matrix for U
const byte U_pattern[8]{
  0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x7E, 0x00
};

//define matrix for combining OK
const byte OK_combined[32]{
  0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x63, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//define matrix for combining the name
const byte name_combined[48]{
  0x3C, 0x66, 0x60, 0x3C, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0xC3, 0xC3, 0xFF, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xE7, 0xFF, 0xDB, 0xC3, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


void setup() {
  for(int row = 0; row < 8; row++)
    pinMode(column_pins[row], HIGH);

  //set up port D for row pins
  DDRD = 0xFF;
}

void loop() {
  //uncomment the code below when demonstrating each patterns
  //scrolling_OK(OK_combined);

  scrolling_name(name_combined);

  //displaying single letters and inverse patterns, construct a loop while displaying single letter patterns
  //display_single_letter(O_pattern);
  //display_single_letter(K_pattern);
}

//single letter patterns for O and K
void display_single_letter(const byte single_letter_pattern[8]){
  for(int row = 0; row < 8; row++){
    PORTB = 0xFF;
    PORTC = 0x0F;
    PORTD = (1 << row);

    //getting row pattern
    byte current_row = single_letter_pattern[row];
    for(int column = 0; column < 8; column++){
        if(current_row & (1 << column)){
          //light up the corresponding column pins according to the row pin
          digitalWrite(column_pins[column], LOW);
        }
    } 
    //delay
    _delay_ms(2);

    //turning off the current display
    PORTD = 0x00;
  }
}
 
//inverse the patterns for O and K
void inverse_display_single_letter(const byte single_letter_pattern[8]){
  for(int row = 0; row < 8; row++){
    PORTB = 0xFF;
    PORTC = 0x0F;
    PORTD = (1 << row);

    //getting row pattern and set up the reverse signal ~
    byte current_row = ~single_letter_pattern[row];
    for(int column = 0; column < 8; column++){
        if(current_row & (1 << column)){
          //light up the corresponding column pins according to the row pin
          digitalWrite(column_pins[column], LOW);
        }
    } 
    //delay
    _delay_ms(2);

    //turning off the current display
    PORTD = 0x00;
  }
}

//scrolling for OK containing 32 bytes with spaces in between O and K
void scrolling_OK(const byte combined_OK[32]){
  for (int scroll_window = 0; scroll_window < 24; scroll_window++){
    for(int visual_speed = 0; visual_speed < 30; visual_speed++){
      for(int row = 0; row < 8; row++){
        PORTB = 0xFF;
        PORTC = 0x0F;
        PORTD = (1 << row);

        byte current_row = combined_OK[scroll_window + row];
        for(int column = 0; column < 8; column++){
            if(current_row & (1 << column)){
              //light up the corresponding column pins according to the row pin
              digitalWrite(column_pins[column], LOW);
            }
        } 
        //delay
        _delay_ms(2);

        //turning off the current display
        PORTD = 0x00;
      }
    }
    _delay_ms(180);
  }
}

//scrolling for names including 48 bytes including spaces in between each letter
void scrolling_name(const byte combined_OK[48]){
  for (int scroll_window = 0; scroll_window < 40; scroll_window++){
    for(int visual_speed = 0; visual_speed < 30; visual_speed++){
      for(int row = 0; row < 8; row++){
        PORTB = 0xFF;
        PORTC = 0x0F;
        PORTD = (1 << row);

        byte current_row = combined_OK[scroll_window + row];
        for(int column = 0; column < 8; column++){
            if(current_row & (1 << column)){
              //light up the corresponding column pins according to the row pin
              digitalWrite(column_pins[column], LOW);
            }
        } 
        //delay
        _delay_ms(2);
        
        //turning off the current display
        PORTD = 0x00;
      } 
    }
    _delay_ms(180);
  }
}

 


