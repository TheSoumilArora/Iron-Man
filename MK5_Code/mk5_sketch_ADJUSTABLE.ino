#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Adjust for global change in small sg90 servo movement //
#define SERVOMIN 150
#define SERVOMAX 450
/////////////////////////////////////////////////////
#define SERVO_FREQ 50

uint8_t servonum = 0;
int buttonPin = 2;
int ledPin = 6;
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
int mainServoClosed = 750;
int mainServoOpen = 1950;
int servoBrowCenterOpen = 40;
int servoBrowCenterClosed = 120;
int servoCheeksOpen = 20;
int servoCheeksClosed = 90;
int servoNoseSideOpen = 10;
int servoNoseSideClosed = 86;
int servoBrowSideOpen = 30;
int servoBrowSideClosed = 90;
int servoNoseCenterOpen = 1;
int servoNoseCenterClosed = 110;


void setup() {
  Serial.begin(9600);
  Serial.println("Boot Up");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  pinMode(buttonPin, INPUT_PULLUP);
  delay(50);
  pwm.sleep();
}

int getAngleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void loop() {

  buttonState = digitalRead(buttonPin);
  if (buttonState == 0) {
    Serial.println("Wake up");
    pwm.wakeup();

    if (globalPos > 0) {
      Serial.println("Opening");
      // CHEEKS
      for (uint16_t pulselen = servoCheeksClosed; pulselen >= servoCheeksOpen; pulselen--) {
        pwm.setPWM(9, 0, getAngleToPulse(servoCheeksClosed + servoCheeksOpen - pulselen));
        pwm.setPWM(8, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      Serial.println("1. Cheeks Open");
      // NOSE Side
      for (uint16_t pulselen = servoNoseSideClosed; pulselen >= servoNoseSideOpen; pulselen--) {
        pwm.setPWM(6, 0, getAngleToPulse(servoNoseSideClosed + servoNoseSideOpen - pulselen));
        pwm.setPWM(7, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      Serial.println("2. Nose Side Open");
      //BROW Center
      for (uint16_t pulselen = servoBrowCenterClosed; pulselen >= servoBrowCenterOpen; pulselen--) {
        pwm.setPWM(servoBrowCenter, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      Serial.println("3. Brow Center Open");
      // BROW Side
      for (uint16_t pulselen = servoBrowSideOpen; pulselen <= servoBrowSideClosed; pulselen++) {
        pwm.setPWM(2, 0, getAngleToPulse(pulselen));
        pwm.setPWM(3, 0, getAngleToPulse(servoBrowSideOpen + servoBrowSideClosed - pulselen));
      }
      delay(animDelay);
      Serial.println("4. Brow Side Open");
      // NOSE Center
      for (uint16_t pulselen = servoNoseCenterClosed; pulselen >= servoNoseCenterOpen; pulselen--) {
        pwm.setPWM(5, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      Serial.println("5. Nose Center Open");
      // EYES
      analogWrite(ledPin, 0); 
      // MAIN SERVOS
      for (uint16_t microsec = mainServoClosed; microsec < mainServoOpen; microsec +=5) {
        pwm.writeMicroseconds(mainServoL, microsec);
        pwm.writeMicroseconds(mainServoR, (mainServoOpen+mainServoClosed-microsec));
      }
      globalPos = 0;
      delay(animDelay);
      Serial.println("6. Main Open");
    } else {
      Serial.println("Closing");
      //MAIN SERVOS
      for (uint16_t microsec = mainServoOpen; microsec > mainServoClosed; microsec-=5) {
        pwm.writeMicroseconds(mainServoL, microsec);
        pwm.writeMicroseconds(mainServoR, (mainServoOpen+mainServoClosed-microsec));
      }
      delay(animDelay);
      //NOSE Center
      for (uint16_t pulselen = servoNoseCenterOpen; pulselen <= servoNoseCenterClosed; pulselen++) {
        pwm.setPWM(5, 0, getAngleToPulse(pulselen));
      }
      
      delay(animDelay);
      
      // BROW Side
      for (uint16_t pulselen = servoBrowSideClosed; pulselen >= servoBrowSideOpen; pulselen--) {
        pwm.setPWM(2, 0, getAngleToPulse(pulselen));
        pwm.setPWM(3, 0, getAngleToPulse(servoBrowSideOpen + servoBrowSideClosed - pulselen));
      }
      delay(animDelay);
      // BROW Center
      for (uint16_t pulselen = servoBrowCenterOpen; pulselen <= servoBrowCenterClosed; pulselen++) {
        pwm.setPWM(servoBrowCenter, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      // NOSE Side
      for (uint16_t pulselen = servoNoseSideOpen; pulselen <= servoNoseSideClosed; pulselen++) {
        pwm.setPWM(6, 0, getAngleToPulse(servoNoseSideClosed + servoNoseSideOpen - pulselen));
        pwm.setPWM(7, 0, getAngleToPulse(pulselen));
      }
      delay(animDelay);
      //CHEEKS
      for (uint16_t pulselen = servoCheeksOpen; pulselen <= servoCheeksClosed; pulselen++) {
        pwm.setPWM(9, 0, getAngleToPulse(servoCheeksClosed + servoCheeksOpen - pulselen));
        pwm.setPWM(8, 0, getAngleToPulse(pulselen));
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
