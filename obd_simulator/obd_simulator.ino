
#include <PS3USB.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS3USB PS3(&Usb); 

#define Speed 3
#define Break 4
#define Steering_Left 5
#define Steering_Right 6


void setup() 
{
  
  Serial.begin(9600);
  pinMode(Speed,OUTPUT);
  pinMode(Break,OUTPUT);
  pinMode(Steering_Left,OUTPUT);
  pinMode(Steering_Right,OUTPUT);
  digitalWrite(Speed,LOW);
  digitalWrite(Break,LOW);
  digitalWrite(Steering_Left,LOW);
  digitalWrite(Steering_Right,LOW);
  
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
}


void loop() 
{
      Usb.Task();
      Serial.print("LeftHatX: ");
      Serial.print(PS3.getAnalogHat(LeftHatX));
      Serial.print("\tRightHatY: ");
      Serial.println(PS3.getAnalogHat(RightHatY));
      if(PS3.getAnalogHat(LeftHatX) > 250 )
      {
            digitalWrite(Steering_Right,HIGH);
            Serial.print("Steering Right High  ");
      }
      else if(PS3.getAnalogHat(LeftHatX) >= 127 && PS3.getAnalogHat(LeftHatX) <= 250)
      {
            digitalWrite(Steering_Right,LOW);
            //Serial.println("  Steering Right LOW ");
      }
      if(PS3.getAnalogHat(LeftHatX) < 10 )
      {
            digitalWrite(Steering_Left, HIGH);
            Serial.println("  Steering Left HIGH  ");
      }
      else if(PS3.getAnalogHat(LeftHatX) <= 127 && PS3.getAnalogHat(LeftHatX) > 10)
      {
            digitalWrite(Steering_Left, LOW);
            //Serial.println("  Steering Left LOW  ");
      }
      if(PS3.getAnalogHat(RightHatY) < 10 )
      {
            digitalWrite(Speed,HIGH);
            Serial.println("  Speed HIGH  ");
      }
      else if(PS3.getAnalogHat(RightHatY) > 10 && PS3.getAnalogHat(RightHatY) <= 127)
      {
            digitalWrite(Speed,LOW);
            //Serial.println("  Speed LOW  ");
      }
      if(PS3.getAnalogHat(RightHatY) > 250 )
      {
            digitalWrite(Break,HIGH);
            Serial.println("  Break HIGH  ");
      }
      else if(PS3.getAnalogHat(RightHatY) < 250 && PS3.getAnalogHat(RightHatY) >= 127)
      {
            digitalWrite(Break,LOW);
            //Serial.println("  Break LOW  ");
      }  
    delay(100);
   
   
}
