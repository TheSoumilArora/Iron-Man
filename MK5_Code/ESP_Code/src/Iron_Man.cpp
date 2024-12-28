#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Adafruit_BusIO_Register.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();  // Create the motor driver object

// Adjust for global change in small sg90 servo movement //
#define SERVOMIN 150
#define SERVOMAX 450

#define SERVO_FREQ 50

uint8_t servonum = 0;
int buttonPin = 5;
int ledPin = 4;
int buttonState = 0;
int globalPos = 1;
unsigned long previousMillis = 0;
unsigned long interval = 5000;
int a = 60;

int animDelay = 1;
int mainServoL = 0;
int mainServoR = 1;
int servoBrowCenter = 4;

// Adjust below numbers for individual servos
int mainServoClosed = 20;
int mainServoOpen = 90;
int servoBrowCenterOpen = 40;
int servoBrowCenterClosed = 120;
int servoCheeksOpen = 20;
int servoCheeksClosed = 90;
int servoNoseSideOpen = 10;
int servoNoseSideClosed = 86;
int servoBrowSideOpen = 30; // this should be 20
int servoBrowSideClosed = 90;
int servoNoseCenterOpen = 1;
int servoNoseCenterClosed = 110;

void setup()
{
  Serial.begin(115200);
  Serial.println("Boot Up");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  pinMode(buttonPin, INPUT_PULLUP);
  delay(50);
  pwm.sleep();
}

int getAngleToPulse(int angle)
{
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void loop()
{
  buttonState = digitalRead(buttonPin);

  if (buttonState == 0)
  {
    Serial.println("Wake up");
    pwm.wakeup();

    if (globalPos > 0) {
      Serial.println("Opening");

      // CHEEKS
      for (int angle = servoCheeksClosed; angle >= servoCheeksOpen; angle--)
      {
        pwm.setPWM(9, 0, getAngleToPulse(servoCheeksClosed + servoCheeksOpen - angle));
        pwm.setPWM(8, 0, getAngleToPulse(angle));
      }
      delay(animDelay);
      Serial.println("1. Cheeks Open");

      // NOSE Side
      for (int angle = servoNoseSideClosed; angle >= servoNoseSideOpen; angle--)
      {
        pwm.setPWM(6, 0, getAngleToPulse(servoNoseSideClosed + servoNoseSideOpen - angle));
        pwm.setPWM(7, 0, getAngleToPulse(angle));
      }
      delay(animDelay);
      Serial.println("2. Nose Side Open");

      //BROW Center
      for (int angle = servoBrowCenterClosed; angle >= servoBrowCenterOpen; angle--)
      {
        pwm.setPWM(servoBrowCenter, 0, getAngleToPulse(angle));
      }
      delay(animDelay);
      Serial.println("3. Brow Center Open");

      // BROW Side
      for (int angle = servoBrowSideOpen; angle <= servoBrowSideClosed; angle++)
      {
        pwm.setPWM(2, 0, getAngleToPulse(angle));
        pwm.setPWM(3, 0, getAngleToPulse(servoBrowSideOpen + servoBrowSideClosed - angle));
      }
      delay(animDelay);
      Serial.println("4. Brow Side Open");
      
      // NOSE Center
      for (int angle = servoNoseCenterClosed; angle >= servoNoseCenterOpen; angle--)
      {
        pwm.setPWM(5, 0, getAngleToPulse(angle));
      }
      delay(animDelay);
      Serial.println("5. Nose Center Open");
      
      // EYES
      analogWrite(ledPin, 0); 
      
      // MAIN SERVOS
      for (int angle = mainServoOpen; angle >= mainServoClosed; angle--)
      {
        pwm.setPWM(mainServoL, 0, getAngleToPulse(angle));
        pwm.setPWM(mainServoR, 0, getAngleToPulse(mainServoOpen + mainServoClosed - angle));
      }
      globalPos = 0;
      delay(animDelay);
      Serial.println("6. Main Open");
    }

    else
    {
      Serial.println("Closing");

      //MAIN SERVOS
      for (int angle = mainServoClosed; angle <= mainServoOpen; angle++)
      {
        pwm.setPWM(mainServoL, 0, getAngleToPulse(angle));
        pwm.setPWM(mainServoR, 0, getAngleToPulse(mainServoOpen + mainServoClosed - angle));
      }
      delay(animDelay);

      //NOSE Center
      for (int angle = servoNoseCenterOpen; angle <= servoNoseCenterClosed; angle++)
      {
        pwm.setPWM(5, 0, getAngleToPulse(angle));
      }
      delay(animDelay);

      // BROW Side
      for (int angle = servoBrowSideClosed; angle >= servoBrowSideOpen; angle--)
      {
        pwm.setPWM(2, 0, getAngleToPulse(angle));
        pwm.setPWM(3, 0, getAngleToPulse(servoBrowSideOpen + servoBrowSideClosed - angle));
      }
      delay(animDelay);

      // BROW Center
      for (int angle = servoBrowCenterOpen; angle <= servoBrowCenterClosed; angle++)
      {
        pwm.setPWM(servoBrowCenter, 0, getAngleToPulse(angle));
      }
      delay(animDelay);

      // NOSE Side
      for (int angle = servoNoseSideOpen; angle <= servoNoseSideClosed; angle++)
      {
        pwm.setPWM(6, 0, getAngleToPulse(servoNoseSideClosed + servoNoseSideOpen - angle));
        pwm.setPWM(7, 0, getAngleToPulse(angle));
      }
      delay(animDelay);

      //CHEEKS
      for (int angle = servoCheeksOpen; angle <= servoCheeksClosed; angle++)
      {
        pwm.setPWM(9, 0, getAngleToPulse(servoCheeksClosed + servoCheeksOpen - angle));
        pwm.setPWM(8, 0, getAngleToPulse(angle));
      }
      delay(animDelay);

      //EYES
      analogWrite(ledPin, 255);
      globalPos = 1;
      delay(100);
      Serial.println("Sleep");
      pwm.sleep();
    }
    delay(500);
  }
  delay(10);
}