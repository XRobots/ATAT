#include <IBusBM.h>
IBusBM IBus; // IBus object

// ramp lib
#include <Ramp.h>

#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

Servo servo4;
Servo servo5;
Servo servo6;

Servo servo7;
Servo servo8;
Servo servo9;

Servo servo10;
Servo servo11;
Servo servo12;

int ch1;
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;
int ch7;
int ch8;
int ch9;
int ch10;

float RFB;
float RFBa;
float RFBFiltered;
float RLR;
float RLRa;
float RLRFiltered;
float RT;
float RTa;
float RTFiltered;

float LFB;
float LFBa;
float LFBFiltered;
float LLR;
float LLRa;
float LLRFiltered;
float LT;
float LTa;
float LTFiltered;

float pos1;
float pos2;
float pos3;
float pos4;
float pos5;
float pos6;
float pos7;
float pos8;
float pos9;
float pos10;
float pos11;
float pos12;

float pos1Offset;
float pos2Offset;
float pos3Offset;
float pos4Offset;
float pos5Offset;
float pos6Offset;
float pos7Offset;
float pos8Offset;
float pos9Offset;
float pos10Offset;
float pos11Offset;
float pos12Offset;

float pos1Filtered;
float pos2Filtered;
float pos3Filtered;
float pos4Filtered;
float pos5Filtered;
float pos6Filtered;
float pos7Filtered;
float pos8Filtered;

float x;
float z;
float za;
float xa;
float y;

float leg1z;
float leg2z;
float leg3z;
float leg4z;
float leg1x;
float leg2x;
float leg3x;
float leg4x;
int leg1y;
int leg2y;
int leg3y;
int leg4y;

float stride;
int stride2;

unsigned long currentMillis;
long previousMillis = 0;        // set up timers
long interval = 10;             // time constant for timer

long previousInterpMillis = 0;  // set up timers for interpolation
int interpFlag = 0;

int stepFlag = 0;
long previousStepMillis = 0;    // timers for walking states 
int stepStartFlag = 0;
int duration;
int timer1;

int duration2;
int timer2;


int stepFlag2 = 0;
long previousStepMillis2 = 0;    // timers for walking states 

float walkXPos1;
float walkXPos2;
float walkXPos3; 
float walkXPos4; 
float walkXPos5;
float walkXPos6; 
float walkXPos7; 
float walkXPos8;

float walkYPos1;
float walkYPos2;
float walkYPos3; 
float walkYPos4; 

float walkZPos1;
float walkZPos2;
float walkZPos3;
float walkZPos4;
float walkZPos5;
float walkZPos6;
float walkZPos7;
float walkZPos8;


class Interpolation {
  public:
    rampInt myRamp;
    int interpolationFlag = 0;
    int savedValue;

    int go(int input, int duration) {

      if (input != savedValue) {   // check for new data
        interpolationFlag = 0;
      }
      savedValue = input;          // bookmark the old value

      if (interpolationFlag == 0) {                                   // only do it once until the flag is reset
        myRamp.go(input, duration, LINEAR, ONCEFORWARD);              // start interpolation (value to go to, duration)
        interpolationFlag = 1;
      }

      int output = myRamp.update();
      return output;
    }
};    // end of class

Interpolation interpFRX;        // interpolation objects
Interpolation interpFRY;
Interpolation interpFRZ;

Interpolation interpFLX;        // interpolation objects
Interpolation interpFLY;
Interpolation interpFLZ;

Interpolation interpBRX;        // interpolation objects
Interpolation interpBRY;
Interpolation interpBRZ;

Interpolation interpBLX;        // interpolation objects
Interpolation interpBLY;
Interpolation interpBLZ;

void setup() {

    Serial.begin(115200);
    
    IBus.begin(Serial3, IBUSBM_NOTIMER);    // change to Serial1 or Serial2 port when required
    
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    
    servo1.attach(22);
    servo2.attach(24); 
    servo3.attach(26); 
    servo4.attach(28);
    servo5.attach(30); 
    servo6.attach(32);
    
    servo7.attach(34);
    servo8.attach(36); 
    servo9.attach(38);
    servo10.attach(40);
    servo11.attach(42); 
    servo12.attach(44);

    // offsets for procesise home positions
        
    pos1Offset = 100;
    pos2Offset = -80;
    pos3Offset = 150;
    pos4Offset = -100;
    pos5Offset = 0;
    pos6Offset = 0;
    pos7Offset = 60;
    pos8Offset = 0;
    pos9Offset = 50;
    pos10Offset = -50;
    pos11Offset = -50;
    pos12Offset = 0;   

}

