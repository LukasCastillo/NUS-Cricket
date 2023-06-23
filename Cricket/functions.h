#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define SPK_OUT   22 

#define MotorA_1 8
#define MotorA_2 9
#define MotorB_1 10
#define MotorB_2 11

#define I2C1_SDA 18
#define I2C1_SCL 19

#define SR04_I2CADDR 0x57
#define OLED_I2CAADR 0x3C

#define NEO_PIXEL 20
#define LED_COUNT 3

Adafruit_NeoPixel strip(LED_COUNT, NEO_PIXEL, NEO_GRB + NEO_KHZ800);
SSD1306AsciiWire oled(Wire1);

// Code to read the range information from the UltraSonic sensor
int ping_mm()
{
    unsigned long distance = 0;
    byte i;   
    byte ds[3];
    long timeChecked;
    
    Wire1.beginTransmission(SR04_I2CADDR);
    Wire1.write(1);          //1 = cmd to start measurement.
    Wire1.endTransmission();

    delay(10); // Delay 200ms

    i = 0;
    Wire1.requestFrom(0x57,3);  //read distance       
    while (Wire1.available())
    {
     ds[i++] = Wire1.read();
    }        
    
    distance = (unsigned long)(ds[0] << 16);
    distance = distance + (unsigned long)(ds[1] << 8);
    distance = (distance + (unsigned long)(ds[2])) / 1000;
    //measured value between 10 mm (1 cm) to 6 meters (600 cm)
    if ((10 <= distance) && (6000 >= distance)) {
        return (int)distance;
    }
    else {
        return 6000;
    }
}




void playTone(unsigned int frequency, unsigned int duration){
    tone(SPK_OUT, frequency, duration - 10); 
    delay(duration);
    noTone(SPK_OUT);
}
                    
                    
// // Direction 0: forward | 1: backward
// // Speed = from 0 to 100
// // Motor  0: Motor A | 1: MOtor B
void motorMoveControl(unsigned short motor, unsigned short direction, unsigned short speed) {    
  analogWrite((motor == 0) ? MotorA_1 : MotorB_1, (direction == 0) ? 0 : ((speed > 0) && (speed < 100)? speed +150 : 0));
  analogWrite((motor == 0) ? MotorA_2 : MotorB_2, (direction == 1) ? 0 : ((speed > 0) && (speed < 100)? speed +150 : 0));
}
