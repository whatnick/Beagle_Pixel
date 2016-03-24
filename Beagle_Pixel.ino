/*  OctoWS2811 Adalight.ino - Receive Adalight and display on WS2811 LEDs
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2014 Paul Stoffregen, PJRC.COM, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.


  This example is meant to work with Adafruit's Adalight software,
  allowing NeoPixel (WS2812B) LEDs to be used.

  https://learn.adafruit.com/adalight-diy-ambient-tv-lighting


  Required Connections    http://www.pjrc.com/store/octo28_adaptor.html
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.
*/

#include <OctoWS2811.h>
// set this to the hardware serial port you wish to use
#define HWSERIAL Serial1

const int ledsPerStrip = 256;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

const unsigned char right_arrow [] = {
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
  0,0,0,1,1,0,0,0,
  0,0,1,1,1,1,0,0,
  0,1,1,1,1,1,1,0,
  1,1,1,1,1,1,1,1,
};

const unsigned char stop_word [] = {
1,0,0,1,1,1,1,0,
1,0,0,1,0,0,1,0,
1,0,0,1,0,0,1,0,
1,1,1,1,0,0,1,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
};





void setup() {
  Serial.begin(115200);
  HWSERIAL.begin(115200);
  Serial.setTimeout(2000);
  leds.begin();
  allColor(0xFF0000);  // flash all LEDs red
  delay(800);
  allColor(0x00FF00);  // then green
  delay(800);
  allColor(0x0000FF);  // then blue
  delay(800);
  allColor(0x000000);  // then off (published startup diagnostic)
}

void allColor(unsigned int c) {
  for (int i=0; i < ledsPerStrip; i++) {
    leds.setPixel(i, c);
  }
  leds.show();
}

void rightArrow(unsigned int c) {
  for (int i=0; i < ledsPerStrip; i++) {
    leds.setPixel(i, c*right_arrow[i]);
  }
  leds.show();
}

void leftArrow(unsigned int c) {
  for (int i=0; i < ledsPerStrip; i++) {
    leds.setPixel(i, c*right_arrow[ledsPerStrip-i-1]);
  }
  leds.show();
}

void show_stop(unsigned int c) {
  for (int i=0; i < ledsPerStrip; i++) {
    leds.setPixel(i, c*stop_word[i-1]);
  }
  leds.show();
}

void loop() {
  int incomingByte;
        
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.print("USB received: ");
    Serial.println(incomingByte, DEC);
                HWSERIAL.print("USB received:");
                HWSERIAL.println(incomingByte, DEC);
    if(incomingByte == '1') rightArrow(0xFF0000);  // red
    if(incomingByte == '2') rightArrow(0x00FF00);  // green
    if(incomingByte == '3') rightArrow(0x0000FF);  // blue
    if(incomingByte == '4') leftArrow(0xFF0000);  // red
    if(incomingByte == '5') leftArrow(0x00FF00);  // green
    if(incomingByte == '6') leftArrow(0x0000FF);  // blue
    if(incomingByte == '7') show_stop(0xFF0000);  // red stop
    if(incomingByte == '8') allColor(0x000000);  // off
  }
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
    Serial.print("UART received: ");
    Serial.println(incomingByte, DEC);
                HWSERIAL.print("UART received:");
                HWSERIAL.println(incomingByte, DEC);
    if(incomingByte == 'a') allColor(0xFF0000);  // red
    if(incomingByte == 'b') allColor(0x00FF00);  // green
    if(incomingByte == 'c') allColor(0x0000FF);  // blue
    if(incomingByte == 'd') allColor(0x000000);  // off
  } 
}