void loop() {

  currentMillis = millis();
  if (currentMillis - previousMillis >= 10) {  // start timed event    
      previousMillis = currentMillis;

      IBus.loop();

      ch1 = IBus.readChannel(0); // get latest value for servo channel 0
      ch2 = IBus.readChannel(1); // get latest value for servo channel 1
      ch3 = IBus.readChannel(2); // get latest value for servo channel 3
      ch4 = IBus.readChannel(3); // get latest value for servo channel 4
      ch5 = IBus.readChannel(4); // get latest value for servo channel 5
      ch6 = IBus.readChannel(5); // get latest value for servo channel 6
      ch7 = IBus.readChannel(6); // get latest value for servo channel 7
      ch8 = IBus.readChannel(7); // get latest value for servo channel 8
      ch9 = IBus.readChannel(8); // get latest value for servo channel 9
      ch10 = IBus.readChannel(9); // get latest value for servo channel 10

      /*
      Serial.print(ch1);
      Serial.print(" , ");
      Serial.print(ch2);
      Serial.print(" , ");
      Serial.print(ch3);
      Serial.print(" , ");
      Serial.print(ch4);
      Serial.print(" , ");
      Serial.print(ch5);
      Serial.print(" , ");
      Serial.print(ch6);
      Serial.print(" , ");
      Serial.print(ch7);
      Serial.print(" , ");
      Serial.print(ch8);
      Serial.print(" , ");
      Serial.print(ch9);
      Serial.print(" , ");
      Serial.print(ch10);
      Serial.println();
      */

      LFB = ch1;
      RFB = ch4;
      RLR = ch2;
      RT = ch6;
      LLR = ch3;
      LT = ch5;
      
      // threshold sticks

      RFBa = thresholdStick(RFB);
      RLRa = thresholdStick(RLR);
      RTa = thresholdStick(RT);
      LFBa = thresholdStick(LFB);
      LLRa = thresholdStick(LLR);
      LTa = thresholdStick(LT);      

      // filter sticks

      RFBFiltered = filter(RFBa, RFBFiltered,20);
      RLRFiltered = filter(RLRa, RLRFiltered,20);
      RTFiltered = filter(RTa, RTFiltered,20);
      LFBFiltered = filter(LFBa, LFBFiltered,20);
      LLRFiltered = filter(LLRa, LLRFiltered,20);
      LTFiltered = filter(LTa, LTFiltered,20);

      if (ch7 < 1300) {               // kinematics test mode
          za =  map(LTFiltered,-255,255,240,295);
          xa =  map(RFBFiltered,-255,255,60,-60);
    
          z = (float) za/1000;
          x = (float) xa/1000;

          //Serial.println(x);

          kinematics(1, x, y ,z ,0, 0);
          kinematics(2, x, y ,z ,0, 0);
          kinematics(3, x, y ,z ,0, 0);
          kinematics(4, x, y ,z ,0, 0);
      }      

      else if (ch7 > 1400 && ch7 < 1700) {                          // walking mode

        timer1 = 400;
        duration = 400;

        // define walking positions

        stride = RFBFiltered/-230;  // sclae stick for stride length.

        walkXPos1 = stride * 0.05;
        walkXPos2 = stride * 0;
        walkXPos3 = stride * -0.05; 
        walkXPos4 = stride * -0.025; 
        walkXPos5 = stride * -0.0125;
        walkXPos6 = stride * 0; 
        walkXPos7 = stride * 0.0125; 
        walkXPos8 = stride * 0.022;

        walkZPos1 = 0.28;    // leg down
        walkZPos2 = 0.25;    // leg up
        walkZPos3 = 0.28;    // leg down
        walkZPos4 = 0.28;    // leg down
        walkZPos5 = 0.28;    // leg down
        walkZPos6 = 0.28;    // leg down
        walkZPos7 = 0.28;    // leg down
        walkZPos8 = 0.28;    // leg down

        if (stepFlag == 0 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;
          stepFlag = 1;
          leg1x = walkXPos3;
          leg1z = walkZPos3;
          leg2x = walkXPos7;
          leg2z = walkZPos7;
          leg3x = walkXPos5;
          leg3z = walkZPos5;
          leg4x = walkXPos1;
          leg4z = walkZPos1;        
        }
        else if (stepFlag == 1 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 2;
          leg1x = walkXPos4;
          leg1z = walkZPos4;
          leg2x = walkXPos8;
          leg2z = walkZPos8;
          leg3x = walkXPos6;
          leg3z = walkZPos6;
          leg4x = walkXPos2;
          leg4z = walkZPos2;
        }
        else if (stepFlag == 2 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 3;
          leg1x = walkXPos5;
          leg1z = walkZPos5;
          leg2x = walkXPos1;
          leg2z = walkZPos1;
          leg3x = walkXPos7;
          leg3z = walkZPos7;
          leg4x = walkXPos3;
          leg4z = walkZPos3;
        }
        else if (stepFlag == 3 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 4;
          leg1x = walkXPos6;
          leg1z = walkZPos6;
          leg2x = walkXPos2;
          leg2z = walkZPos2;
          leg3x = walkXPos8;
          leg3z = walkZPos8;
          leg4x = walkXPos4;
          leg4z = walkZPos4;
        }
        else if (stepFlag == 4 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 5;
          leg1x = walkXPos7;
          leg1z = walkZPos7;
          leg2x = walkXPos3;
          leg2z = walkZPos3;
          leg3x = walkXPos1;
          leg3z = walkZPos1;
          leg4x = walkXPos5;
          leg4z = walkZPos5;
        }
        else if (stepFlag == 5 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 6;
          leg1x = walkXPos8;
          leg1z = walkZPos8;
          leg2x = walkXPos4;
          leg2z = walkZPos4;
          leg3x = walkXPos2;
          leg3z = walkZPos2;
          leg4x = walkXPos6;
          leg4z = walkZPos6;
        }
        else if (stepFlag == 6 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 7;
          leg1x = walkXPos1;
          leg1z = walkZPos1;
          leg2x = walkXPos5;
          leg2z = walkZPos5;
          leg3x = walkXPos3;
          leg3z = walkZPos3;
          leg4x = walkXPos7;
          leg4z = walkZPos7;
        }
        else if (stepFlag == 7 && currentMillis - previousStepMillis > timer1) {
          previousStepMillis = currentMillis;          
          stepFlag = 0;
          leg1x = walkXPos2;
          leg1z = walkZPos2;
          leg2x = walkXPos6;
          leg2z = walkZPos6;
          leg3x = walkXPos4;
          leg3z = walkZPos4;
          leg4x = walkXPos8;
          leg4z = walkZPos8;
        }

        kinematics(1, leg1x, 0 ,leg1z ,1, duration);
        kinematics(2, leg2x, 0 ,leg2z ,1, duration);
        kinematics(3, leg3x, 0 ,leg3z ,1, duration);
        kinematics(4, leg4x, 0 ,leg4z ,1, duration);      
        
      }

      else if (ch7 > 1700) {

        timer2 = 400;
        duration2 = 400;

        stride2 = LTFiltered*-1;  // sclae stick for stride length.

        walkZPos1 = 0.28;    // leg down
        walkZPos2 = 0.25;    // leg up

        if (stepFlag2 == 0 && currentMillis - previousStepMillis2 > timer2) {
          previousStepMillis2 = currentMillis;
          stepFlag2 = 1;
          leg1z = walkZPos1;
          leg2z = walkZPos2;
          leg3z = walkZPos2;
          leg4z = walkZPos1;          
        }
        if (stepFlag2 == 1 && currentMillis - previousStepMillis2 > timer2) {
          previousStepMillis2 = currentMillis;
          stepFlag2 = 2;
          leg1y = stride2;
          leg2y = stride2*-1;
          leg3y = stride2*-1;
          leg4y = stride2;
        }
        else if (stepFlag2 == 2 && currentMillis - previousStepMillis2 > timer2) {
          previousStepMillis2 = currentMillis;
          stepFlag2 = 3;
          leg1z = walkZPos1;
          leg2z = walkZPos1;
          leg3z = walkZPos1;
          leg4z = walkZPos1;
        } 
        else if (stepFlag2 == 3 && currentMillis - previousStepMillis2 > timer2) {
          previousStepMillis2 = currentMillis;
          stepFlag2 = 4;
          leg1z = walkZPos2;
          leg2z = walkZPos1;
          leg3z = walkZPos1;
          leg4z = walkZPos2;                 
        }
        else if (stepFlag2 == 4 && currentMillis - previousStepMillis2 > timer2) {
          previousStepMillis2 = currentMillis;
          stepFlag2 = 5;
          leg1y = stride2*-1;
          leg2y = stride2;
          leg3y = stride2;
          leg4y = stride2*-1;          
        }
        else if (stepFlag2 == 5 && currentMillis - previousStepMillis2 > timer2) {
          previousStepMillis2 = currentMillis;
          stepFlag2 = 0;
          leg1z = walkZPos1;
          leg2z = walkZPos1;
          leg3z = walkZPos1;
          leg4z = walkZPos1;
        }



        //Serial.println(leg1y);

        leg1x = 0;  
        kinematics(1, leg1x, leg1y ,leg1z ,1, duration2);
        kinematics(2, leg2x, leg2y ,leg2z ,1, duration2);
        kinematics(3, leg3x, leg3y ,leg3z ,1, duration2);
        kinematics(4, leg4x, leg4y ,leg4z ,1, duration2); 
        
      }
      

  
      

  }   // end of timed loop

}



