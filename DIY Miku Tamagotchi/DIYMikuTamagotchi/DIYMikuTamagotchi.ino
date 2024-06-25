#include <avr/sleep.h>
#include <avr/interrupt.h>


#include <Arduino.h>
#include <font6x8.h>
#include <font8x16.h>
#include <ssd1306xled.h>
#include "image.h"
#include "image_two.h"
int mikuX = 41;
int mikuY = 2;
int wakeTime = 0;

const int oled = 3;
const int button1 = 1; // button pin on pb1
const int switchPin = 4;

// the setup routine runs once when you press reset:
void setup() {
  pinMode(switchPin, INPUT);
  digitalWrite(switchPin, HIGH);
  // initialize the digital pin as an output.
  pinMode(oled, OUTPUT);
  pinMode(button1, INPUT);
  digitalWrite(button1, HIGH);
}
void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
    } // sleep

ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
    }

// the loop routine runs over and over again forever
void loop() {

  digitalWrite(oled, HIGH); // turn the LED on (HIGH is the voltage level)
  _delay_ms(40);
  SSD1306.ssd1306_init();
  SSD1306.ssd1306_draw_bmp(mikuX, mikuY, (mikuX + 46), (mikuY + 6), miku);
  delay(10);            // wait for a second

  while (wakeTime <= 10){
    int buttonState = digitalRead(button1);  // check if the pushbutton is pressed.
    if (buttonState == LOW){
      SSD1306.ssd1306_setpos((mikuX + 5), 0);
      SSD1306.ssd1306_string_font6x8("<3");
      delay(250);
      SSD1306.ssd1306_setpos((mikuX + 5), 0);
      SSD1306.ssd1306_string_font6x8("     ");
    }
    delay(200);
    wakeTime++;
  }
  SSD1306.ssd1306_draw_bmp(mikuX, 0, (mikuX + 46), (8), mikuBlank);
  digitalWrite(oled, LOW);  // turn the LED off by making the voltage LOW
  wakeTime = 0;

  sleep();
  delay(30); 
  mikuX = 0;
}